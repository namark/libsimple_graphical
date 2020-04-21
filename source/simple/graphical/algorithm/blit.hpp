#ifndef SIMPLE_GRAPHICAL_ALGORITHM_BLIT_HPP
#define SIMPLE_GRAPHICAL_ALGORITHM_BLIT_HPP
#include "blit.h"

#include "simple/geom/algorithm.hpp"

namespace simple::graphical
{


	template <typename Pixel, typename Raw, typename DestRaw>
	void blit
	(
		const pixel_reader<Pixel,Raw>& source,
		const pixel_writer<Pixel,DestRaw>& destination
	)
	{
		// TODO: optimize for special cases

		auto source_size = float2(source.size());
		auto dest_size = destination.size();
		auto scale = source_size/float2(dest_size);
		geom::loop(int2::zero(), dest_size, int2::one(), [&](auto&& i)
		{
			destination.set(source.get(float2(i)*scale), i);
		});

	}

} // namespace simple::graphical

#endif /* end of include guard */
