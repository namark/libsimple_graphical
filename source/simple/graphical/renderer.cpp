#include "simple/support/enum.hpp"

#include "window.h"
#include "surface.h"
#include "renderer.h"
#include "renderer_window.h"

using namespace simple::graphical;

renderer::renderer(const surface& target)
	: sdl_renderer_wrapper
	(
		SDL_CreateSoftwareRenderer(target.guts().get()),
		SDL_DestroyRenderer
	)
{}

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

point2D renderer::target_size() const noexcept
{
	auto result = -point2D::one();
	SDL_GetRendererOutputSize(guts().get(), &result.x(), &result.y());
	return result;
}
