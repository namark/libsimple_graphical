#ifndef SIMPLE_GRAPHICAL_SOFTWARE_WINDOW_H
#define SIMPLE_GRAPHICAL_SOFTWARE_WINDOW_H
#include "window.h"
#include "surface.h"

namespace simple::graphical
{
	class software_window : public window
	{
		public:

		software_window
		(
			std::string title,
			int2 size,
			flags windowflags = flags::shown,
			int2 position = undefined_position
		);

		const graphical::surface & surface() const noexcept;

		void update() const;
		bool refresh() const noexcept;

		private:

		class free_surface : public graphical::surface
		{
			public:
			free_surface(SDL_Surface* guts);
		};

		const free_surface _surface;
	};

} // namespace simple::graphical

#endif /* end of include guard */
