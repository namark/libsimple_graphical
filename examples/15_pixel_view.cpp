#include <cstdio>
#include <cerrno>
#include <cassert>

#include <algorithm>
#include <thread>
#include <chrono>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/pixel_format.h"
#include "simple/graphical/algorithm/blit.h"
#include "simple/graphical/algorithm/fill.h"
#include "simple/graphical/pixels.h"
#include "simple/support/algorithm.hpp"

using namespace simple::graphical;
using namespace simple::graphical::color_literals;
using namespace std::chrono_literals;

using simple::support::all_of;
using simple::support::range;

int main() try
{
	initializer graphics;

	software_window win("pixel view", {400,400}, window::flags::borderless);

	surface playground{win.size(), pixel_format(pixel_format::type::rgb24)};

	// pixel_reader and pixel_writer are non-owning, lightweight
	// views on pixels data

	// surface format is dynamic so we have to select pixel type at runtime
	// in this case we know exactly which format to get
	const auto all_pixels = std::get<
		// pixel_writer can read and write pixels,
		pixel_writer<rgb_pixel, surface::byte>
	>(playground.pixels());

	// pixel_reader can only read
	const auto all_pixels_const = pixel_reader(all_pixels);
	// you can construct a reader from a writer but NOT vise versa

	// lets fill the surface with something ineresting
	const auto blue = playground.format().color(0x0000ff_rgb); // type erased color
	const auto box = range{int2::zero(), int2::one(200)};
	fill(pixel_writer(all_pixels, box), 0xff0000_rgb); // top left
	fill(pixel_writer(all_pixels, box + int2::i(200)), rgb_pixel::green()); // top right
	fill(pixel_writer(all_pixels, box + int2::j(200)), blue); // bottom left

	blit(playground, win.surface());
	win.update();
	std::this_thread::sleep_for(1s);

	// not lets copy everything to the bottom right corner in place
	// for an interesting effect
	// TODO: blit algorithms for pixel views with pixel type conversion and scaling
	const auto bottom_right = pixel_writer(all_pixels,box + 200);
	auto from = int2::zero();
	auto to = all_pixels.size();
	auto i = from;
	auto dimension = i.begin();
	while(dimension != i.end())
	{
		bottom_right.set(all_pixels.get(i), i/2);
		dimension = simple::support::advance_vector(i.begin(), i.end(), from.begin(), to.begin());
	}

	blit(playground, win.surface());
	win.update();
	std::this_thread::sleep_for(3s);

	// dangerous stuff

	// raw_range is a range of raw bytes
	// the rows of pixels might not be consecutive in memory
	// there could be alignemnt padding between them
	// or worse pixels of a "parent" image
	// don't use this unless you know what you're doing
	//
	// in this case we know it's padding at worst
	auto raw_bytes = all_pixels.raw_range();
	// set it all to some value
	std::fill(raw_bytes.begin(), raw_bytes.end(), 13);
	// check the value through reader
	assert(( all_of(all_pixels_const.raw_range(),
		[](auto b){ return b == 13; }) ));


	blit(playground, win.surface());
	win.update();

	std::this_thread::sleep_for(1313ms);

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
