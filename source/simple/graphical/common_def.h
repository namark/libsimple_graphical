#ifndef SIMPLE_GRAPHICAL_COMMON_DEF_H
#define SIMPLE_GRAPHICAL_COMMON_DEF_H

#include <SDL2/SDL.h>

#include "simple/geom/vector.hpp"
#include "simple/geom/segment.hpp"

namespace simple::graphical
{

	using point2D = geom::vector<int, 2>;
	using vector2D = geom::vector<float, 2>;
	using rect = geom::vector_segment<int, 2>;
	using anchored_rect = geom::anchored_vector_segment<int, 2, float>;
	using range2D = rect::range;

	enum class blend_mode : std::underlying_type_t<SDL_BlendMode>
	{
		none = SDL_BLENDMODE_NONE,
		alpha = SDL_BLENDMODE_BLEND,
		add = SDL_BLENDMODE_ADD,
		multiply = SDL_BLENDMODE_MOD
	};

} // namespace simple::graphical

#endif /* end of include guard */
