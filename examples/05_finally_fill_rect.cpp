#include <cstdio>
#include <cerrno>

#include "simple/graphical/initializer.h"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm/blit.h"
#include "simple/graphical/algorithm/fill.h"

#include "common.h"

using namespace simple::graphical;
using namespace simple::support::literals;

constexpr auto half2D = float2::one() * 0.5f;

int main() try
{

	initializer init;

	software_window win("Look at rects", {640, 480}, window::flags::borderless);
	auto center = win.surface().size() / 2;

	auto dark = win.surface().format().color({80_u8,80_u8,80_u8});
	auto bright = win.surface().format().color({160_u8,160_u8,160_u8});

	fill(win.surface(), dark);
	// draws a checker pattern (see common.h)
	checker_up(win.surface(), int2::one() * 10, bright);

	win.update();
	SDL_Delay(1313);

	surface alpha_layer(win.surface().size(), pixel_format(pixel_format::type::rgba8888));
	auto light_blue = alpha_layer.format().color({0_u8, 177_u8, 177_u8, 177_u8});
	auto light_pink = alpha_layer.format().color({177_u8, 0_u8, 177_u8, 177_u8});

	// fill on surface does not blend, it overwrites
	fill(alpha_layer, light_blue, anchored_rect{ {{100, 300}, center}, half2D } );
	fill(alpha_layer, light_pink, anchored_rect{ {{300, 100}, center}, half2D } );
	blit(alpha_layer, win.surface()); // blit blends

	win.update();
	SDL_Delay(1313);

	surface alpha_layer2(alpha_layer.size(), alpha_layer.format());

	// so these will be blended with the previous two
	fill(alpha_layer2, light_blue, anchored_rect{ {{150, 150}, center + 75}, half2D } );
	fill(alpha_layer2, light_pink, anchored_rect{ {{150, 150}, center - 75}, half2D } );
	blit(alpha_layer2, win.surface()); // thanks to this blit

	win.update();
	SDL_Delay(1313 * 3);

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
