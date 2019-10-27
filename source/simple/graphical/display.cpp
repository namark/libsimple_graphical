#include "display.h"

namespace simple::graphical
{

	namespace detail
	{

		template <typename Derived, typename... Payload>
		Derived& index_iterator<Derived,Payload...>::operator++() noexcept
		{
			++index;
			return static_cast<Derived&>(*this);
		}

		template <typename Derived, typename... Payload>
		Derived index_iterator<Derived,Payload...>::operator++(int) noexcept
		{
			Derived temp = static_cast<Derived&>(*this);
			++(*this);
			return temp;
		}

		template <typename Derived, typename... Payload>
		bool index_iterator<Derived,Payload...>::operator==(const Derived& other) const noexcept
		{
			return index == other.index;
		}

		template <typename Derived, typename... Payload>
		bool index_iterator<Derived,Payload...>::operator!=(const Derived& other) const noexcept
		{
			return !(*this == other);
		}

		template <typename Derived, typename... Payload>
		int index_iterator<Derived,Payload...>::operator-(const Derived& other) const noexcept
		{
			return index - other.index;
		}

		template <typename Derived, typename... Payload>
		index_iterator<Derived,Payload...>::index_iterator(protector, int index, Payload... payload) noexcept
		: index(index), payload{payload...}
		{ }

		template <typename Iterator, typename... Payload>
		Iterator index_list<Iterator,Payload...>::make_iterator(int index) const noexcept
		{
			using protector = typename Iterator::protector;
			return std::make_from_tuple<Iterator>(
				std::tuple_cat(
					std::tuple{protector{},index},
					payload
				)
			);
		}

		template <typename Iterator, typename... Payload>
		Iterator index_list<Iterator,Payload...>::begin() const noexcept
		{
			return make_iterator(0);
		}

		template <typename Iterator, typename... Payload>
		Iterator index_list<Iterator,Payload...>::end() const noexcept
		{
			return make_iterator(count);
		}

		template <typename Iterator,typename... Payload>
		index_list<Iterator,Payload...>::index_list(int count, Payload... payload) noexcept
		: count(count), payload{payload...}
		{ }

		template class index_iterator<display::mode_iterator,int>;
		template class index_list<display::mode_iterator, int>;
		template class index_iterator<display_iterator>;
		template class index_list<display_iterator>;

	} // namespace detail

	display::mode from_sdl_mode(const SDL_DisplayMode& mode)
	{
		return {
			static_cast<pixel_format::type>(mode.format),
			{mode.w, mode.h},
			mode.refresh_rate
		};
	}

	bool display::mode::operator==(const mode& other) const noexcept
	{
		return other.format == format &&
			other.size == size &&
			other.refresh_rate == refresh_rate;
	}

	bool display::mode::operator!=(const mode& other) const noexcept
	{
		return !operator==(other);
	}

	auto display::mode_iterator::operator*() const -> mode
	{
		SDL_DisplayMode mode;
		int display_index = std::get<0>(payload);
		sdlcore::utils::throw_error(
			SDL_GetDisplayMode(display_index, index, &mode));
		return from_sdl_mode(mode);
	}

	auto display::current_mode() const -> mode
	{
		SDL_DisplayMode mode;
		sdlcore::utils::throw_error(
			SDL_GetCurrentDisplayMode(index, &mode));
		return from_sdl_mode(mode);
	}

	auto display::initial_mode() const -> mode
	{
		SDL_DisplayMode mode;
		sdlcore::utils::throw_error(
			SDL_GetDesktopDisplayMode(index, &mode));
		return from_sdl_mode(mode);
	}

	auto display::modes() const noexcept -> mode_list
	{
		auto count = SDL_GetNumDisplayModes(index);
		if(count < 0)
			count = 0;
		return mode_list(count, index);
	}

	const char* display::name() const
	{
		auto result = SDL_GetDisplayName(index);
		sdlcore::utils::throw_error(result);
		return result;
	}

	display::display(int index) noexcept : index(index)
	{ }

	display display_iterator::operator*() const
	{
		return display(index);
	}

} // namespace simple::graphical
