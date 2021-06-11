#include <cstdio>
#include <cerrno>
#include <thread>
#include <random>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm/blit.h"
#include "simple/graphical/algorithm/fill.h"
#include "simple/graphical/algorithm/line.hpp"
#include "simple/support/misc.hpp"

using namespace simple::graphical;
using namespace color_literals;
using namespace std::chrono_literals;
using simple::geom::vector;
using simple::support::min_element;
using rgb_pixels = pixel_writer<rgb_pixel, surface::byte>;

const auto tau = 2*std::acos(-1);
float2 from_angle(float angle) { return {cos(angle), sin(angle)}; }

void integer_dda_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value);
void floating_dda_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value);

void integer_vector_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value);
void floating_vector_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value);
void radius_vector_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value); // TODO

void bresenham_line(const line<int2> line, rgb_pixels pixels, rgb_pixel value);
void xiaolin_wu_line(const line<int2> line, rgb_pixels pixels, rgb_pixel value);

void perfect_line(const line<float2> line, rgb_pixels pixels, rgb_pixel value); // TODO

int main(int argc, char const* argv[]) try
{
	initializer init;

	software_window win("Line drawing", int2::one(400), window::flags::borderless);

	const unsigned zoom = argc > 1 ? simple::support::ston<unsigned>(argv[1]) : 1;

	const auto logical_size = win.surface().size() / zoom;
	surface canvas (logical_size, pixel_format(pixel_format::type::rgb24));
	auto pixels = std::get<rgb_pixels>(canvas.pixels());

	const int half_size = min_element(win.size())/2 - 1; // -1 becaus we set the enpoints
	const float offset = tau/10;
	const float radius = half_size / zoom;
	const auto position = float2::one(half_size) / zoom;
	const auto color = 0xffffff_rgb;
	for(float i = 0; i < tau; i += tau*0.001f)
	{
		float angle = i;
		fill(canvas, canvas.format().color(0x000000_rgb));

		integer_dda_line({position + from_angle(angle) * radius/2, position + from_angle(angle) * radius}, pixels, color);

		angle -= offset;
		floating_dda_line({position + from_angle(angle) * radius/2, position + from_angle(angle) * radius}, pixels, color);

		angle -= offset;
		integer_vector_line({position + from_angle(angle) * radius/2, position + from_angle(angle) * radius}, pixels, color);

		angle -= offset;
		floating_vector_line({position + from_angle(angle) * radius/2, position + from_angle(angle) * radius}, pixels, color);

		angle -= offset;
		bresenham_line({int2(round(position + from_angle(angle) * radius/2)), int2(round(position + from_angle(angle) * radius))}, pixels, color);

		angle -= offset;
		xiaolin_wu_line({int2(round(position + from_angle(angle) * radius/2)), int2(round(position + from_angle(angle) * radius))}, pixels, color);


		blit(canvas, win.surface(), rect{win.surface().size()});
		win.update();
		std::this_thread::sleep_for(16ms);
	}
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

void floating_dda_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value)
{
	const auto setter = [&pixels, &value](const float2& position)
	{
		pixels.set<rgb_vector>(value, position);
	};
	const auto tail = dda_line(line, setter);
	const auto tail_size = tail.end() - tail.begin();
	rgba_vector end_value = rgb_vector(value);
	end_value.a() = std::clamp(tail_size.length(),0.f,1.f);
	pixels.set(end_value, tail.end());
}

void integer_dda_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value)
{
	const auto setter = [&pixels, &value](const float2& position)
	{
		pixels.set(value, int2(round(position)));
	};
	setter(dda_line(line, setter).end());
}

void floating_vector_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value)
{
	const auto setter = [&pixels, &value](const float2& position)
	{
		pixels.set<rgb_vector>(value, position);
	};
	const auto tail = vector_line(line, setter);
	const auto tail_size = tail.end() - tail.begin();
	rgba_vector end_value = rgb_vector(value);
	end_value.a() = std::clamp(tail_size.length(),0.f,1.f);
	pixels.set(end_value, tail.end());
}

void integer_vector_line(const line<float2>& line, rgb_pixels pixels, rgb_pixel value)
{
	const auto setter = [&pixels, &value](const float2& position)
	{
		pixels.set(value, int2(round(position)));
	};
	setter(vector_line(line, setter).end());
}

void bresenham_line(line<int2> line, rgb_pixels pixels, rgb_pixel value)
{
	bresenham_line(line, [&pixels, &value](int2 position)
	{
		pixels.set(value, position);
	});
}

void xiaolin_wu_line(line<int2> line, rgb_pixels pixels, rgb_pixel value)
{
	using rational = rational<int>;
	bresenham_line(line, [&pixels, &value](int2 position, rational ratio)
	{
		const auto blended = value.mutant_clone([&ratio](auto color)
			{ return rgb_pixel::coordinate_type(int(color * ratio)); });
		pixels.set(rgb_pixel{blended}, position);
	});
}
