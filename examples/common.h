#ifndef SIMPLE_GRAPHICAL_EXAMPLES_COMMON_H
#define SIMPLE_GRAPHICAL_EXAMPLES_COMMON_H

#include <string>

#include "simple/graphical/surface.h"
#include "simple/graphical/algorithm/fill.h"

#if ENABLE_OPENGL_EXAMPLES
#include <GL/glew.h>
#include <GL/gl.h>
#endif

using namespace simple::graphical;

template <typename Filler>
inline void checker_up(const surface& board, int2 tile_size, Filler filler)
{
	using simple::support::intersects;

	rect board_rect{board.size()};
	rect tile = rect{tile_size};
	while( intersects<int2>(board_rect, tile) )
	{
		filler(board, tile);

		for(size_t i = 0; i < int2::dimensions; ++i)
		{
			auto step = tile.size * 2 * int2::unit(i);
			auto t = tile;
			while( t.position += step, intersects<int2>(board_rect, t) )
				filler(board, t);
		}

		tile.position += tile.size;
	}

}

inline void checker_up(const surface& board, int2 tile_size, color tile_color)
{
	checker_up(board, tile_size, [tile_color](auto&& surf, auto&& rect){ fill(surf, tile_color, rect); });
}

std::string to_string(int value, int padding)
{
	int digits = 1;
	int copy = value;
	while(copy / 10)
	{
		copy /= 10;
		++digits;
	}

	auto str = std::to_string(value);
	while(digits < padding)
	{
		str = '0' + str;
		++digits;
	}

	return str;
}

#if ENABLE_OPENGL_EXAMPLES

void gl_read_pixels(surface& to)
{
	auto pixels = std::get<pixel_writer<rgb_pixel, surface::byte>>(to.pixels());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0,0, to.size().x(), to.size().y(), GL_RGB, GL_UNSIGNED_BYTE, pixels.row());
}

#endif


#endif /* end of include guard */
