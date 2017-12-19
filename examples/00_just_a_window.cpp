#include <cstring>
#include <cstdio>

#include "simple/graphical/window.h"
#include "simple/graphical/algorithm.h"

using namespace simple;

using point2D = graphical::point2D;
using Window = graphical::window;
using WFlags = Window::flags;

int main() try
{
	SDL_Init(SDL_INIT_EVERYTHING);
	{
		Window win("Nomo", point2D(640, 480), WFlags::shown | WFlags::borderless);
		graphical::fill(win.surface(), win.surface().format().color(0x77, 0x00, 0xAA));
		win.update();
		SDL_Delay(1313);
	}
	SDL_Quit();
	return 0;
}
catch(...)
{
	if(errno)
		std::puts(std::strerror(errno));
	throw;
}
