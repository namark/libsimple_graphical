#include <cstdio>
#include <cerrno>

#include <iostream>

#include "simple/graphical/initializer.h"

using namespace simple::graphical;

std::ostream& operator<<(std::ostream& os, display::mode mode)
{
	os << to_string(mode.format) << ' '
		<< mode.size.x() << 'x' << mode.size.y() << '@'
		<< mode.refresh_rate;
	return os;
}

int main() try
{
	initializer graphics;

	for(auto&& display : graphics.displays())
	{
		std::cout << "Didplay: " << display.name() << '\n';
		for(auto&& mode : display.modes())
		{
			std::cout << mode;
			if(mode == display.current_mode())
				std::cout << " <--";
			std::cout << '\n';
		}
		std::cout << '\n';
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
