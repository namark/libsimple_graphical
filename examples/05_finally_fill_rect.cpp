#include <cstdio>

#include "simple/graphical/window.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;
constexpr auto half2D = vector2D::one() * 0.5f;

void checker_up(const surface& board, point2D tile_size, color tile_color);

int main() try
{

	SDL_Init(SDL_INIT_EVERYTHING);
	{
		window win("Look at rects", {640, 480}, window::flags::borderless);
		auto center = win.surface().size() / 2;

		auto dark = win.surface().format().color(80,80,80);
		auto bright = win.surface().format().color(160,160,160);

		fill(win.surface(), dark);
		checker_up(win.surface(), point2D::one() * 10, bright);

		win.update();
		SDL_Delay(1313);

		surface alpha_layer(win.surface().size(), pixel_format(pixel_format::type::rgba8888));
		auto light_blue = alpha_layer.format().color(0, 177, 177, 177);
		auto light_pink = alpha_layer.format().color(177, 0, 177, 177);

		// fill on surface does not blend, it overwrites
		fill(alpha_layer, light_blue, anchored_rect{ {100, 300}, center, half2D } );
		fill(alpha_layer, light_pink, anchored_rect{ {300, 100}, center, half2D } );
		// blit blends
		blit(alpha_layer, win.surface());

		win.update();
		SDL_Delay(1313);

		surface alpha_layer2(alpha_layer.size(), alpha_layer.format());

		// so these will appear blended, with the previous two
		fill(alpha_layer2, light_blue, anchored_rect{ {150, 150}, center + 75, half2D } );
		fill(alpha_layer2, light_pink, anchored_rect{ {150, 150}, center - 75, half2D } );
		blit(alpha_layer2, win.surface()); // thanks to this blit

		win.update();
		SDL_Delay(1313 * 3);
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

void checker_up(const surface& board, point2D tile_size, color tile_color)
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
