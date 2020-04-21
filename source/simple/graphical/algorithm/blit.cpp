#include "simple/sdlcore/utils.hpp"
#include "blit.hpp"
#include "../surface.h"

// TODO: implement blit combination that SDL doesn't support
// scaled blit between two indexed surfaces with same palette
// scaled blit from indexed surface to rgb surface
// ...

namespace simple::graphical
{
	bool blit(const surface& source, const surface& destination, int2 position)
	{
		return blit(source, rect{ source.size() }, destination, position);
	}

	bool blit(const surface& source, range2D src_range, const surface& destination, int2 position)
	{
		auto sr = sdlcore::utils::to_rect<SDL_Rect>(src_range);
		auto dr = sdlcore::utils::to_rect<SDL_Rect>({position});
		return !sdlcore::utils::check_error(
			SDL_BlitSurface(source.guts().get(), &sr, destination.guts().get(), &dr));
	}

	bool blit(const surface& source, const surface& destination, range2D dest_range)
	{
		return blit(source, rect{ source.size() }, destination, dest_range);
	}

	bool blit(const surface& source, range2D src_range, const surface& destination, range2D dest_range)
	{
		auto sr = sdlcore::utils::to_rect<SDL_Rect>(src_range);
		auto dr = sdlcore::utils::to_rect<SDL_Rect>(dest_range);
		return !sdlcore::utils::check_error(
			SDL_BlitScaled(source.guts().get(), &sr, destination.guts().get(), &dr));
	}

	class scoped_blend
	{
		const surface& surf;
		const blend_mode original_blend;
		public:
		scoped_blend(const surface& surf, blend_mode new_blend) noexcept
			: surf(surf), original_blend(surf.blend())
		{
			surf.blend(new_blend);
		}
		~scoped_blend() noexcept
		{
			surf.blend(original_blend);
		}
	};

	bool blit(const surface& source, const surface& destination, blend_mode blend)
	{
		scoped_blend temp{source, blend};
		return blit(source, destination);
	}

	bool blit(const surface& source, range2D src_range, const surface& destination, blend_mode blend)
	{
		scoped_blend temp{source, blend};
		return blit(source, src_range, destination);
	}

	bool blit(const surface& source, const surface& destination, int2 position, blend_mode blend)
	{
		scoped_blend temp{source, blend};
		return blit(source, destination, position);
	}

	bool blit(const surface& source, range2D src_range, const surface& destination, int2 position, blend_mode blend)
	{
		scoped_blend temp{source, blend};
		return blit(source, src_range, destination, position);
	}

	bool blit(const surface& source, const surface& destination, range2D dest_range, blend_mode blend)
	{
		scoped_blend temp{source, blend};
		return blit(source, destination, dest_range);
	}

	bool blit(const surface& source, range2D src_range, const surface& destination, range2D dest_range, blend_mode blend)
	{
		scoped_blend temp{source, blend};
		return blit(source, src_range, destination, dest_range);
	}


	template void blit<rgba_pixel, pixel_byte, pixel_byte>
	(
		const pixel_reader<rgba_pixel, pixel_byte>&,
		const pixel_writer<rgba_pixel, pixel_byte>&
	);

	template void blit<rgb_pixel, pixel_byte, pixel_byte>
	(
		const pixel_reader<rgb_pixel, pixel_byte>&,
		const pixel_writer<rgb_pixel, pixel_byte>&
	);

	template void blit<uint16_t, pixel_byte, pixel_byte>
	(
		const pixel_reader<uint16_t, pixel_byte>&,
		const pixel_writer<uint16_t, pixel_byte>&
	);

	template void blit<pixel_byte, pixel_byte, pixel_byte>
	(
		const pixel_reader<pixel_byte, pixel_byte>&,
		const pixel_writer<pixel_byte, pixel_byte>&
	);

} // namespace simple::graphical
