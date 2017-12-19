#include "pixel_format.h"
#include "color.h"

namespace simple::graphical
{

	void pixel_format::free_pixel_format(const SDL_PixelFormat* one)
	{
		SDL_FreeFormat(const_cast<SDL_PixelFormat*>(one));
	}

	graphical::color pixel_format::color(uint8_t red, uint8_t green, uint8_t blue) const
	{
		return graphical::color(SDL_MapRGB(guts().get(), red, green, blue));
	}

	graphical::color pixel_format::color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) const
	{
		return graphical::color(SDL_MapRGBA(guts().get(), red, green, blue, alpha));
	}

} // namespace simple::graphical
