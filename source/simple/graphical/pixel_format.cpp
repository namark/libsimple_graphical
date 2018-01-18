#include <type_traits>

#include "pixel_format.h"
#include "color.h"

namespace simple::graphical
{

	void pixel_format::free_pixel_format(const SDL_PixelFormat* one)
	{
		SDL_FreeFormat(const_cast<SDL_PixelFormat*>(one));
	}

	pixel_format::pixel_format(type format_type)
		: sdl_pixel_format_wrapper
		(
			SDL_AllocFormat(static_cast<std::underlying_type_t<type>>(format_type)),
			free_pixel_format
		)
	{}

	int pixel_format::bits() const { return guts()->BitsPerPixel; }
	int pixel_format::bytes() const { return guts()->BytesPerPixel; }

	std::uint32_t pixel_format::red_mask() const { return guts()->Rmask; }
	std::uint32_t pixel_format::green_mask() const { return guts()->Gmask; }
	std::uint32_t pixel_format::blue_mask() const { return guts()->Bmask; }
	std::uint32_t pixel_format::alpha_mask() const { return guts()->Amask; }

	graphical::color pixel_format::color(uint8_t red, uint8_t green, uint8_t blue) const
	{
		return graphical::color(SDL_MapRGB(guts().get(), red, green, blue));
	}

	graphical::color pixel_format::color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) const
	{
		return graphical::color(SDL_MapRGBA(guts().get(), red, green, blue, alpha));
	}

	graphical::color pixel_format::color(uint8_t gray) const
	{ return color(gray, gray, gray); }

	graphical::color pixel_format::color(uint8_t gray, uint8_t alpha) const
	{ return color(gray, gray, gray, alpha); }

} // namespace simple::graphical
