#ifndef SIMPLE_GRAPHICAL_COMMON_DEF_H
#define SIMPLE_GRAPHICAL_COMMON_DEF_H

#include "simple/geom/vector.hpp"
#include "simple/geom/segment.hpp"

namespace simple::graphical
{

	using point2D = geom::vector<int, 2>;
	using vector2D = geom::vector<float, 2>;
	using rect = geom::vector_segment<int, 2>;
	using anchored_rect = geom::anchored_vector_segment<int, 2, float>;
	using range2D = rect::range;

} // namespace simple::graphical

#endif /* end of include guard */
