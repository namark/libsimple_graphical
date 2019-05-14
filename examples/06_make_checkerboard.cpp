#include <cstdio>
#include <cerrno>
#include <array>

#include "simple/graphical/initializer.h"
#include "simple/graphical/algorithm.hpp"
#include "simple/support/misc.hpp"

#include "common.h"

using namespace simple::graphical;
using namespace simple::support::literals;
using simple::support::ston;

enum ColorIndex
{
	Black,
	White
};

constexpr auto colors = std::array
{
	rgba_pixel::white(0_u8),
	rgba_pixel::white(255_u8)
};

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

	int2 tile_size { ston<int>(argv[1]), ston<int>(argv[2]) };
	int2 board_dimensions { ston<int>(argv[3]), ston<int>(argv[4]) };

	initializer init;

	surface board (tile_size * board_dimensions, pixel_format(pixel_format::type::index8));
	auto palette = board.format().palette();
	if(palette)
		palette->set_colors(colors);
	fill(board, color(White));
	checker_up(board, tile_size, color(Black));
	board.save(argv[5]);

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
