#include <cassert>
#include "common_def.h"
#include "simple/support/enum.hpp"
#include "simple/sdlcore/utils.hpp"
#include "texture.h"

using namespace simple::graphical;
using simple::support::to_integer;

std::tuple<bool, texture::access, int2, pixel_format::type> texture::info() const noexcept
{
	uint32_t format;
	int acs;
	std::tuple<bool, access, int2, pixel_format::type> result;
	int2& size = std::get<int2>(result);
	std::get<bool>(result) = !sdlcore::utils::check_error(
			SDL_QueryTexture(_guts, &format, &acs, &size.x(), &size.y()));
	std::get<pixel_format::type>(result) = static_cast<pixel_format::type>(format);
	std::get<access>(result) = static_cast<access>(acs);
	return result;
};

texture::texture(SDL_Texture* guts) : _guts(guts)
{}

texture::texture(SDL_Renderer* rend, SDL_Surface* surface)
	: _guts(SDL_CreateTextureFromSurface(rend, surface))
{
	sdlcore::utils::throw_error(_guts);
}

texture::texture(SDL_Renderer* rend, pixel_format::type format, int2 size, access acs)
	: _guts(SDL_CreateTexture(rend, to_integer(format), to_integer(acs), size.x(), size.y()))
{
	sdlcore::utils::throw_error(_guts);
}

SDL_Texture* texture::guts() const
{
	assert(_guts && "simple::graphical::texture must not be null");
	return _guts;
}

basic_texture::basic_texture(const streaming_texture & tex) noexcept : texture(tex.guts())
{}
basic_texture::basic_texture(const render_texture & tex) noexcept : texture(tex.guts())
{}

streaming_texture::streaming_texture(const basic_texture & tex) : texture(tex.guts())
{
	const auto [success, acs, ignore, also_ignore]  = tex.info();
	((void)ignore, (void)also_ignore); // silence unused variable warning
	if(!success || acs != access::streaming)
		throw std::runtime_error("Invalid texture conversion");
}

render_texture::render_texture(const basic_texture & tex) : texture(tex.guts())
{
	const auto [success, acs, ignore, also_ignore]  = tex.info();
	((void)ignore, (void)also_ignore); // silence unused variable warning
	if(!success || acs != access::render_target)
		throw std::runtime_error("Invalid texture conversion");
}

texture_view::texture_view(basic_texture tex, range2D region, int2 pivot, texture::flip_direction flip)
	: texture(tex), region(region), pivot(pivot), flip(flip)
{}

texture_view::texture_view(basic_texture tex, int2 pivot, texture::flip_direction flip)
	: texture_view(tex, {int2::zero(), std::get<int2>(tex.info())}, pivot, flip)
{}
