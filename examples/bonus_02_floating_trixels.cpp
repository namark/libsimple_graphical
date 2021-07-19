#include <cstdio>
#include <thread>
#include <random>
#include <optional>
#include <iostream>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm/blit.h"
#include "simple/graphical/algorithm/fill.h"
#include "simple/support/misc.hpp"
#include "simple/support/random/distribution/diagonal.hpp"

using namespace simple::graphical;
using namespace color_literals;
using namespace std::chrono_literals;

using simple::geom::vector;
using float2x2 = vector<float2, 2>;

using namespace simple::support::random;

struct triangle
{
	float2 position;
	float2x2 transform;
};

struct floating_pixel
{
	float2 position;
	float2 destination;
};

int main(int argc, char const* argv[]) try
{
	const int zoom = argc > 1 ? simple::support::ston<int>(argv[1]) : 16;
	if(zoom <= 0)
		throw std::logic_error("zoom is not positive!");

	const unsigned waypoint_count = argc > 2 ? simple::support::ston<unsigned>(argv[2]) : 5;
	const float fastity = argc > 3 ? simple::support::ston<float>(argv[3]) : 0.1;
	const unsigned pixel_count = argc > 4 ? simple::support::ston<unsigned>(argv[4]) : 10000;

	initializer init;

	software_window win("Floating trixels", int2::one(400), window::flags::borderless);

	const auto logical_size = win.surface().size() / zoom;

	std::optional<surface> zoomed_canvas = std::nullopt;
	if(zoom > 1)
		zoomed_canvas = surface (logical_size, pixel_format(pixel_format::type::rgba8888));
	const surface& canvas = zoomed_canvas ? *zoomed_canvas : win.surface();

	std::default_random_engine gen(std::random_device{}());
	std::uniform_real_distribution<float> dist(0,logical_size.x());
	distribution::triangle<decltype(dist)> tri_dist(0,1);
	auto random_float2 = [&gen, &dist]()
	{ return float2(dist(gen), dist(gen)); };
	auto random_float2_tri = [&gen, &tri_dist]()
	{ return float2(tri_dist(gen), tri_dist(gen)); };

	std::vector<float2> waypoints(waypoint_count*3);
	std::generate(waypoints.begin(), waypoints.end(), random_float2);

	std::vector<triangle> triangles(waypoint_count);
	for(size_t i = 0; i < waypoint_count; ++i)
	{
		size_t j = i*3;
		auto& t = triangles[i];
		t.position = waypoints[j];
		t.transform[0] = waypoints[j+1] - t.position;
		t.transform[1] = waypoints[j+2] - t.position;
	}

	std::vector<floating_pixel> floaters(pixel_count);

	auto pixels = std::get<pixel_writer<rgba_pixel, surface::byte>>(canvas.pixels());
	for
	(
		auto target = triangles.begin();
		target != triangles.end();
		std::this_thread::sleep_for(16ms)
	)
	{
		fill(canvas, canvas.format().color(0x000000ff_rgba));
		for(auto&& pixel : floaters)
			pixels.set(0xff00ff55_rgba, pixel.position);

		if(canvas != win.surface())
			blit(canvas, win.surface(), rect{win.surface().size()});

		win.update();

		auto any_pixel = floaters[0];
		float2 diff = any_pixel.position - any_pixel.destination;
		if(diff(diff) < 0.01)
		{
			++target;
			for(auto&& pixel : floaters)
				pixel.destination = target->position + target->transform(random_float2_tri());
		}
		else
		{
			for(auto&& pixel : floaters)
				pixel.position += (pixel.destination - pixel.position) * fastity;
		}

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

