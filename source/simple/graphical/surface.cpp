#include "simple/support/function_utils.hpp"
#include "simple/support/enum.hpp"
#include "simple/sdlcore/utils.hpp"
#include "surface.h"

using simple::support::to_integer;

namespace simple::graphical
{

	surface::surface(const char* filename)
		: sdl_surface_wrapper(SDL_LoadBMP(filename), SDL_FreeSurface),
		_format(this->guts()->format)
	{}

	surface::surface(const surface& other)
	: sdl_surface_wrapper
	(
#if SDL_VERSION_ATLEAST(2,0,6)
	 	SDL_DuplicateSurface(other.guts().get());
#else
		SDL_ConvertSurface(other.guts().get(), other.guts()->format, other.guts()->flags),
#endif
		SDL_FreeSurface
	),
	_format(this->guts()->format)
	{}

	surface::surface(point2D size, const pixel_format& format)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurface
		(
			0,
			size.x(), size.y(),
			format.bits(),
			format.red_mask(), format.green_mask(), format.blue_mask(), format.alpha_mask()
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format)
	{}

	surface::surface(byte* pixels, point2D size, const pixel_format& format, int pitch)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurfaceFrom
		(
			pixels,
			size.x(), size.y(),
			format.bits(),
			pitch ? pitch : format.bytes() * size.x(),
			format.red_mask(), format.green_mask(), format.blue_mask(), format.alpha_mask()
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format)
	{}

	surface::surface(std::unique_ptr<byte[]> pixels, point2D size, const pixel_format& format, int pitch)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurfaceFrom
		(
			pixels.get(),
			size.x(), size.y(),
			format.bits(),
			pitch ? pitch : format.bytes() * size.x(),
			format.red_mask(), format.green_mask(), format.blue_mask(), format.alpha_mask()
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format),
	pixels_owner(pixels.release(), [](byte* x){ delete [] x; })
	{}

	surface::surface(std::unique_ptr<byte[], void(*)(byte*)> pixels, point2D size, const pixel_format& format, int pitch)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurfaceFrom
		(
			pixels.get(),
			size.x(), size.y(),
			format.bits(),
			pitch ? pitch : format.bytes() * size.x(),
			format.red_mask(), format.green_mask(), format.blue_mask(), format.alpha_mask()
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format),
	pixels_owner(std::move(pixels))
	{}

#if SDL_VERSION_ATLEAST(2,0,5)

	surface::surface(point2D size, pixel_format::type format)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurfaceWithFormat
		(
			0,
			size.x(), size.y(),
			0,
			to_integer(format)
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format)
	{}

	surface::surface(byte* pixels, point2D size, pixel_format::type format, int pitch)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurfaceWithFormatFrom
		(
			pixels,
			size.x(), size.y(),
			0,
			pitch ? pitch : SDL_BYTESPERPIXEL(to_integer(format)) * size.x(),
			to_integer(format)
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format)
	{}

	surface::surface(std::unique_ptr<byte[]> pixels, point2D size, pixel_format::type format, int pitch)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurfaceWithFormatFrom
		(
			pixels.get(),
			size.x(), size.y(),
			format.bits(),
			pitch ? pitch : SDL_BYTESPERPIXEL(to_integer(format)) * size.x(),
			to_integer(format);
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format),
	pixels_owner(pixels.release(), [](byte* x){ delete [] x; })
	{}

	surface::surface(std::unique_ptr<byte[], void(*)(byte*)> pixels, point2D size, const pixel_format& format, int pitch)
	: sdl_surface_wrapper
	(
		SDL_CreateRGBSurfaceWithFormatFrom
		(
			pixels.get(),
			size.x(), size.y(),
			format.bits(),
			pitch ? pitch : SDL_BYTESPERPIXEL(to_integer(format)) * size.x(),
			to_integer(format)
		),
		SDL_FreeSurface
	),
	_format(this->guts()->format),
	pixels_owner(std::move(pixels))
	{}

#endif

	surface::surface(SDL_Surface* guts, Deleter deleter)
		: sdl_surface_wrapper(guts, deleter),
		_format(this->guts()->format)
	{}

	surface::free_pixel_format::free_pixel_format(SDL_PixelFormat* guts)
		: pixel_format(guts, support::nop)
	{}

	const pixel_format& surface::format() const
	{
		return _format;
	}

	point2D surface::size() const
	{
		return {guts()->w, guts()->h};
	}

	blend_mode surface::blend() const noexcept
	{
		SDL_BlendMode result;
		SDL_GetSurfaceBlendMode(guts().get(), &result);
		return static_cast<blend_mode>(result);
	}

	void surface::blend(blend_mode new_value) const noexcept
	{
		SDL_SetSurfaceBlendMode
		(
			guts().get(),
			static_cast<SDL_BlendMode>(support::to_integer(new_value))
		);
	}

	uint8_t surface::alpha() const noexcept
	{
		uint8_t result;
		SDL_GetSurfaceAlphaMod(guts().get(), &result);
		return result;
	}

	void surface::alpha(uint8_t new_value) const noexcept
	{
		SDL_SetSurfaceAlphaMod(guts().get(), new_value);
	}

	rgb_pixel surface::color() const noexcept
	{
		rgb_pixel result;
		SDL_GetSurfaceColorMod(guts().get(), &result.r(), &result.g(), &result.b());
		return result;
	}

	void surface::color(rgb_pixel new_value) const noexcept
	{
		SDL_SetSurfaceColorMod(guts().get(), new_value.r(), new_value.g(), new_value.b());
	}

	pixel_writer_variant pixel_writer_from_format(pixel_byte* data, point2D size, int pitch, int bpp)
	{
		point2D raw_size = size;
		raw_size.x() *= bpp;
		switch(bpp)
		{
			case 2:
			return pixel_writer<uint16_t, pixel_byte>(
				data, raw_size, pitch);
			case 3:
			return pixel_writer<rgb_pixel, pixel_byte>(
				data, raw_size, pitch);
			case 4:
			return pixel_writer<rgba_pixel, pixel_byte>(
				data, raw_size, pitch);
			default:
			return pixel_writer<pixel_byte>(
				data, raw_size, pitch);
		}
	}

	pixel_writer_variant surface::pixels() const noexcept
	{
		return pixel_writer_from_format(
				reinterpret_cast<byte*>(guts()->pixels),
				size(), guts()->pitch, format().bytes());
	}

	void surface::save(const char* filename) const
	{
		sdlcore::utils::throw_error(SDL_SaveBMP(guts().get(), filename));
	}

} // namespace simple::graphical
