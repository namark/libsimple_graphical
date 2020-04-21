#ifndef SIMPLE_GRAPHICAL_ALGORITHM_BLIT_H
#define SIMPLE_GRAPHICAL_ALGORITHM_BLIT_H

#include "../common_def.h"
#include "../color.h"
#include "../pixels.h"

namespace simple::graphical
{
	class surface;

	bool blit(const surface& source, const surface& destination, int2 position = int2::zero());
	bool blit(const surface& source, range2D src_range, const surface& destination, int2 position = int2::zero());
	bool blit(const surface& source, const surface& destination, range2D dest_range);
	bool blit(const surface& source, range2D src_range, const surface& destination, range2D dest_range);

	bool blit(const surface& source, const surface& destination, blend_mode);
	bool blit(const surface& source, range2D src_range, const surface& destination, blend_mode);
	bool blit(const surface& source, const surface& destination, int2 position, blend_mode);
	bool blit(const surface& source, range2D src_range, const surface& destination, int2 position, blend_mode);
	bool blit(const surface& source, const surface& destination, range2D dest_range, blend_mode);
	bool blit(const surface& source, range2D src_range, const surface& destination, range2D dest_range, blend_mode);

	// TODO: blend modes
	// TODO: handle different pixel types(conversion)
	// TODO: subpixel source/destination range
	template <typename Pixel, typename Raw, typename DestRaw>
	void blit
	(
			const pixel_reader<Pixel,Raw>&,
			const pixel_writer<Pixel,DestRaw>&
	);

} // namespace simple::graphical

#endif /* end of include guard */
