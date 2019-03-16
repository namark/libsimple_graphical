#include <cstdio>
#include <cerrno>
#include <random>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;

int2 randomPoint(int2 max)
{
	static std::minstd_rand rand(std::random_device{}());
	std::uniform_int_distribution<short> dist;
	return int2(dist(rand), dist(rand)) % max;
}

int main(int argc, char const* argv[]) try
{
	if(argc < 2)
	{
		std::puts("Image not specified.");
		return -1;
	}

	initializer init;

	software_window win("Mess", {640, 480}, window::flags::borderless);
	surface image(argv[1]);

	// significantly improves subsequent blit performance, if the original image pixel format differs from window's pixel format
	image = convert(image, win.surface().format());

	for(int i = 0; i < 1000; ++i)
		blit(image, win.surface(), randomPoint(win.surface().size() - image.size()));

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
