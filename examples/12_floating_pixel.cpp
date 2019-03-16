#include <cstdio>
#include <cerrno>
#include <thread>
#include <random>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"
#include "simple/support/misc.hpp"

using namespace simple::graphical;
using namespace color_literals;
using namespace std::chrono_literals;

int main(int argc, char const* argv[]) try
{
	initializer init;

	software_window win("Floating pixel", int2::one(400), window::flags::borderless);

	const unsigned zoom = argc > 1 ? simple::support::ston<unsigned>(argv[1]) : 16;
	const unsigned waypoint_count = argc > 2 ? simple::support::ston<unsigned>(argv[2]) : 5;
	const float fastity = argc > 3 ? simple::support::ston<float>(argv[3]) : 0.1;

	const auto logical_size = win.surface().size() / zoom;
	surface canvas (logical_size, pixel_format(pixel_format::type::rgb24));
	auto pixels = std::get<pixel_writer<rgb_pixel, surface::byte>>(canvas.pixels());

	std::default_random_engine gen(std::random_device{}());
	std::uniform_real_distribution<float> dist(0,logical_size.x());
	auto random_float2 = [&dist, &gen]()
	{
		return float2(dist(gen), dist(gen));
	};

	float2 pos = random_float2();
	std::vector<float2> waypoints(waypoint_count);
	std::generate(waypoints.begin(), waypoints.end(), random_float2);

	// edge cases
	waypoints.push_back(float2(logical_size) - float2(0.74f, 5.82f));
	waypoints.push_back(float2(logical_size) - float2(5.82f, 0.74f));
	waypoints.push_back(float2(logical_size) - float2(0.74f, 0.54f));

	for
	(
		auto target = waypoints.begin();
		target != waypoints.end();
		std::this_thread::sleep_for(16ms)
	)
	{
		fill(canvas, canvas.format().color(0x000000_rgb));
		pixels.set<rgb_vector>(0x00ffff_rgb, *target);
		pixels.set<rgb_vector>(0xff00ff_rgb, pos);
		blit(canvas, win.surface(), rect{win.surface().size()});
		win.update();

		float2 diff = pos - *target;
		if(diff(diff) < 0.01)
			++target;
		else
			pos += (*target - pos) * fastity;
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
