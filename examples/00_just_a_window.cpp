#include <cstdio>
#include <cerrno>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.hpp"

using namespace simple::graphical;
using namespace color_literals;

int main() try
{
	initializer init;

	software_window win("Borderless window", int2(640, 480), window::flags::borderless);
	fill(win.surface(), win.surface().format().color(0x7700AA_rgb));
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
