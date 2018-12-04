#include "palette_view.h"

using namespace simple::graphical;

size_t palette_view::size() const noexcept
{
	return static_cast<size_t>(raw->ncolors);
}

size_t palette_view::set_colors(support::range<const rgba_pixel*> colors, size_t start_index) const noexcept
{
	auto src_size = colors.end() - colors.begin();
	auto to_write = std::min<size_t>(size() - start_index, src_size);
	for(size_t i = 0; i < to_write ; ++i)
	{
		auto& dst_color = raw->colors[i];
		auto& src_color = *(colors.begin() + i);
		dst_color.r = src_color.r();
		dst_color.g = src_color.g();
		dst_color.b = src_color.b();
		dst_color.a = src_color.a();
	}
	SDL_SetPaletteColors(raw, raw->colors, 0, 1); // let SDL know changes occurred
	return to_write;
}

palette_view::palette_view(SDL_Palette* raw) noexcept : raw(raw) {}
