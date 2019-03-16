#ifndef SIMPLE_GRAPHICAL_RENDERER_H
#define SIMPLE_GRAPHICAL_RENDERER_H

#include "simple/support/enum_flags_operators.hpp"
#include "simple/sdlcore/utils.hpp"

#include "common_def.h"
#include "color.h"
#include "texture.h"
#include "pixels.hpp"

namespace simple::graphical
{

	using sdl_renderer_wrapper = sdlcore::utils::object_wrapper<SDL_Renderer>;

	class renderer_window;
	class surface;

	enum class quality_hint
	{
		nearest,
		linear,
		best
	};

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

		blend_mode blend() const noexcept;
		void blend(blend_mode new_value) const noexcept;

		quality_hint default_scale_quality() const noexcept;

		int2 target_size() const noexcept;

		basic_texture get_texture(const surface&) const;
		basic_texture get_texture(const surface&, quality_hint scale_quality) const;
		streaming_texture get_texture(int2 size, pixel_format::type) const;
		streaming_texture get_texture(int2 size, quality_hint scale_quality, pixel_format::type) const;
		render_texture get_render_texture(int2 size, pixel_format::type) const;
		render_texture get_render_texture(int2 size, quality_hint scale_quality,  pixel_format::type) const;
		void destroy_texture(texture&) const;

		blend_mode texture_blend(const texture&) const noexcept;
		void texture_blend(const texture&, blend_mode new_value) const noexcept;
		rgb_pixel texture_color(const texture&) const noexcept;
		void texture_color(const texture&, const rgb_pixel& new_value) const noexcept;
		uint8_t texture_alpha(const texture&) const noexcept;
		void texture_alpha(const texture&, uint8_t new_value) const noexcept;

		void update(const texture&, pixel_reader_variant, int2 destination = int2::zero()) const;
		void update(const streaming_texture&, std::function<void(pixel_writer_variant)>) const;
		void update(const streaming_texture&, std::function<void(pixel_writer_variant)>, range2D) const;

		std::optional<render_texture> target() const noexcept;
		void target(render_texture) const;
		void target_default() const noexcept;

		bool render(const texture&) const;
		bool render(const texture&, const range2D& destination) const;

		bool render(const texture_view&, int2 position = int2::zero()) const;
		bool render(const texture_view&, const range2D& destination) const;
		bool render(const texture_view&, int2 position, double angle) const;
		bool render(const texture_view&, const range2D& destination, double angle) const;

		protected:
		using sdl_renderer_wrapper::sdl_renderer_wrapper;
		renderer(SDL_Renderer*, quality_hint scale_quality, sdl_renderer_wrapper::Deleter);

		private:

		quality_hint _default_scale_quality;

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
