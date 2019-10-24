#ifndef SIMPLE_GRAPHICAL_DISPLAY_H
#define SIMPLE_GRAPHICAL_DISPLAY_H

#include "common_def.h"
#include "pixel_format.h"

namespace simple::graphical
{

	namespace detail
	{

		// there is probably some library that does this better...
		// maybe boost/iterator??
		template <typename Derived, typename... Payload>
		class index_iterator
		{
			protected:

			class protector {};

			public:
			Derived& operator++() noexcept;
			Derived operator++(int) noexcept;
			bool operator==(const Derived&) const noexcept;
			bool operator!=(const Derived&) const noexcept;
			int operator-(const Derived&) const noexcept;

			explicit index_iterator(protector, int, Payload...) noexcept;

			protected:
			int index;
			std::tuple<Payload...> payload;
		};

		template <typename Iterator, typename... Payload>
		class index_list
		{
			public:
			Iterator begin() const noexcept;
			Iterator end() const noexcept;

			protected:
			explicit index_list(int, Payload...) noexcept;
			private:
			Iterator make_iterator(int) const noexcept;
			int count;
			std::tuple<Payload...> payload;
		};

	} // namespace detail

	class display
	{
		public:

		struct mode
		{
			pixel_format::type format;
			int2 size;
			int refresh_rate;
			bool operator==(const mode&) const noexcept;
			bool operator!=(const mode&) const noexcept;
		};

		class mode_iterator : public detail::index_iterator<mode_iterator,int>
		{
			public:
			using index_iterator<mode_iterator,int>::index_iterator;
			mode operator*() const;

			friend class detail::index_list<mode_iterator, int>;
		};

		class mode_list : public detail::index_list<mode_iterator, int>
		{
			private:
			using index_list<mode_iterator,int>::index_list;
			friend display;
		};

		mode current_mode() const;
		mode initial_mode() const;
		mode_list modes() const noexcept;
		// TODO: bounds, DPI
		private:
		explicit display(int) noexcept;
		int index;

		friend class display_iterator;
		friend class window;
	};

	class display_iterator : public detail::index_iterator<display_iterator>
	{
		public:
		using index_iterator<display_iterator>::index_iterator;
		display operator*() const;

		friend class detail::index_list<display_iterator>;
	};

	class display_list : public detail::index_list<display_iterator>
	{
		private:
		using index_list<display_iterator>::index_list;
		friend class initializer;
	};


} // namespace simple::graphical

#endif /* end of include guard */
