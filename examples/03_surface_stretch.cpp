#include <cstdio>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"
#include "simple/support/misc.hpp"

using namespace simple::graphical;
using simple::support::ston;

int main(int argc, char const* argv[]) try
{
	switch(argc)
	{
		case 0: std::puts("What even is?");
		case 1: std::puts("Image not specified.");
		case 2: std::puts("Width not specified.");
		case 3: std::puts("Height not specified.");
			return -1;
	}

	int2 size { ston<int>(argv[2]), ston<int>(argv[3]) };

	initializer init;

	software_window win("Stretchy", size, window::flags::borderless);
	surface image(argv[1]);

	// blit will stretch if a destination rectangle is specified
	blit(image, win.surface(), rect{win.surface().size()});

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
