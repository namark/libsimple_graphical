#include "simple/support/function_utils.hpp"
#include "surface.h"
#include "utils.hpp"

namespace simple::graphical
{

	surface::surface(const char* filename)
		: sdl_surface_wrapper(SDL_LoadBMP(filename), SDL_FreeSurface),
		_format(this->guts()->format)
	{}

	surface::surface(SDL_Surface* guts, Deleter deleter)
		: sdl_surface_wrapper(guts, deleter),
		_format(this->guts()->format)
	{}

	surface::free_pixel_format::free_pixel_format(SDL_PixelFormat* guts)
		: pixel_format(guts, support::nop)
	{}

	const pixel_format& surface::format() const
	{
		return _format;
	}

	point2D surface::size() const
	{
		return {guts()->w, guts()->h};
	}

} // namespace simple::graphical
