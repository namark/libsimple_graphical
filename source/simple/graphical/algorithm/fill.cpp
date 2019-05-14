#include "simple/sdlcore/utils.hpp"
#include "fill.h"
#include "../surface.h"
#include "../renderer.h"

namespace simple::graphical
{

	bool fill(const surface& surf, color col)
	{
		return !sdlcore::utils::check_error(
			SDL_FillRect(surf.guts().get(), NULL, static_cast<uint32_t>(col)));
	}

	bool fill(const surface& surf, color col, const range2D& area)
	{
		auto r = sdlcore::utils::to_rect<SDL_Rect>(area);
		return !sdlcore::utils::check_error(
			SDL_FillRect(surf.guts().get(), &r, static_cast<uint32_t>(col)));
	}

	bool fill(const renderer& rend)
	{
		return !sdlcore::utils::check_error(SDL_RenderClear(rend.guts().get()));
	}

	bool fill(const renderer& rend, const range2D& area)
	{
		auto r = sdlcore::utils::to_rect<SDL_Rect>(area);
		return !sdlcore::utils::check_error(SDL_RenderFillRect(rend.guts().get(), &r));
	}

	bool fill(const renderer& rend, const rgba_pixel& color)
	{
		rend.color(color);
		return fill(rend);
	}

	bool fill(const renderer& rend, const rgba_pixel& color, const range2D& area)
	{
		rend.color(color);
		return fill(rend, area);
	}

} // namespace simple::graphical
