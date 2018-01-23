#include <type_traits>

#include "pixel_format.h"
#include "palette_view.h"

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

	uint32_t pixel_format::red_mask() const { return guts()->Rmask; }
	uint32_t pixel_format::green_mask() const { return guts()->Gmask; }
	uint32_t pixel_format::blue_mask() const { return guts()->Bmask; }
	uint32_t pixel_format::alpha_mask() const { return guts()->Amask; }

	graphical::color pixel_format::color(const rgb_pixel& values) const
	{
		return graphical::color(SDL_MapRGB(guts().get(), values.r(), values.g(), values.b()));
	}

	graphical::color pixel_format::color(const rgba_pixel& values) const
	{
		return graphical::color(SDL_MapRGBA(guts().get(), values.r(), values.g(), values.b(), values.a()));
	}

	std::optional<palette_view> pixel_format::palette() const
	{
		auto palette_ptr = guts()->palette;
		return palette_ptr ? std::make_optional(palette_view(palette_ptr)) : std::nullopt;
	}

} // namespace simple::graphical
