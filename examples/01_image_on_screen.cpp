#include <cstring>
#include <cstdio>

#include "simple/graphical/window.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;

int main(int argc, char const* argv[]) try
{
	if(argc < 2)
	{
		puts("Image not specified.");
		return -1;
	}

	SDL_Init(SDL_INIT_EVERYTHING);
	{
		surface image(argv[1]);
		window win("Image", image.size(), window::flags::borderless);
		blit(image, win.surface());
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

	const char* sdl_error = SDL_GetError();
	if(*sdl_error)
		std::puts(sdl_error);

	throw;
}
