#include "simple/support/function_utils.hpp"
#include "window.h"
#include "utils.hpp"

namespace simple::graphical
{

	using flags = window::flags;

	flags operator | (const flags& one, const flags& other)
	{
		using UType = std::underlying_type_t<flags>;
		return static_cast<flags>(static_cast<UType>(one) | static_cast<UType>(other));
	}

	flags operator & (const flags& one, const flags& other)
	{
		using UType = std::underlying_type_t<flags>;
		return static_cast<flags>(static_cast<UType>(one) & static_cast<UType>(other));
	}

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
		: surface(guts)
	{}

} // namespace simple::graphical
