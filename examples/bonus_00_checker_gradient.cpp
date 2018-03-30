#include <cstdio>

#include "simple/graphical/algorithm.h"
#include "simple/support/misc.hpp"

#include "common.h"

using namespace simple::graphical;
using namespace color_literals;
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
		const auto start = rgb_vector::red() + rgb_vector::green();
		const auto end = rgb_vector::blue();

		checker_up(board, tile_size, [start, end](const surface& s, rect r)
		{
			auto end_ratio = rgb_vector(r.position.mix<0,1,1>(1)) / rgb_vector(s.size().mix<0,1,1>());
			auto start_ratio = rgb_vector::white() - end_ratio;
			class color c(s.format(), rgb_pixel(start * start_ratio + end * end_ratio ));
			fill(s, c, r);

			r.position -= r.size * point2D::i(); // a bit of a hack
			class color c2(s.format(), rgb_pixel(start * end_ratio + end * start_ratio ));
			fill(s, c2, r);
		});
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
