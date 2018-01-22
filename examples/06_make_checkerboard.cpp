#include <cstdio>

#include "simple/graphical/algorithm.h"
#include "simple/support/misc.hpp"

#include "common.h"

using namespace simple::graphical;
using namespace simple::support::literals;
using simple::support::ston;

int main(int argc, char const* argv[]) try
{

	switch(argc)
	{
		case 0: std::puts("Command not specified??");
		case 1: std::puts("Tile width not specified.");
		case 2: std::puts("Tile height not specified.");
		case 3: std::puts("Columns not specified");
		case 4: std::puts("Rows not specified");
		case 5: std::puts("Destination not specified");
			return -1;
	}

	point2D tile_size { ston<int>(argv[1]), ston<int>(argv[2]) };
	point2D board_dimensions { ston<int>(argv[3]), ston<int>(argv[4]) };

	SDL_Init(SDL_INIT_EVERYTHING);
	{
		surface board (tile_size * board_dimensions, pixel_format(pixel_format::type::rgb24));
		fill(board, board.format().color(rgb_pixel::gray(255_u8)));
		checker_up(board, tile_size, board.format().color(rgb_pixel::gray(0_u8)));
		board.save(argv[5]);
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
