#include "palette_view.h"
#include "cassert"

using namespace simple::graphical;

size_t palette_view::size() const noexcept
{
	return static_cast<size_t>(raw->ncolors);
}

void palette_view::set_color(size_t index, rgba_pixel color) const noexcept
{
	auto& dst_color = raw->colors[index];
	dst_color.r = color.r();
	dst_color.g = color.g();
	dst_color.b = color.b();
	dst_color.a = color.a();
	SDL_SetPaletteColors(raw, raw->colors, 0, 1); // let SDL know changes occurred
}

size_t palette_view::set_colors(support::range<const rgba_pixel*> colors, size_t start_index) const noexcept
{
	auto src_size = colors.end() - colors.begin();
	auto to_write = std::min<size_t>(size() - start_index, src_size);
	for(size_t i = 0; i < to_write ; ++i) // could just copy bytes?
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

rgba_pixel palette_view::operator[](size_t index) const noexcept
{
	assert(index < size_t(raw->ncolors));
	SDL_Color color = raw->colors[index];
	return {color.r, color.g, color.b, color.a};
}

rgba_pixel palette_view::at(size_t index) const
{
	if(index >= size_t(raw->ncolors))
		throw std::out_of_range("simple::graphical::palette_view::at: index out of range.");
	return (*this)[index];
}

palette_view::palette_view(SDL_Palette* raw) noexcept : raw(raw) {}
