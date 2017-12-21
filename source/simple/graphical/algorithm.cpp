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
		SDL_Rect dest_rect;
		dest_rect.x = position.x();
		dest_rect.y = position.y();
		return utils::check_sdl_error(
			SDL_BlitSurface(source.guts().get(), NULL, destination.guts().get(), &dest_rect));
	}

	surface convert(const surface& source, const pixel_format& format)
	{
		return surface( SDL_ConvertSurface(source.guts().get(), format.guts().get(), 0) );
	}

} // namespace simple::graphical
