#include <cstdio>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;

int main() try
{

	initializer init;

	software_window win("Gradient", {600,600}, window::flags::borderless);

	surface canvas(win.surface().size(), pixel_format(pixel_format::type::rgb24));
	auto pixels = std::get<pixel_writer<rgb_pixel, surface::byte>>(canvas.pixels());

	auto start = rgb_vector(0.0f,1.0f,1.0f);
	auto end = rgb_vector(1.0f,0.0f,1.0f);
	rgb_vector color;
	for(point2D position{}; position < pixels.size(); ++position.y())
	{
		for(; position < pixels.size(); ++position.x())
		{
			auto ratio = vector2D(position) / vector2D(pixels.size());
			color = start + (end - start) * rgb_vector(ratio.xyz(1.0f));
			pixels.set( rgb_pixel(color), position );
		}
		position.x() = 0;
	}

	blit(canvas, win.surface());
	win.update();
	SDL_Delay(1313);

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
