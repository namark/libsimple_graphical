#include "simple/sdlcore/utils.hpp"
#include "convert.h"
#include "../surface.h"

// TODO: implement conversion that SDL doesn't support

namespace simple::graphical
{

	surface convert(const surface& source, const pixel_format& format)
	{
		return surface( SDL_ConvertSurface(source.guts().get(), format.guts().get(), 0) );
	}

} // namespace simple::graphical
