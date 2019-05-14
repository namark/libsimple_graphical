#ifndef SIMPLE_GRAPHICAL_ALGORITHM_FILL_H
#define SIMPLE_GRAPHICAL_ALGORITHM_FILL_H
#include "../common_def.h"
#include "../color.h"

namespace simple::graphical
{
	class surface;
	class renderer;

	bool fill(const surface& surf, color col);
	bool fill(const surface& surf, color col, const range2D& area);
	bool fill(const renderer& rend);
	bool fill(const renderer& rend, const rgba_pixel& color);
	bool fill(const renderer& rend, const range2D& area);
	bool fill(const renderer& rend, const rgba_pixel& color, const range2D& area);

} // namespace simple::graphical

#endif /* end of include guard */
