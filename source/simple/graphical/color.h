#ifndef SIMPLE_GRAPHICAL_COLOR_H
#define SIMPLE_GRAPHICAL_COLOR_H

#include <cstdint>

#include "color_vector.hpp"

namespace simple::graphical
{

	class pixel_format;

	class color
	{
		uint32_t raw;

		public:
		color() = default;
		constexpr explicit color(uint32_t raw) : raw(raw) {};
		color(const pixel_format& format, const rgb_pixel& values);
		color(const pixel_format& format, const rgba_pixel& values);
		explicit operator uint32_t() const;
	};

} // namespace simple::graphical

#endif /* end of include guard */
