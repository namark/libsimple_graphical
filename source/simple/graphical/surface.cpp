#include "simple/support/function_utils.hpp"
#include "surface.h"
#include "utils.hpp"

namespace simple::graphical
{

	surface::free_pixel_format::free_pixel_format(SDL_PixelFormat* guts)
		: pixel_format(guts, support::nop)
	{}

	surface::surface(SDL_Surface* guts)
		: sdl_surface_wrapper(guts, support::nop),
		_format(this->guts()->format)
	{}

	const pixel_format& surface::format() const
	{
		return _format;
	}

} // namespace simple::graphical
