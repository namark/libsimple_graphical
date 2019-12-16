#include "fill.h"

namespace simple::graphical
{
	template <typename Pixel, typename RawType, typename Color>
	void fill_color(const pixel_writer<Pixel, RawType>& writer, Color color)
	{
		for(int2 i = int2::zero(), size = writer.size(); i.y() < size.y(); ++i.y())
			for(i.x() = 0; i.x() < size.x(); ++i.x())
				writer.set(color, i);
	}

	template <typename Pixel, typename RawType>
	void fill(const pixel_writer<Pixel, RawType>& writer, Pixel pixel)
	{
		fill_color(writer, pixel);
	}

} // namespace simple::graphical
