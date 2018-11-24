#ifndef SIMPLE_GRAPHICAL_GL_WINDOW_H
#define SIMPLE_GRAPHICAL_GL_WINDOW_H
#include <bitset>
#include "simple/support/enum.hpp"
#include "simple/support/array_utils.hpp"
#include "window.h"

namespace simple::graphical
{

	class gl_window : public window
	{

		public:

		enum class context_flag
		{
			debug = SDL_GL_CONTEXT_DEBUG_FLAG,
			forward_compatible = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
			robust_access = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
			reset_isolation = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
		};

		enum class context_profile
		{
			core = SDL_GL_CONTEXT_PROFILE_CORE,
			compatibility = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
			es = SDL_GL_CONTEXT_PROFILE_ES
		};

		enum class attribute
		{
			red,
			green,
			blue,
			alpha,
			buffer,
			double_buffer,
			depth,
			stencil,
			accum_red,
			accum_green,
			accum_blue,
			accum_alpha,
			stereo,
			msaa_buffers,
			msaa_samples,
			force_acceleration,
			major_version,
			minor_version,
			context_flags,
			context_profile,
			share_context,

#if SDL_VERSION_ATLEAST(2,0,1)
			srgb_framebuffer,
#endif

#if SDL_VERSION_ATLEAST(2,0,4)
			release_context,
#endif
		};

		enum class vsync_mode
		{
			disabled = 0,
			enablded = 1,
			adaptive = -1
		};

		using config_data = std::tuple<
			size_t,       // red
			size_t,       // green
			size_t,       // blue
			size_t,       // alpha
			size_t,       // buffer
			bool,         // double_buffer
			size_t,       // depth
			size_t,       // stencil
			size_t,       // accum_red
			size_t,       // accum_green
			size_t,       // accum_blue
			size_t,       // accum_alpha
			bool,         // stereo
			size_t,       // msaa_buffers
			size_t,       // msaa_samples
			bool,         // force_acceleration
			size_t,       // major_version
			size_t,       // minor_version
			context_flag,
			context_profile,
			bool          // share_context

#if SDL_VERSION_ATLEAST(2,0,1)
			,bool         // srgb_framebuffer
#endif

#if SDL_VERSION_ATLEAST(2,0,4)
			,bool         // release_context
#endif
		>;

		static SDL_GLattr to_sdl_glatrr(attribute attr);

		class config
		{
			int active_windows = 0;
			config_data data;
			std::bitset<std::tuple_size_v<config_data>> required_attributes;

			template <size_t Index>
			bool check_all() noexcept
			{
				bool result = (bool)check<static_cast<attribute>(Index)>();
				if constexpr (Index > 0)
					return result && check_all<Index - 1>();
				else
					return result;
			}

			template <size_t Index>
			void ensure_requirements()
			{
				if(required_attributes.test(Index) && !check<static_cast<attribute>(Index)>())
					throw std::runtime_error("Requirement not satisfied!");
				if constexpr (Index > 0)
					ensure_requirements<Index - 1>();
			}

			config() = default;
			config(const config&) = delete;
			config(config&&) = delete;

			void register_window() noexcept;
			void unregister_window() noexcept;

			friend class gl_window;

			public:

			struct request_info
			{
				enum class state
				{
					pending,
					failure,
					success
				};

				state status;

				explicit operator bool() noexcept { return state::success == status; }
			};

			template<attribute A>
			struct request_data
			{
				using value_type = std::tuple_element_t<support::to_integer(A), config_data>;
				value_type requested;
				value_type retrieved;
			};


			template<attribute A>
			request_info request(const std::tuple_element_t<support::to_integer(A), config_data>& value) noexcept
			{
				constexpr auto attribute_id = support::to_integer(A);
				std::get<attribute_id>(data) = value;

				if(0 == active_windows)
				{
					SDL_GL_SetAttribute(to_sdl_glatrr(A), static_cast<int>(value));
					return {request_info::state::pending};
				}

				int current;
				bool sdl_result = sdlcore::utils::check_error(
					SDL_GL_GetAttribute(to_sdl_glatrr(A), &current) );

				if(!sdl_result || current != static_cast<int>(value))
					return {request_info::state::failure};

				return {request_info::state::success};
			}

			template<attribute A>
			request_info require(std::tuple_element_t<support::to_integer(A), config_data> value)
			{
				auto info = request<A>(value);
				switch(info.status)
				{
					case request_info::state::pending:
						required_attributes.set(support::to_integer(A));
						break;

					case request_info::state::success:
						break;

					case request_info::state::failure:
						throw std::runtime_error("Requirement failed!");
						break;
				}
				return info;
			}

			template<attribute A>
			request_info check() noexcept
			{
				if(0 == active_windows)
					return {request_info::state::pending};

				auto data = get<A>();
				if(data.requested == data.retrieved)
					return {request_info::state::success};

				return {request_info::state::failure};
			}

			template<attribute A>
			request_data<A> get() noexcept
			{
				constexpr auto id = support::to_integer(A);
				int value;
				SDL_GL_GetAttribute(to_sdl_glatrr(A), &value);
				return {std::get<id>(data), static_cast<std::tuple_element_t<id, config_data>>(value)};
			}

			bool check_all() noexcept
			{
				return check_all<std::tuple_size_v<config_data> - 1>();
			}

			void ensure_requirements()
			{
				ensure_requirements<std::tuple_size_v<config_data> - 1>();
			}

		};

		private:

		using context_wrapper = sdlcore::utils::object_wrapper<void>;
		class : public context_wrapper
		{
			using context_wrapper::object_wrapper;
			friend class gl_window;
		} context;

		public:

		static config global;

		gl_window
		(
			std::string title,
			int2 size,
			flags windowflags = flags::shown,
			int2 position = undefined_position
		);

		~gl_window() noexcept;

		void update() noexcept;

		vsync_mode vsync();
		bool request_vsync(vsync_mode);
		void require_vsync(vsync_mode);

	};


} // namespace simple::graphical

template<>
struct simple::support::define_enum_flags_operators<simple::graphical::gl_window::context_flag>
	: std::true_type {};

#endif /* end of include guard */
