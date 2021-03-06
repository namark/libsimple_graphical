#ifndef SIMPLE_GRAPHICAL_SURFACE_H
#define SIMPLE_GRAPHICAL_SURFACE_H

#include <memory>

#include "simple/support/function_utils.hpp"
#include "simple/sdlcore/utils.hpp"

#include "common_def.h"
#include "pixel_format.h"
#include "pixels.h"
#include "color.h"

namespace simple::graphical
{
	using sdl_surface_wrapper = sdlcore::utils::object_wrapper<SDL_Surface>;

	class surface : public sdl_surface_wrapper
	{

		public:

		using byte = pixel_byte;

		explicit surface(const surface& other);
		surface(surface&& other) = default;
		surface& operator=(surface&& other) = default;

		// TODO: constructor with a pallete
		explicit surface(const char* filename);
		surface(int2 size, const pixel_format& format);
		surface(byte* pixels, int2 size, const pixel_format& format, int pitch = 0);
		surface(std::unique_ptr<byte[]> pixels, int2 size, const pixel_format& format, int pitch = 0);
		surface(std::unique_ptr<byte[], void(*)(byte[])> pixels, int2 size, const pixel_format& format, int pitch = 0);
#if SDL_VERSION_ATLEAST(2,0,5)
		surface(int2 size, pixel_format::type format);
		surface(byte* pixels, int2 size, pixel_format::type format, int pitch = 0);
		surface(std::unique_ptr<byte[]> pixels, int2 size, pixel_format::type format, int pitch = 0);
		surface(std::unique_ptr<byte[], void(*)(byte[])> pixels, int2 size, pixel_format::type format, int pitch = 0);
#endif

		const pixel_format& format() const;
		int2 size() const;

		blend_mode blend() const noexcept;
		void blend(blend_mode new_value) const noexcept;

		uint8_t alpha() const noexcept;
		void alpha(uint8_t new_value) const noexcept;

		rgb_pixel color() const noexcept;
		void color(rgb_pixel new_value) const noexcept;

		// TODO: generic interface wrappers, implicitly visiting, like simple::motion::variant
		pixel_writer_variant pixels() const noexcept;

		void save(const char* filename) const;

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

		friend bool fill(const surface&, class color);
		friend bool fill(const surface&, class color, const range2D&);
		friend bool blit(const surface&, range2D, const surface&, int2);
		friend bool blit(const surface&, range2D, const surface&, range2D);
		friend surface convert(const surface& source, const pixel_format& format);
		friend class renderer;
		friend class window;
	};

	pixel_writer_variant pixel_writer_from_format(pixel_byte* data, int2 size, int pitch, int bpp);

} // namespace simple::graphical

#endif /* end of include guard */
