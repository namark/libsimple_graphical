#include "simple/support/enum.hpp"
#include "renderer_window.h"

using namespace simple::graphical;

renderer_window::renderer_window
(
	std::string title,
	point2D size,
	flags window_options,
	renderer::flags renderer_options,
	point2D position
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
