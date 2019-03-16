#include "simple/support/enum.hpp"

#include "window.h"
#include "surface.h"
#include "renderer.h"
#include "renderer_window.h"

using namespace simple;
using namespace simple::graphical;

static void set_scale_quality_hint(quality_hint hint)
{
	static const char* quality_hint_names[] = { "nearest", "linear", "best" };
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, quality_hint_names[static_cast<int>(hint)]);
}

renderer::renderer(const surface& target)
	: sdl_renderer_wrapper
	(
		SDL_CreateSoftwareRenderer(target.guts().get()),
		SDL_DestroyRenderer
	)
{}

renderer::renderer(SDL_Renderer* rend, quality_hint scale_quality, sdl_renderer_wrapper::Deleter deleter)
	: sdl_renderer_wrapper(rend, deleter), _default_scale_quality(scale_quality)
{
}

void renderer::update() const noexcept
{
	SDL_RenderPresent(guts().get());
}

rgba_pixel renderer::color() const noexcept
{
	rgba_pixel result;
	SDL_GetRenderDrawColor(guts().get(), &result.r(), &result.g(), &result.b(), &result.a());
	return result;
}

void renderer::color(const rgba_pixel& new_value) const noexcept
{
	SDL_SetRenderDrawColor(guts().get(), new_value.r(), new_value.g(), new_value.b(), new_value.a());
}

blend_mode renderer::blend() const noexcept
{
	SDL_BlendMode result;
	SDL_GetRenderDrawBlendMode(guts().get(), &result);
	return static_cast<blend_mode>(result);
}

void renderer::blend(blend_mode new_value) const noexcept
{
	SDL_SetRenderDrawBlendMode
	(
		guts().get(),
		static_cast<SDL_BlendMode>(support::to_integer(new_value))
	);
}

quality_hint renderer::default_scale_quality() const noexcept
{
	return _default_scale_quality;
}

int2 renderer::target_size() const noexcept
{
	auto result = -int2::one();
	SDL_GetRendererOutputSize(guts().get(), &result.x(), &result.y());
	return result;
}

basic_texture renderer::get_texture(const surface& surface) const
{
	return get_texture(surface, _default_scale_quality);
}

basic_texture renderer::get_texture(const surface& surface, quality_hint scale_quality) const
{
	set_scale_quality_hint(scale_quality);
	auto result = basic_texture(guts().get(), surface.guts().get());
	return result;
}

streaming_texture renderer::get_texture(int2 size, pixel_format::type format) const
{
	return get_texture(size, _default_scale_quality, format);
}

streaming_texture renderer::get_texture(int2 size, quality_hint scale_quality, pixel_format::type format) const
{
	set_scale_quality_hint(scale_quality);
	return streaming_texture(guts().get(), format, size, texture::access::streaming);
}

render_texture renderer::get_render_texture(int2 size, pixel_format::type format) const
{
	return get_render_texture(size, _default_scale_quality, format);
}

render_texture renderer::get_render_texture(int2 size, quality_hint scale_quality, pixel_format::type format) const
{
	set_scale_quality_hint(scale_quality);
	return render_texture(guts().get(), format, size, texture::access::render_target);
}

void renderer::destroy_texture(texture& tex) const
{
	SDL_DestroyTexture(tex.guts());
#ifndef NDEBUG
	tex._guts = nullptr;
#endif
}

blend_mode renderer::texture_blend(const texture& tex) const noexcept
{
	SDL_BlendMode result;
	SDL_GetTextureBlendMode(tex.guts(), &result);
	return static_cast<blend_mode>(result);
}

void renderer::texture_blend(const texture& tex, blend_mode new_value) const noexcept
{
	SDL_SetTextureBlendMode
	(
		tex.guts(),
		static_cast<SDL_BlendMode>(support::to_integer(new_value))
	);
}

rgb_pixel renderer::texture_color(const texture& tex) const noexcept
{
	rgb_pixel result;
	SDL_GetTextureColorMod(tex.guts(), &result.r(), &result.g(), &result.b());
	return result;
}

void renderer::texture_color(const texture& tex, const rgb_pixel& new_value) const noexcept
{
	SDL_SetTextureColorMod(tex.guts(), new_value.r(), new_value.g(), new_value.b());
}

uint8_t renderer::texture_alpha(const texture& tex) const noexcept
{
	uint8_t result;
	SDL_GetTextureAlphaMod(tex.guts(), &result);
	return result;
}

