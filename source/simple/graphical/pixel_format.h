#ifndef SIMPLE_GRAPHICAL_PIXEL_FORMAT_H
#define SIMPLE_GRAPHICAL_PIXEL_FORMAT_H

#include <optional>
#include "simple/sdlcore/utils.hpp"
#include "color.h"
#include "palette_view.h"

namespace simple::graphical
{

	using sdl_pixel_format_wrapper = sdlcore::utils::object_wrapper<const SDL_PixelFormat>;

	class surface;

	class pixel_format : public sdl_pixel_format_wrapper
	{
		public:

		enum class type : uint32_t
		{
			unknown = SDL_PIXELFORMAT_UNKNOWN,
			index1lsb = SDL_PIXELFORMAT_INDEX1LSB,
			index1msb = SDL_PIXELFORMAT_INDEX1MSB,
			index4lsb = SDL_PIXELFORMAT_INDEX4LSB,
			index4msb = SDL_PIXELFORMAT_INDEX4MSB,
			index8 = SDL_PIXELFORMAT_INDEX8,
			rgb332 = SDL_PIXELFORMAT_RGB332,
			rgb444 = SDL_PIXELFORMAT_RGB444,
			rgb555 = SDL_PIXELFORMAT_RGB555,
			bgr555 = SDL_PIXELFORMAT_BGR555,
			argb4444 = SDL_PIXELFORMAT_ARGB4444,
			rgba4444 = SDL_PIXELFORMAT_RGBA4444,
			abgr4444 = SDL_PIXELFORMAT_ABGR4444,
			bgra4444 = SDL_PIXELFORMAT_BGRA4444,
			argb1555 = SDL_PIXELFORMAT_ARGB1555,
			rgba5551 = SDL_PIXELFORMAT_RGBA5551,
			abgr1555 = SDL_PIXELFORMAT_ABGR1555,
			bgra5551 = SDL_PIXELFORMAT_BGRA5551,
			rgb565 = SDL_PIXELFORMAT_RGB565,
			bgr565 = SDL_PIXELFORMAT_BGR565,
			rgb24 = SDL_PIXELFORMAT_RGB24,
			bgr24 = SDL_PIXELFORMAT_BGR24,
			xrgb8888 = SDL_PIXELFORMAT_RGB888,
			rgbx8888 = SDL_PIXELFORMAT_RGBX8888,
			xbgr8888 = SDL_PIXELFORMAT_BGR888,
			bgrx8888 = SDL_PIXELFORMAT_BGRX8888,
			argb8888 = SDL_PIXELFORMAT_ARGB8888,
			rgba8888 = SDL_PIXELFORMAT_RGBA8888,
			abgr8888 = SDL_PIXELFORMAT_ABGR8888,
			bgra8888 = SDL_PIXELFORMAT_BGRA8888,
			argb2101010 = SDL_PIXELFORMAT_ARGB2101010,
#if SDL_VERSION_ATLEAST(2,0,5)
			rgba32 = SDL_PIXELFORMAT_RGBA32,
			argb32 = SDL_PIXELFORMAT_ARGB32,
			bgra32 = SDL_PIXELFORMAT_BGRA32,
			abgr32 = SDL_PIXELFORMAT_ABGR32,
#else
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rgba32 = SDL_PIXELFORMAT_RGBA8888,
			argb32 = SDL_PIXELFORMAT_ARGB8888,
			bgra32 = SDL_PIXELFORMAT_BGRA8888,
			abgr32 = SDL_PIXELFORMAT_ABGR8888,
	#else
			rgba32 = SDL_PIXELFORMAT_ABGR8888,
			argb32 = SDL_PIXELFORMAT_BGRA8888,
			bgra32 = SDL_PIXELFORMAT_ARGB8888,
			abgr32 = SDL_PIXELFORMAT_RGBA8888,
	#endif

#endif
			yv12 = SDL_PIXELFORMAT_YV12, // planar mode: Y + V + U (3 planes)
			iyuv = SDL_PIXELFORMAT_IYUV, // planar mode: Y + U + V (3 planes)
			yuy2 = SDL_PIXELFORMAT_YUY2, // packed mode: Y0+U0+Y1+V0 (1 plane)
			uyvy = SDL_PIXELFORMAT_UYVY, // packed mode: U0+Y0+V0+Y1 (1 plane)
			yvyu = SDL_PIXELFORMAT_YVYU, // packed mode: Y0+V0+Y1+U0 (1 plane)
#if SDL_VERSION_ATLEAST(2,0,4)
			nv12 = SDL_PIXELFORMAT_NV12, // planar mode: Y + U/V interleaved (2 planes)
			nv21  = SDL_PIXELFORMAT_NV21 // planar mode: Y + V/U interleaved (2 planes)
#endif
		};

		using sdl_pixel_format_wrapper::sdl_pixel_format_wrapper;

		explicit pixel_format(type t);

		int bits() const noexcept;
		int bytes() const noexcept;
		uint32_t red_mask() const noexcept;
		uint32_t green_mask() const noexcept;
		uint32_t blue_mask() const noexcept;
		uint32_t alpha_mask() const noexcept;

		graphical::color color(const rgb_pixel&) const noexcept;
		graphical::color color(const rgba_pixel&) const noexcept;

		rgb_pixel rgb(const graphical::color&) const noexcept;
		rgba_pixel rgba(const graphical::color&) const noexcept;

		std::optional<palette_view> palette() const noexcept;

		const char* name() const noexcept;

		private:
		static void free_pixel_format(const SDL_PixelFormat* one) noexcept;

		friend surface convert(const surface& source, const pixel_format& format);
	};

	const char* to_string(pixel_format::type) noexcept;

} // namespace simple::graphical

#endif /* end of include guard */
