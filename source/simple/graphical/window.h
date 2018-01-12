#ifndef SIMPLE_GRAPHICAL_WINDOW_H
#define SIMPLE_GRAPHICAL_WINDOW_H

#include <memory>
#include <string>
#include <SDL2/SDL.h>

#include "simple/support/enum_flags_operators.hpp"

#include "common_def.h"
#include "surface.h"
#include "utils.hpp"

namespace simple::graphical
{

	using sdl_window_wrapper = utils::sdl_object_wrapper<SDL_Window>;

	class window : public sdl_window_wrapper
	{

		public:

		enum class flags : uint32_t
		{
			fullscreen = SDL_WINDOW_FULLSCREEN, // fullscreen window
			fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP, // fullscreen window at the current desktop resolution
			opengl = SDL_WINDOW_OPENGL, // window usable with OpenGL context
			shown = SDL_WINDOW_SHOWN, // window is visible
			hidden = SDL_WINDOW_HIDDEN, // window is not visible
			borderless = SDL_WINDOW_BORDERLESS, // no window decoration
			resizable = SDL_WINDOW_RESIZABLE, // window can be resized
			minimized = SDL_WINDOW_MINIMIZED, // window is minimized
			maximized = SDL_WINDOW_MAXIMIZED, // window is maximized
			input_grabbed = SDL_WINDOW_INPUT_GRABBED, // window has grabbed input focus
			input_focus = SDL_WINDOW_INPUT_FOCUS, // window has input focus
			mouse_focus = SDL_WINDOW_MOUSE_FOCUS, // window has mouse focus
			foreign = SDL_WINDOW_FOREIGN, // window not created by SDL

#if SDL_VERSION_ATLEAST(2,0,1)
			allow_highdpi = SDL_WINDOW_ALLOW_HIGHDPI, // window should be created in high-DPI mode if supported
#endif

#if SDL_VERSION_ATLEAST(2,0,4)
			mouse_capture = SDL_WINDOW_MOUSE_CAPTURE, // window has mouse captured (unrelated to INPUT_GRABBED)
#endif

#if SDL_VERSION_ATLEAST(2,0,5)
			always_on_top = SDL_WINDOW_ALWAYS_ON_TOP, // window should always be above others (X11 only)
			skip_taskbar = SDL_WINDOW_SKIP_TASKBAR, // window should not be added to the taskbar (X11 only)
			utility = SDL_WINDOW_UTILITY, // window should be treated as a utility window (X11 only)
			tooltip = SDL_WINDOW_TOOLTIP, // window should be treated as a tooltip (X11 only)
			popup_menu = SDL_WINDOW_POPUP_MENU, // window should be treated as a popup menu (X11 only)
#endif

		};

		constexpr static const point2D undefined_position = point2D::one() * SDL_WINDOWPOS_UNDEFINED;
		constexpr static const point2D center_position = point2D::one() * SDL_WINDOWPOS_CENTERED;

		window
		(
			std::string title,
			point2D size,
			flags windowflags = flags::shown,
			point2D position = undefined_position
		);

		const graphical::surface & surface() const;

		void update()
		{
			utils::throw_sdl_error(SDL_UpdateWindowSurface(guts().get()));
		}

		private:

		class free_surface : public graphical::surface
		{
			public:
			free_surface(SDL_Surface* guts);
		};

		const free_surface _surface;

	};

	using ::operator |;
	using ::operator &;
	using ::operator &&;

} // namespace simple::graphical

template<> struct simple::support::define_enum_flags_operators<simple::graphical::window::flags>
	: std::true_type {};

#endif /* end of include guard */

