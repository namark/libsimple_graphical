#include <cstdio>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;

int main(int argc, char const* argv[]) try
{
	if(argc < 2)
	{
		std::puts("Image not specified.");
		return -1;
	}

	initializer init;

	surface image(argv[1]);
	software_window win("Image", image.size(), window::flags::borderless);
	blit(image, win.surface());
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
