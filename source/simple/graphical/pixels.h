#ifndef SIMPLE_GRAPHICAL_PIXELS_H
#define SIMPLE_GRAPHICAL_PIXELS_H

#include <variant>
#include <cassert>

#include "common_def.h"
#include "color_vector.hpp"

namespace simple::graphical
{

	class color;

	template<typename RawType>
	class pixels
	{
		// TODO: SDL independent pixels owner, compatible with the views below
	};

	namespace pixel_view_details
	{

		template <typename Pixel>
		struct pixel_fraction { using type = rgba_vector; };

		template <>
		struct pixel_fraction<rgba_pixel> { using type = rgba_vector; };
		template <>
		struct pixel_fraction<rgb_pixel> { using type = rgb_vector; };
		template <>
		struct pixel_fraction<uint16_t> { using type = float; };
		template <>
		struct pixel_fraction<pixel_byte> { using type = float; };

		template <typename Pixel>
		using pixel_fraction_t = typename pixel_fraction<Pixel>::type;

		class tag
		{
			public:
			class pixel_view {};
			class reader : public pixel_view {};
			class writer : public pixel_view {};

			template <typename Tag>
			static constexpr bool valid = std::is_base_of_v<pixel_view, Tag>;


			template<typename T, typename RT>
			using select_raw_type = std::conditional_t<std::is_same_v<T, tag::reader>, std::add_const_t<RT>, RT>;

		};

		template <typename Tag, typename Pixel, typename RawType>
		static constexpr bool valid_impl = tag::valid<Tag> && (sizeof(Pixel) % sizeof(RawType) == 0);

		// template<typename Tag, typename Pixel, typename RawType,
		// std::enable_if_t<valid_impl<Tag, Pixel, RawType>>* = nullptr>
		template<typename Tag, typename Pixel, typename RawType>
		class impl
		{

			protected:
			constexpr static int2 ratio{(int)(sizeof(Pixel) / sizeof(RawType)),1};

			public:
			using raw_type = tag::select_raw_type<Tag, RawType>;
			using byte_type = tag::select_raw_type<Tag, pixel_byte>;
			using pixel_type = Pixel;

			const int2& raw_size() const noexcept;
			int2 size() const noexcept;

			// TODO: proper row_iterator interface?
			raw_type* row(int index = 0) const noexcept;

			raw_type* row_offset(raw_type* row, int offset = 1) const noexcept;

			raw_type* next_row(raw_type* row) const noexcept;

			raw_type* prev_row(raw_type* row) const noexcept;

			raw_type& operator[](int2 position) const noexcept;

			simple::support::range<raw_type*> raw_range() const noexcept;

			auto get(int2 position) const
			-> std::conditional_t<std::is_same_v<Pixel,RawType>, const Pixel&, Pixel>;

			template <typename ColorVector = pixel_fraction_t<Pixel>>
			Pixel get(float2 position) const;

			template <typename ColorVector = pixel_fraction_t<Pixel>>
			Pixel get(float2 position, range2D spread) const;

			impl(const impl & other, range2D range);

			int pitch() const noexcept;

			protected:

			impl(tag::select_raw_type<Tag,pixel_byte>* target, int2 size, int pitch = 0);

			template<typename OtherTag>
			impl(const impl<OtherTag, Pixel, RawType> & other);

			template<typename OtherTag>
			impl(const impl<OtherTag, Pixel, RawType> & other, range2D range);

			private:
			tag::select_raw_type<Tag,pixel_byte>* _raw;
			int2 _raw_size;
			int _pitch;

			static int to_index(const int2& position, int pitch) noexcept;

		};

	} // namespace pixel_view_details


	// TODO: add more pixel types, for all the crazy pixel formats SDL supports
	template<template<typename Pixel, typename Raw = Pixel> typename PixelView>
	using pixel_view_variant = std::variant<
		PixelView<pixel_byte>, // 1 bytes per pixel
		PixelView<uint16_t, pixel_byte>, // 2 bytes per pixel
		PixelView<rgb_pixel, pixel_byte>, // 3 bytes per pixel
		PixelView<rgba_pixel, pixel_byte> // 4 bytes per pixel
	>;

	template<typename Pixel, typename RawType = Pixel>
	class pixel_writer :
		public pixel_view_details::impl<pixel_view_details::tag::writer, Pixel, RawType>
	{
		public:
		using impl = pixel_view_details::impl<pixel_view_details::tag::writer, Pixel, RawType>;
		using impl::impl;

		void set(const Pixel& pixel, int2 position) const;

		template <typename ColorVector = rgba_vector>
		void set(const Pixel& pixel, float2 position) const;

		template <typename ColorVector = rgba_vector>
		void set(const ColorVector& pixel, float2 position) const;

		template <typename ColorVector = rgba_vector>
		void set(const Pixel& pixel, float2 position, range2D spread) const;

		template <typename ColorVector = rgba_vector>
		void set(const ColorVector& pixel, float2 position, range2D spread) const;

		void set(const color& pixel, int2 position) const;

		private:
		pixel_writer(pixel_byte* target, int2 size, int pitch = 0);
		friend pixel_view_variant<pixel_writer> pixel_writer_from_format(pixel_byte*, int2, int, int bpp);
	};

	template<typename Pixel, typename RawType>
	pixel_writer(pixel_writer<Pixel, RawType>, range2D)
		-> pixel_writer<Pixel, RawType>;

	template<typename Pixel, typename RawType = Pixel>
	class pixel_reader :
		public pixel_view_details::impl<pixel_view_details::tag::reader, Pixel, RawType>
	{
		public:
		using impl = pixel_view_details::impl<pixel_view_details::tag::reader, Pixel, RawType>;
		using impl::impl;

		pixel_reader(const pixel_writer<Pixel, RawType> & other);

		pixel_reader
		(
		 	const pixel_writer<Pixel, RawType> & other,
			range2D range
		);
	};

	template<typename Pixel, typename RawType>
	pixel_reader(pixel_writer<Pixel, RawType>)
		-> pixel_reader<Pixel, RawType>;

	template<typename Pixel, typename RawType>
	pixel_reader(pixel_writer<Pixel, RawType>, range2D)
		-> pixel_reader<Pixel, RawType>;

	template<typename Pixel, typename RawType>
	pixel_reader(pixel_reader<Pixel, RawType>, range2D)
		-> pixel_reader<Pixel, RawType>;

	using pixel_reader_variant = pixel_view_variant<pixel_reader>;
	using pixel_writer_variant = pixel_view_variant<pixel_writer>;

} // namespace simple::graphical

#endif /* end of include guard */
