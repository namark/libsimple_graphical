#include <type_traits>

#include "simple/support/function_utils.hpp"
#include "window.h"

namespace simple::graphical
{

	using flags = window::flags;

	window::window(std::string title, point2D size, flags options, point2D position)
		: sdl_window_wrapper
		(
			SDL_CreateWindow
			(
				title.c_str(),
				position.x(), position.y(),
				size.x(), size.y(),
				static_cast<std::underlying_type_t<flags>>(options)
			),
			SDL_DestroyWindow
		),
		_surface(SDL_GetWindowSurface(guts().get()))
	{}

	const graphical::surface & window::surface() const
	{
		return _surface;
	}

	window::free_surface::free_surface(SDL_Surface * guts)
		: surface(guts, support::nop)
	{}

} // namespace simple::graphical
