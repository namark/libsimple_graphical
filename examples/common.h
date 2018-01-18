#ifndef SIMPLE_GRAPHICAL_EXAMPLES_COMMON_H
#define SIMPLE_GRAPHICAL_EXAMPLES_COMMON_H

#include "simple/graphical/surface.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;

inline void checker_up(const surface& board, point2D tile_size, color tile_color)
{
	using simple::support::intersects;

	rect board_rect{board.size()};
	rect tile = rect{tile_size};
	while( intersects<point2D>(board_rect, tile) )
	{
		fill(board, tile_color, tile);

		for(size_t i = 0; i < point2D::dimensions; ++i)
		{
			auto step = tile.size * 2 * point2D::unit(i);
			auto t = tile;
			while( t.position += step, intersects<point2D>(board_rect, t) )
				fill(board, tile_color, t);
		}

		tile.position += tile.size;
	}

}

#endif /* end of include guard */
