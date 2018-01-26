#include <cstdio>

#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"

using namespace simple;
using namespace graphical::color_literals;

using point2D = graphical::point2D;
using Window = graphical::software_window;
using WFlags = Window::flags;

int main() try
{
	SDL_Init(SDL_INIT_EVERYTHING);
	{
		Window win("Borderless window", point2D(640, 480), WFlags::borderless);
		graphical::fill(win.surface(), win.surface().format().color(0x7700AA_rgb));
		win.update();
		SDL_Delay(1313);
	}
	SDL_Quit();
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
