#ifndef SIMPLE_GRAPHICAL_UTILS_HPP
#define SIMPLE_GRAPHICAL_UTILS_HPP

#include <stdexcept>
#include <string>
#include <memory>
#include <utility>
#include <SDL2/SDL.h>

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

} // namespace simple::graphical::utils

#endif /* end of include guard */
