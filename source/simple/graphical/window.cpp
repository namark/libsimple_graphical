#include <type_traits>

#include "simple/support/function_utils.hpp"
#include "simple/support/enum.hpp"
#include "window.h"

namespace simple::graphical
{

	point2D window::size() const noexcept
	{
		auto result = -point2D::one();
		SDL_GetWindowSize(guts().get(), &result.x(), &result.y());
		return result;
	}

	void window::size(point2D value) const noexcept
	{
		SDL_SetWindowSize(guts().get(), value.x(), value.y());
	}

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
