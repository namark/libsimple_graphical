#include <SDL2/SDL.h>
#include "algorithm.h"
#include "utils.hpp"

namespace simple::graphical
{

	bool fill(const surface& surf, color col)
	{
		return utils::check_sdl_error(
			SDL_FillRect(surf.guts().get(), NULL, static_cast<uint32_t>(col)));
	}

	bool blit(const surface& source, const surface& destination, point2D position)
	{
		return blit(source, rect{ source.size() }, destination, position);
	}

	bool blit(const surface& source, range2D src_range, const surface& destination, point2D position)
	{
		auto sr = utils::to_sdl_rect<SDL_Rect>(src_range);
		auto dr = utils::to_sdl_rect<SDL_Rect>({position});
		return utils::check_sdl_error(
			SDL_BlitSurface(source.guts().get(), &sr, destination.guts().get(), &dr));
	}

	bool blit(const surface& source, const surface& destination, range2D dest_range)
	{
		return blit(source, rect{ source.size() }, destination, dest_range);
	}

	bool blit(const surface& source, range2D src_range, const surface& destination, range2D dest_range)
	{
		auto sr = utils::to_sdl_rect<SDL_Rect>(src_range);
		auto dr = utils::to_sdl_rect<SDL_Rect>(dest_range);
		return utils::check_sdl_error(
			SDL_BlitScaled(source.guts().get(), &sr, destination.guts().get(), &dr));
	}

	surface convert(const surface& source, const pixel_format& format)
	{
		return surface( SDL_ConvertSurface(source.guts().get(), format.guts().get(), 0) );
	}

} // namespace simple::graphical
