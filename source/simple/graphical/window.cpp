#include <type_traits>

#include "simple/support/function_utils.hpp"
#include "simple/support/enum.hpp"
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
				support::to_integer(options)
			),
			SDL_DestroyWindow
		)
	{}

} // namespace simple::graphical
