#include "software_window.h"

using namespace simple::graphical;

software_window::software_window(std::string title, int2 size, flags options, int2 position)
	: window(title, size, options, position),
	_surface(SDL_GetWindowSurface(guts().get()))
{}

const surface & software_window::surface() const noexcept
{
	return _surface;
}

void software_window::update_surface()
{
	_surface = free_surface(SDL_GetWindowSurface(guts().get()));
}

const surface & software_window::get_surface()
{
	update_surface();
	return surface();
}

void software_window::update() const
{
	sdlcore::utils::throw_error(SDL_UpdateWindowSurface(guts().get()));
}

bool software_window::refresh() const noexcept
{
	return !sdlcore::utils::check_error(SDL_UpdateWindowSurface(guts().get()));
}

software_window::free_surface::free_surface(SDL_Surface * guts)
	: surface(guts, support::nop)
{}
