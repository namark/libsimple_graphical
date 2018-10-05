#ifndef SIMPLE_GRAPHICAL_TEXTURE_H
#define SIMPLE_GRAPHICAL_TEXTURE_H
#include <tuple>

#include "common_def.h"
#include "pixel_format.h"

namespace simple::graphical
{

	class texture
	{
		public:

		enum class access
		{
			basic = SDL_TEXTUREACCESS_STATIC,
			streaming = SDL_TEXTUREACCESS_STREAMING,
			render_target = SDL_TEXTUREACCESS_TARGET
		};

		enum class flip_direction
		{
			none = SDL_FLIP_NONE,
			horizontal = SDL_FLIP_HORIZONTAL,
			vertical = SDL_FLIP_VERTICAL
		};

		std::tuple<bool, access, int2, pixel_format::type> info() const noexcept;

		protected:
		SDL_Texture* guts() const;
		explicit texture(SDL_Texture* guts);

		private:
		SDL_Texture* _guts;
		explicit texture(SDL_Renderer*, SDL_Surface*);
		explicit texture(SDL_Renderer*, pixel_format::type, int2 size, access);

		friend class renderer;
	};

	class basic_texture;
	class streaming_texture;
	class render_texture;

	class basic_texture : public texture
	{
		public:
		basic_texture(const streaming_texture &) noexcept;
		basic_texture(const render_texture &) noexcept;

		private:
		using texture::texture;
		friend class streaming_texture;
		friend class render_texture;
		friend class renderer;
	};

	class streaming_texture : public texture
	{
		public:
		explicit streaming_texture(const basic_texture &);

		private:
		using texture::texture;
		friend class basic_texture;
		friend class renderer;
	};

	class render_texture : public texture
	{
		public:
		explicit render_texture(const basic_texture &);

		private:
		using texture::texture;
		friend class basic_texture;
		friend class renderer;
	};

	using static_texture = basic_texture;

	struct texture_view
	{
		basic_texture texture;
		// TODO: make invalid ranges flip the texture
		range2D region = {int2{0,0}, range2D::limit().upper()};
		int2 pivot;
		texture::flip_direction flip;

		texture_view(basic_texture, range2D, int2 pivot = int2::zero(), texture::flip_direction flip = texture::flip_direction::none);
		texture_view(basic_texture, int2 pivot = int2::zero(), texture::flip_direction flip = texture::flip_direction::none);
	};

	using ::operator |;
	using ::operator &;
	using ::operator &&;

} // namespace simple::graphical

template<> struct simple::support::define_enum_flags_operators<simple::graphical::texture::flip_direction>
	: std::true_type {};

#endif /* end of include guard */
