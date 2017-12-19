#ifndef SIMPLE_GRAPHICAL_COLOR_H
#define SIMPLE_GRAPHICAL_COLOR_H

#include <cstdint>
#include "pixel_format.h"

namespace simple::graphical
{

	class color
	{
		uint32_t _raw;

		public:
		color() = default;
		constexpr explicit color(uint32_t raw) : _raw(raw) {}
		color(const pixel_format& format, uint8_t r, uint8_t g, uint8_t b);
		color(const pixel_format& format, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		explicit operator uint32_t() const;
	};

} // namespace simple::graphical

#endif /* end of include guard */
