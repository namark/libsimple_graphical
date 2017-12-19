#ifndef SIMPLE_GRAPHICAL_SURFACE_H
#define SIMPLE_GRAPHICAL_SURFACE_H

#include <memory>
#include <SDL2/SDL.h>

#include "common_def.h"
#include "pixel_format.h"
#include "utils.hpp"

namespace simple::graphical
{
	using sdl_surface_wrapper = utils::sdl_object_wrapper<SDL_Surface>;

	class color;

	class surface : public sdl_surface_wrapper
	{
		public:
		surface(const char* filename);

		const pixel_format& format() const;
		point2D size() const;

		protected:

		surface(SDL_Surface* guts);

		private:

		using sdl_surface_wrapper::sdl_surface_wrapper;

		class free_pixel_format : public pixel_format
		{
			public:
			free_pixel_format(SDL_PixelFormat* guts);
		};

		const free_pixel_format _format;

		friend bool fill(const surface& surf, color col);
		friend bool blit(const surface& source, const surface& destination, point2D position);
	};

} // namespace simple::graphical

#endif /* end of include guard */
