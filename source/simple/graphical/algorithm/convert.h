#ifndef SIMPLE_GRAPHICAL_ALGORITHM_CONVERT_H
#define SIMPLE_GRAPHICAL_ALGORITHM_CONVERT_H

namespace simple::graphical
{
	class surface;
	class pixel_format;

	surface convert(const surface& source, const pixel_format& format);

} // namespace simple::graphical

#endif /* end of include guard */
