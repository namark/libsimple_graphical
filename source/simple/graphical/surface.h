#ifndef SIMPLE_GRAPHICAL_SURFACE_H
#define SIMPLE_GRAPHICAL_SURFACE_H

#include <memory>
#include <SDL2/SDL.h>

#include "simple/support/function_utils.hpp"

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

		using byte = unsigned char;

		explicit surface(const surface& other);
		surface(surface&& other) = default;
		surface& operator=(surface&& other) = default;

		explicit surface(const char* filename);
		surface(point2D size, const pixel_format& format);
#if SDL_VERSION_ATLEAST(2,0,5)
		surface(point2D size, pixel_format::type format);
#endif
		surface(byte* pixels, point2D size, const pixel_format& format);
		surface(std::unique_ptr<byte[]> pixels, point2D size, const pixel_format& format);
		surface(std::unique_ptr<byte[], void(*)(byte[])> pixels, point2D size, const pixel_format& format);

		const pixel_format& format() const;
		point2D size() const;

		protected:

		explicit surface(SDL_Surface* guts, Deleter deleter = SDL_FreeSurface);

		private:

		using sdl_surface_wrapper::sdl_surface_wrapper;

		class free_pixel_format : public pixel_format
		{
			public:
			free_pixel_format(SDL_PixelFormat* guts);
		};

		free_pixel_format _format;
		std::unique_ptr<byte[], void(*)(byte*)> pixels_owner {nullptr, support::nop};

		friend bool fill(const surface&, color);
		friend bool fill(const surface&, color, range2D);
		friend bool blit(const surface&, range2D, const surface&, point2D);
		friend bool blit(const surface&, range2D, const surface&, range2D);
		friend surface convert(const surface& source, const pixel_format& format);
	};

} // namespace simple::graphical

#endif /* end of include guard */
