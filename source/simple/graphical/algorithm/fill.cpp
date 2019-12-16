#include "simple/sdlcore/utils.hpp"
#include "fill.hpp"
#include "../surface.h"
#include "../renderer.h"

namespace simple::graphical
{

	bool fill(const surface& surf, color col)
	{
		return !sdlcore::utils::check_error(
			SDL_FillRect(surf.guts().get(), NULL, static_cast<uint32_t>(col)));
	}

	bool fill(const surface& surf, color col, const range2D& area)
	{
		auto r = sdlcore::utils::to_rect<SDL_Rect>(area);
		return !sdlcore::utils::check_error(
			SDL_FillRect(surf.guts().get(), &r, static_cast<uint32_t>(col)));
	}

	bool fill(const renderer& rend)
	{
		return !sdlcore::utils::check_error(SDL_RenderClear(rend.guts().get()));
	}

	bool fill(const renderer& rend, const range2D& area)
	{
		auto r = sdlcore::utils::to_rect<SDL_Rect>(area);
		return !sdlcore::utils::check_error(SDL_RenderFillRect(rend.guts().get(), &r));
	}

	bool fill(const renderer& rend, const rgba_pixel& color)
	{
		rend.color(color);
		return fill(rend);
	}

	bool fill(const renderer& rend, const rgba_pixel& color, const range2D& area)
	{
		rend.color(color);
		return fill(rend, area);
	}

	void fill(pixel_writer_variant writer, color color)
	{
		std::visit([&color](auto&& writer)
		{
			fill_color(writer, color);
		}, writer);
	}


	// hmmm... is this guaranteed to work?
	[[maybe_unused]]
	void instantiate_fill(pixel_writer_variant var)
	{
		std::visit([](auto writer)
		{
			fill(writer, typename decltype(writer)::pixel_type{});
		}, var);
	}

	// template void fill<rgba_pixel, pixel_byte>(const pixel_writer<rgba_pixel, pixel_byte>&, rgba_pixel);
	// template void fill<rgb_pixel, pixel_byte>(const pixel_writer<rgb_pixel, pixel_byte>&, rgb_pixel);
	// template void fill<uint16_t, pixel_byte>(const pixel_writer<uint16_t, pixel_byte>&, uint16_t);
	// template void fill<pixel_byte, pixel_byte>(const pixel_writer<pixel_byte, pixel_byte>&, pixel_byte);

} // namespace simple::graphical
