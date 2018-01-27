#ifndef SIMPLE_GRAPHICAL_RENDERER_H
#define SIMPLE_GRAPHICAL_RENDERER_H

#include "simple/support/enum_flags_operators.hpp"

#include "common_def.h"
#include "utils.hpp"
#include "color.h"

namespace simple::graphical
{

	using sdl_renderer_wrapper = utils::sdl_object_wrapper<SDL_Renderer>;

	class renderer_window;
	class surface;

	class renderer : public sdl_renderer_wrapper
	{

		public:

		enum class flags : uint32_t
		{
			automatic = 0, // prefers accelerated
			software = SDL_RENDERER_SOFTWARE, // the renderer is a software fallback
			accelerated = SDL_RENDERER_ACCELERATED, // the renderer uses hardware acceleration
			v_sync = SDL_RENDERER_PRESENTVSYNC, // present is synchronized with the refresh rate
			texture_target = SDL_RENDERER_TARGETTEXTURE // the renderer supports rendering to texture
		};

		explicit renderer(const surface& target);
		explicit renderer(surface&& target) = delete;

		void update() const noexcept;
		rgba_pixel color() const noexcept;
		void color(const rgba_pixel& new_value) const noexcept;
		point2D target_size() const noexcept;

		protected:
		using sdl_renderer_wrapper::sdl_renderer_wrapper;

		private:
		friend class renderer_window;
		friend bool fill(const renderer&);
		friend bool fill(const renderer&, const rgba_pixel&);
		friend bool fill(const renderer&, const range2D&);
		friend bool fill(const renderer&, const rgba_pixel&, const range2D&);
	};

	using ::operator |;
	using ::operator &;
	using ::operator &&;

} // namespace simple::graphical

template<> struct simple::support::define_enum_flags_operators<simple::graphical::renderer::flags>
	: std::true_type {};

#endif /* end of include guard */
