#include "color.h"
#include "pixel_format.h"

using namespace simple::graphical;

color::color(const pixel_format& format, const rgb_pixel& values)
: color(format.color(values))
{}

color::color(const pixel_format& format, const rgba_pixel& values)
: color(format.color(values))
{}

color::operator uint32_t() const
{
	return raw;
}

