#include <cstdio>
#include <cerrno>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm/blit.h"
#include "simple/support/algorithm.hpp"
#include "simple/support/misc.hpp"

using namespace simple::graphical;
using namespace std::chrono_literals;
using namespace simple::graphical::color_literals;

using simple::support::advance_vector;
using simple::support::range;

// dumb sort
// smarter sorts don't work
template <typename It>
void selection_sort(It begin, It end)
{
	for(auto i = begin; i != end; ++i)
	{
		auto min = i;
		for(auto j = i+1; j != end; ++j)
			if(*j < *min)
				min = j;
		std::swap(*i, *min);
	}
}

// dumb equal range
// smarter one below works, but not as well
template <typename It>
range<It> equal_range(It begin, It end)
{
	auto i = begin+1;
	for(;i != end &&
		(
			!(*i < *(begin))
			&&
			!(*(begin) < *i)
		);
	++i)
	{}
	return {begin,i};
}

// template <typename It>
// range<It> equal_range(It begin, It end)
// {
// 	auto r = std::equal_range(begin, end, *begin);
// 	return {r.first,r.second};
// }

int main(int argc, char** argv) try
{

	initializer init;

	int2 size{};
	auto flags = window::flags::borderless;

	using simple::support::ston;
	if(argc == 2)
	{
		size = int2::one(ston<int>(argv[1]));
	}
	else if(argc > 2)
	{
		size = int2{
			ston<int>(argv[1]),
			ston<int>(argv[2])
		};
	}
	else
	{
		std::puts("Running in fullscreen mode. This could take a long time...\n");
		flags = flags | window::flags::fullscreen_desktop;
	}

	software_window win("unnatural order", size, flags);

	std::vector<int2> points;

	// generate all points
	auto i = int2::zero();
	auto dimension = i.begin();
	while(dimension != i.end())
	{
		points.push_back(i);
		dimension = advance_vector(i, int2::zero(), win.size());
	}

	// if you skip this, selection sort has no effect
	std::shuffle(points.begin(), points.end(), std::mt19937(std::random_device{}()));

	selection_sort(points.begin(), points.end());

	surface canvas(win.surface().size(), pixel_format(pixel_format::type::rgb24));
	auto pixels = std::get<pixel_writer<rgb_pixel, surface::byte>>(canvas.pixels());

	auto colors = std::array{
		0x000000_rgb,
		0x111111_rgb,
		0x222222_rgb,
		0x333333_rgb,
		0x444444_rgb,
		0x555555_rgb,
		0x666666_rgb,
		0x777777_rgb,
		0x888888_rgb,
		0x999999_rgb,
		0xaaaaaa_rgb,
		0xbbbbbb_rgb,
		0xcccccc_rgb,
		0xdddddd_rgb,
		0xeeeeee_rgb,
		0xffffff_rgb};
	int color_index = 0;
	for(auto i = points.begin(); i != points.end();
		std::this_thread::sleep_for(100ms))
	{
		auto range = equal_range(i, points.end());
		for(auto point : range)
			pixels.set( colors[color_index], point );
		i = range.end();
		color_index = (color_index + 1) % colors.size();
		blit(canvas, win.surface());
		win.update();
	}

	canvas.save("unnatural_order.bmp");
	std::this_thread::sleep_for(3313ms);

	return 0;
}
catch(...)
{
	if(errno)
		std::perror("ERROR");

	const char* sdl_error = SDL_GetError();
	if(*sdl_error)
		std::puts(sdl_error);

	throw;
}