void renderer::texture_alpha(const texture& tex, uint8_t new_value) const noexcept
{
	SDL_SetTextureAlphaMod(tex.guts(), new_value);
}

void renderer::update(const texture& tex, pixel_reader_variant data_variant, int2 destination) const
{
	std::visit([&tex, &destination](auto&& data)
	{
		auto dr = sdlcore::utils::to_rect<SDL_Rect>(rect{data.size()});
		dr.x = destination.x();
		dr.y = destination.y();
		sdlcore::utils::throw_error(
			SDL_UpdateTexture(tex.guts(), &dr, data.row(), data.row(1) - data.row()) );
	}, data_variant);
}

void renderer::update(const streaming_texture& tex, std::function<void(pixel_writer_variant)> callback, range2D area) const
{
	const auto [ignore0, ignore1, ignore2, format]  = tex.info();
	((void)ignore0, (void)ignore1, (void)ignore2); // silence unused variable warning
	void* pixels;
	int pitch;
	auto rect = sdlcore::utils::to_rect<SDL_Rect>(area);
	sdlcore::utils::throw_error(
			SDL_LockTexture(tex.guts(), &rect, &pixels, &pitch) );
	callback(pixel_writer_from_format(
				reinterpret_cast<pixel_byte*>(pixels),
				area.upper() - area.lower(),
				pitch,
				SDL_BYTESPERPIXEL(support::to_integer(format)) ));
	SDL_UnlockTexture(tex.guts());
}

void renderer::update(const streaming_texture& tex, std::function<void(pixel_writer_variant)> callback) const
{
	const auto [ignore0, ignore1, size, format]  = tex.info();
	((void)ignore0, (void)ignore1); // silence unused variable warning

	void* pixels;
	int pitch;
	sdlcore::utils::throw_error(
			SDL_LockTexture(tex.guts(), NULL, &pixels, &pitch) );
	callback(pixel_writer_from_format(
				reinterpret_cast<pixel_byte*>(pixels),
				size,
				pitch,
				SDL_BYTESPERPIXEL(support::to_integer(format)) ));
	SDL_UnlockTexture(tex.guts());
}

std::optional<render_texture> renderer::target() const noexcept
{
	const auto tex = SDL_GetRenderTarget(guts().get());
	return NULL == tex ? std::make_optional(render_texture(tex)) : std::nullopt;
}

void renderer::target(render_texture tex) const
{
	sdlcore::utils::throw_error(SDL_SetRenderTarget(guts().get(), tex.guts()));
}

void renderer::target_default() const noexcept
{
	SDL_SetRenderTarget(guts().get(), NULL);
}

bool renderer::render(const texture& source) const
{
	return !sdlcore::utils::check_error(SDL_RenderCopy(guts().get(), source.guts(), NULL, NULL));
}

bool renderer::render(const texture& source, const range2D& destination) const
{
	auto dr = sdlcore::utils::to_rect<SDL_Rect>(destination);
	return !sdlcore::utils::check_error(SDL_RenderCopy(guts().get(), source.guts(), NULL, &dr));
}

static bool render_copy_ex(SDL_Renderer* rend, SDL_Texture* tex, range2D src, range2D dest, double angle, int2 center, texture::flip_direction flip)
{
	auto sr = sdlcore::utils::to_rect<SDL_Rect>(src);
	auto dr = sdlcore::utils::to_rect<SDL_Rect>(dest);
	auto c = SDL_Point{center.x(), center.y()};
	return !sdlcore::utils::check_error(SDL_RenderCopyEx(rend, tex, &sr, &dr,
				angle, &c, static_cast<SDL_RendererFlip>(support::to_integer(flip))));
}

bool renderer::render(const texture_view& source, int2 position) const
{
	return render_copy_ex(
			guts().get(), source.texture.guts(),
			source.region, source.region + position,
			0, source.pivot,
			source.flip);
}

bool renderer::render(const texture_view& source, const range2D& destination) const
{
	return render_copy_ex(
			guts().get(), source.texture.guts(),
			source.region, destination,
			0, source.pivot,
			source.flip);
}
bool renderer::render(const texture_view& source, int2 position, double angle) const
{
	return render_copy_ex(
			guts().get(), source.texture.guts(),
			source.region, source.region + position,
			angle, source.pivot,
			source.flip);
}
bool renderer::render(const texture_view& source, const range2D& destination, double angle) const
{
	return render_copy_ex(
			guts().get(), source.texture.guts(),
			source.region, destination,
			angle, source.pivot,
			source.flip);
}
