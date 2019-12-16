#ifndef SIMPLE_GRAPHICAL_ALGORITHM_FILL_H
#define SIMPLE_GRAPHICAL_ALGORITHM_FILL_H
#include "../common_def.h"
#include "../color.h"

#include "../pixels.h"

namespace simple::graphical
{
	class surface;
	class renderer;

	bool fill(const surface& surf, color);
	bool fill(const surface& surf, color, const range2D&);
	bool fill(const renderer& rend);
	bool fill(const renderer& rend, const rgba_pixel&);
	bool fill(const renderer& rend, const range2D&);
	bool fill(const renderer& rend, const rgba_pixel&, const range2D& area);

	template <typename Pixel, typename RawType>
	void fill(const pixel_writer<Pixel,RawType>&, Pixel);

	void fill(pixel_writer_variant, color);

} // namespace simple::graphical

#endif /* end of include guard */
