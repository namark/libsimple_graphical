#ifndef SIMPLE_GRAPHICAL_ALGORITHM_BLIT_H
#define SIMPLE_GRAPHICAL_ALGORITHM_BLIT_H

#include "../common_def.h"
#include "../color.h"

namespace simple::graphical
{
	class surface;

	bool blit(const surface& source, const surface& destination, int2 position = int2::zero());
	bool blit(const surface& source, range2D src_range, const surface& destination, int2 position = int2::zero());
	bool blit(const surface& source, const surface& destination, range2D dest_range);
	bool blit(const surface& source, range2D src_range, const surface& destination, range2D dest_range);

} // namespace simple::graphical

#endif /* end of include guard */
