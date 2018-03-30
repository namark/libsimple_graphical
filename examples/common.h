#ifndef SIMPLE_GRAPHICAL_EXAMPLES_COMMON_H
#define SIMPLE_GRAPHICAL_EXAMPLES_COMMON_H

#include "simple/graphical/surface.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;

template <typename Filler>
inline void checker_up(const surface& board, point2D tile_size, Filler filler)
{
	using simple::support::intersects;

	rect board_rect{board.size()};
	rect tile = rect{tile_size};
	while( intersects<point2D>(board_rect, tile) )
	{
		filler(board, tile);

		for(size_t i = 0; i < point2D::dimensions; ++i)
		{
			auto step = tile.size * 2 * point2D::unit(i);
			auto t = tile;
			while( t.position += step, intersects<point2D>(board_rect, t) )
				filler(board, t);
		}

		tile.position += tile.size;
	}

}

inline void checker_up(const surface& board, point2D tile_size, color tile_color)
{
	checker_up(board, tile_size, [tile_color](auto&& surf, auto&& rect){ fill(surf, tile_color, rect); });
}

#endif /* end of include guard */
