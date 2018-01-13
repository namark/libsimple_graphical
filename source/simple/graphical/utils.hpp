#ifndef SIMPLE_GRAPHICAL_UTILS_HPP
#define SIMPLE_GRAPHICAL_UTILS_HPP

#include <stdexcept>
#include <string>
#include <memory>
#include <utility>
#include <SDL2/SDL.h>

#include "simple/support/range.hpp"
#include "simple/geom/vector.hpp"

namespace simple::graphical::utils
{

	template <typename Type>
	inline bool check_sdl_error(Type&& pointer)
	{
		return (NULL == pointer);
	}

	inline bool check_sdl_error(int error_code)
	{
		return error_code < 0;
	}

	template <typename Type>
	inline void throw_sdl_error(Type&& error)
	{
		using namespace std::literals;
		if ( check_sdl_error(std::forward<Type>(error)) )
			throw std::runtime_error("SDL_ERROR: "s + SDL_GetError());
	}

	template <typename SDL_Object>
	class sdl_object_wrapper
	{

		protected:
		using Deleter = void(*)(SDL_Object*);
		using object_ptr = std::unique_ptr<SDL_Object, Deleter>;

		private:
		object_ptr _guts;

		protected:

		sdl_object_wrapper(SDL_Object* guts, Deleter deleter)
			: _guts(guts, deleter)
		{
			throw_sdl_error(_guts.get());
		}

		const object_ptr& guts() const
		{
			return _guts;
		}

	};

	template <typename Rect>
	using sdl_rect_range_t = support::range<geom::vector<decltype(std::declval<Rect>().x), 2>>;

	template <typename Rect>
	inline Rect to_sdl_rect(const sdl_rect_range_t<Rect>& r) noexcept
	{
		Rect rect;
		rect.x = r.lower().x();
		rect.y = r.lower().y();
		auto size = r.upper() - r.lower();
		rect.w = size.x();
		rect.h = size.y();
		return rect;
	}

} // namespace simple::graphical::utils

#endif /* end of include guard */
