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

} // namespace simple::graphical
