#include "simple/support/enum.hpp"
#include "renderer_window.h"

using namespace simple::graphical;

renderer_window::renderer_window
(
	std::string title,
	int2 size,
	flags window_options,
	renderer::flags renderer_options,
	quality_hint renderer_scale_quality,
	int2 position
)
	: window(title, size, window_options, position),
	_renderer
	(
		SDL_CreateRenderer
		(
			guts().get(),
			-1,
			support::to_integer(renderer_options)
		),
		renderer_scale_quality,
		SDL_DestroyRenderer
	)
{}

const renderer & renderer_window::renderer() const noexcept
{
	return _renderer;
}

void renderer_window::update() const noexcept
{
	_renderer.update();
}
