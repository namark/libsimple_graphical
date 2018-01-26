#include "software_window.h"

using namespace simple::graphical;

software_window::software_window(std::string title, point2D size, flags options, point2D position)
	: window(title, size, options, position),
	_surface(SDL_GetWindowSurface(guts().get()))
{}

const surface & software_window::surface() const noexcept
{
	return _surface;
}

void software_window::update() const
{
	utils::throw_sdl_error(SDL_UpdateWindowSurface(guts().get()));
}

bool software_window::refresh() const noexcept
{
	return utils::check_sdl_error(SDL_UpdateWindowSurface(guts().get()));
}

software_window::free_surface::free_surface(SDL_Surface * guts)
	: surface(guts, support::nop)
{}
