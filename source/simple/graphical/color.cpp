#include "color.h"
//

namespace simple::graphical
{

	color::color(const pixel_format& format, uint8_t red, uint8_t green, uint8_t blue)
		: color(format.color(red, green, blue))
	{}

	color::color(const pixel_format& format, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		: color(format.color(red, green, blue, alpha))
	{}

	color::operator uint32_t() const
	{
		return _raw;
	}

} // namespace simple::graphical
