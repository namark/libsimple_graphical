#ifndef SIMPLE_GRAPHICAL_SURFACE_H
#define SIMPLE_GRAPHICAL_SURFACE_H

#include <memory>
#include <SDL2/SDL.h>
#include "pixel_format.h"
#include "utils.hpp"

namespace simple::graphical
{
	using sdl_surface_wrapper = utils::sdl_object_wrapper<SDL_Surface>;

	class color;

	class surface : public sdl_surface_wrapper
	{
		public:
		const pixel_format& format() const;

		protected:

		surface(SDL_Surface* guts);

		friend bool fill(const surface& surf, color col);

		private:

		using sdl_surface_wrapper::sdl_surface_wrapper;

		class free_pixel_format : public pixel_format
		{
			public:
			free_pixel_format(SDL_PixelFormat* guts);
		};

		const free_pixel_format _format;

	};

} // namespace simple::graphical

#endif /* end of include guard */
