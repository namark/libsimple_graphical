#include "simple/sdlcore/utils.hpp"
#include "blit.h"
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
} // namespace simple::graphical
