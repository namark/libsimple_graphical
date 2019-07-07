#ifndef SIMPLE_GRAPHICAL_PALETTE_VIEW_H
#define SIMPLE_GRAPHICAL_PALETTE_VIEW_H

#include <SDL2/SDL.h>
#include "simple/support/range.hpp"
#include "color.h"

namespace simple::graphical
{

	class palette_view
	{
		using range = support::range<const rgba_pixel*>;

		public:
		size_t size() const noexcept;

		void set_color(size_t index, rgba_pixel) const noexcept;
		size_t set_colors(range colors, size_t start_index = 0) const noexcept;
		template<typename Range>
		size_t set_colors(const Range& colors, size_t start_index = 0) const noexcept
		{ return set_colors({colors.begin(), colors.end()}, start_index); }

		rgba_pixel operator[](size_t index) const noexcept;
		rgba_pixel at(size_t index) const;

		private:
		SDL_Palette* raw;
		explicit palette_view(SDL_Palette* raw) noexcept;
		friend class pixel_format;
	};

} // namespace simple::graphical

#endif /* end of include guard */
