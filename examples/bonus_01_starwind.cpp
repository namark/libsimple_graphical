#include <cstdio>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"
#include "simple/support/algorithm.hpp"
#include "simple/support/misc.hpp"

using namespace simple::graphical;
using simple::support::ston;

using namespace std::chrono_literals;
using namespace color_literals;

using float3 = simple::geom::vector<float, 3>;
using float2 = simple::geom::vector<float, 2>;
using int2 = simple::geom::vector<int, 2>;

void set_subpixel(const pixel_writer<rgba_pixel, pixel_byte>&, rgba_vector, float2);
void set_pixel(const pixel_writer<rgba_pixel, pixel_byte>&, rgba_vector, float2);
void set_radius(const pixel_writer<rgba_pixel, pixel_byte>&, rgba_vector, float2); // TODO

int main(int argc, char const* argv[]) try
{

	switch(argc)
	{
		case 0: std::puts("Command not specified??");
		case 1: std::puts("Distance not specified");
		case 2: std::puts("Count not specified");
		case 3: std::puts("Speed not specified");
		case 4: std::puts("Render mode not specified.");
		case 5: std::puts("Color not specified");
			return -1;
	}

	const auto distance = ston<float>(argv[1]);
	const auto count = ston<size_t>(argv[2]);
	const auto speed = ston<float>(argv[3]);
	const auto pixel_setter = strcmp("subpixel", argv[4]) == 0 ? set_subpixel : set_pixel;
	const rgb_vector color { rgb_pixel::from(ston<rgb_pixel::int_type>(argv[5])) };

	const auto frame_time = 16ms;

	std::minstd_rand rand(std::random_device{}());
	std::uniform_real_distribution<float> z_dist(.01f,1);
	std::uniform_real_distribution<float> xy_dist(-1,1);

	std::vector<float3> stars{count};
	for(auto&& star : stars)
	{
		for(auto&& coordinate : star)
			coordinate = xy_dist(rand);
		star.z() = z_dist(rand);
	}

	initializer init;

	software_window win("Starwind", {600,600}, window::flags::borderless);
	auto pixels = std::get<pixel_writer<rgba_pixel, pixel_byte>>(win.surface().pixels());
	auto half_size = float2(win.surface().size()) / 2;

	for
	(
		auto [position, next_frame_time] = std::tuple{0.f, std::chrono::high_resolution_clock::now()};
		position < distance;
		position += speed,
		next_frame_time += frame_time,
		std::this_thread::sleep_until(next_frame_time)
	)
	{
		fill(win.surface(), win.surface().format().color(0_rgb));
		for(auto&& star : stars)
		{
			star.z() -= speed;
			if(star.z() < .01f)
				star.z() = 1.f;
			const auto perspective_star = star.xy() / star.z();
			if(-float2::one() < perspective_star && perspective_star < float2::one())
				pixel_setter(pixels, color * (-star.z() + 1), (half_size + perspective_star * (half_size)));
		}
		win.update();
	}

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

void set_pixel(const pixel_writer<rgba_pixel, pixel_byte>& pixels, rgba_vector pixel, float2 position)
{
	pixels.set(rgba_pixel(pixel), int2(position) );
}

void set_subpixel(const pixel_writer<rgba_pixel, pixel_byte>& pixels, rgba_vector pixel, float2 position)
{
	auto floor = int2(position);
	auto fraction = position - float2(floor);

	auto i = float2::zero();
	auto begin = i.rbegin();
	auto end = (floor[0] != pixels.size()[0] - 1) ? i.rend() : i.rend() - 1;
	auto terminator = (floor[1] != pixels.size()[1] - 1) ? end : end - 1;
	do
	{
		auto ratio = (float2::one() - fraction)*(float2::one() - i) + fraction * i;
		rgba_vector old_color {pixels.get(floor + int2(i))};
		auto opacity = std::accumulate(ratio.begin(), ratio.end(), 0.f)/ratio.dimensions;
		pixels.set(rgba_pixel(pixel * opacity + old_color * (1 - opacity)), floor + int2(i));
	}
	while(simple::support::next_number(begin, end) != terminator);
}
