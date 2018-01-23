#include "simple/support/function_utils.hpp"
#include "simple/support/enum.hpp"
#include "surface.h"
#include "utils.hpp"

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

	void surface::save(const char* filename) const
	{
		utils::throw_sdl_error(SDL_SaveBMP(guts().get(), filename));
	}

} // namespace simple::graphical
