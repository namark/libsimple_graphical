#include <cstdio>
#include <cerrno>

#include "simple/graphical/initializer.h"
#include "simple/graphical/renderer_window.h"
#include "simple/graphical/renderer.h"
#include "simple/graphical/algorithm.hpp"

using namespace simple::graphical;
using namespace color_literals;

constexpr auto half2D = float2::one() * 0.5f;

int main() try
{
	initializer init;

	renderer_window win("Renderer", int2(600, 600), window::flags::borderless);
	auto center = win.renderer().target_size() / 2;

	fill(win.renderer(), 0x7700AA_rgb);
	win.update();
	SDL_Delay(1313);

	fill(win.renderer(), 0x330077_rgb, anchored_rect{ {{300, 300}, center - 75}, half2D });
	fill(win.renderer(), 0x330077_rgb, anchored_rect{ {{300, 300}, center + 75}, half2D });
	win.update();
	SDL_Delay(1313);

	win.renderer().blend(blend_mode::alpha);
	fill(win.renderer(), 0xFFFFFF77_rgba, anchored_rect{ {{100, 100}, center - 25}, half2D });
	fill(win.renderer(), 0xFFFFFF77_rgba, anchored_rect{ {{100, 100}, center + 25}, half2D });
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
