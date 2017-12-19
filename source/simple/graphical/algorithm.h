#ifndef SIMPLE_GRAPHICAL_ALGORITHM_H
#define SIMPLE_GRAPHICAL_ALGORITHM_H

#include "common_def.h"
#include "surface.h"
#include "color.h"

namespace simple::graphical
{

	bool fill(const surface& surf, color col);
	bool blit(const surface& source, const surface& destination, point2D position = point2D::zero());

} // namespace simple::graphical

#endif /* end of include guard */
