#ifndef SIMPLE_GRAPHICAL_RENDERER_WINDOW_H
#define SIMPLE_GRAPHICAL_RENDERER_WINDOW_H
#include "window.h"
#include "renderer.h"

namespace simple::graphical
{

	class renderer_window : public window
	{
		public:

		renderer_window
		(
			std::string title,
			point2D size,
			flags window_options = flags::shown,
			renderer::flags renderer_options = renderer::flags::automatic,
			point2D position = undefined_position
		);

		const graphical::renderer& renderer() const noexcept;

		void update() const noexcept;

		private:

		graphical::renderer _renderer;
	};

} // namespace simple::graphical

#endif /* end of include guard */
