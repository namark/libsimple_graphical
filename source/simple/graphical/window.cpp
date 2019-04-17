#include <type_traits>

#include "simple/support/function_utils.hpp"
#include "simple/support/enum.hpp"
#include "window.h"

namespace simple::graphical
{

	window::window(std::string title, int2 size, flags options, int2 position)
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

	int2 window::size() const noexcept
	{
		auto result = -int2::one();
		SDL_GetWindowSize(guts().get(), &result.x(), &result.y());
		return result;
	}

	void window::size(int2 value) const noexcept
	{
		SDL_SetWindowSize(guts().get(), value.x(), value.y());
	}

	int2 window::position() const noexcept
	{
		auto result = window::undefined_position;
		SDL_GetWindowPosition(guts().get(), &result.x(), &result.y());
		return result;
	}

	void window::position(int2 value) const noexcept
	{
		SDL_SetWindowPosition(guts().get(), value.x(), value.y());
	}

#if SDL_VERSION_ATLEAST(2,0,5)
	float window::opacity() const noexcept
	{
		float result = -1;
		SDL_GetWindowOpacity(guts().get(), &result);
		return result;
	}

	void window::opacity(float value) const noexcept
	{
		SDL_SetWindowOpacity(guts().get(), value);
	}
#endif

	void window::show() const noexcept { SDL_ShowWindow(guts().get()); }
	void window::hide() const noexcept { SDL_HideWindow(guts().get()); }

	void window::minimize() const noexcept
	{
		// SDL_PumpEvents();
		SDL_MinimizeWindow(guts().get());
	}

	void window::restore() const noexcept
	{
		// SDL_PumpEvents();
		SDL_RestoreWindow(guts().get());
	}

} // namespace simple::graphical
