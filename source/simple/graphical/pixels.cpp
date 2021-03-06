#include "pixels.hpp"

namespace simple::graphical
{
	namespace pixel_view_details
	{
		template class impl<tag::writer, pixel_byte, pixel_byte>;
		template class impl<tag::reader, pixel_byte, pixel_byte>;
		template class impl<tag::writer, uint16_t, pixel_byte>;
		template class impl<tag::reader, uint16_t, pixel_byte>;
		template class impl<tag::writer, rgb_pixel, pixel_byte>;
		template class impl<tag::reader, rgb_pixel, pixel_byte>;
		template class impl<tag::writer, rgba_pixel, pixel_byte>;
		template class impl<tag::reader, rgba_pixel, pixel_byte>;
	} // namespace pixel_view_details

	template class pixel_writer<pixel_byte>;
	template class pixel_reader<pixel_byte>;
	template class pixel_writer<uint16_t, pixel_byte>;
	template class pixel_reader<uint16_t, pixel_byte>;
	template class pixel_writer<rgb_pixel, pixel_byte>;
	template class pixel_reader<rgb_pixel, pixel_byte>;
	template class pixel_writer<rgba_pixel, pixel_byte>;
	template class pixel_reader<rgba_pixel, pixel_byte>;

// TODO: as if the stuff above wasn't enough, this can explode quadratically
// maybe just force user to instantiate themselves if they are working with obscure types that happen to fit here
// also are all these combinations useful??
#define SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(Pixel,Vector,Param) \
	template void pixel_writer<Pixel,pixel_byte>::set<Vector>(const Param &, float2) const; \
	template void pixel_writer<Pixel,pixel_byte>::set<Vector>(const Param &, float2, range2D) const;
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgb_pixel, rgb_vector, rgb_vector)
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgb_pixel, rgba_vector, rgba_vector)
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgb_pixel, rgb_vector, rgb_pixel)
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgb_pixel, rgba_vector, rgb_pixel)
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgba_pixel, rgb_vector, rgb_vector)
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgba_pixel, rgba_vector, rgba_vector)
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgba_pixel, rgb_vector, rgba_pixel)
SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(rgba_pixel, rgba_vector, rgba_pixel)
// SIMPLE_GRAPHICAL_PIXEL_WRITER_SET(uint16_t, float, uint16_t) // TODO
#undef SIMPLE_GRAPHICAL_PIXEL_WRITER_SET
using namespace pixel_view_details;
#define SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(Tag,Pixel,Vector) \
	template Pixel impl<Tag,Pixel,pixel_byte>::get<Vector>(float2) const; \
	template Pixel impl<Tag,Pixel,pixel_byte>::get<Vector>(float2, range2D) const;
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::reader, rgb_pixel, rgb_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::reader, rgb_pixel, rgba_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::reader, rgba_pixel, rgb_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::reader, rgba_pixel, rgba_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::reader, uint16_t, float)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::reader, pixel_byte, float)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::writer, rgb_pixel, rgb_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::writer, rgb_pixel, rgba_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::writer, rgba_pixel, rgb_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::writer, rgba_pixel, rgba_vector)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::writer, uint16_t, float)
SIMPLE_GRAPHICAL_PIXEL_VIEW_GET(tag::writer, pixel_byte, float)
#undef SIMPLE_GRAPHICAL_PIXEL_VIEW_GET

} // namespace simple::graphical
