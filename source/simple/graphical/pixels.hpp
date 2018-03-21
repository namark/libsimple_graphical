#ifndef SIMPLE_GRAPHICAL_PIXELS_H
#define SIMPLE_GRAPHICAL_PIXELS_H

#include <memory>
#include <variant>

#include "common_def.h"
#include "color_vector.hpp"

namespace simple::graphical
{

	template<typename RawType>
	class pixels
	{
		// TODO: SDL independent pixels owner, compatible with the views below
	};

	namespace pixel_view_details
	{

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

		template<typename Tag, typename Pixel, typename RawType,
		std::enable_if_t<sizeof(Pixel) % sizeof(RawType) == 0>* = nullptr,
		std::enable_if_t<tag::valid<Tag>>* = nullptr>
		class impl
		{

			constexpr static int ratio = sizeof(Pixel) / sizeof(RawType);

			public:
			explicit impl(const pixels<RawType>&);


			const point2D& raw_size() const noexcept { return _raw_size; }
			point2D size() const noexcept { return _raw_size / point2D(ratio, 1); }

			// TODO: proper row_iterator interface?
			template<typename T = Tag, typename RT = tag::select_raw_type<T, RawType>>
			RT* row(int index = 0) const noexcept
			{ return reinterpret_cast<RT*>(_raw + index * _pitch); }

			template<typename T = Tag, typename RT = tag::select_raw_type<T, RawType>>
			RT* row_offset(RawType* row, int offset = 1) const noexcept
			{ return reinterpret_cast<RT*>(reinterpret_cast<pixel_byte*>(row) + offset * _pitch); }

			template<typename T = Tag, typename RT = tag::select_raw_type<T, RawType>>
			RT* next_row(RT* row) const noexcept
			{ return reinterpret_cast<RT*>(reinterpret_cast<pixel_byte*>(row) + _pitch); }

			template<typename T = Tag, typename RT = tag::select_raw_type<T, RawType>>
			RT* prev_row(RawType* row) const noexcept
			{ return reinterpret_cast<RT*>(reinterpret_cast<pixel_byte*>(row) - _pitch); }

			RawType& operator[](point2D position) const noexcept
			{ return row(position.y())[position.x()]; }

			auto get(point2D position)
			-> std::conditional_t<std::is_same_v<Pixel,RawType>, const Pixel&, Pixel>
			{
				if constexpr(std::is_same_v<Pixel,RawType>)
					return *this[position];
				else
				{
					Pixel pixel;
					position.x() *= ratio;
					memcpy(&pixel, &(*this)[position], sizeof(Pixel));
					return pixel;
				}
			}

			template<typename T=Tag, std::enable_if_t<std::is_same_v<T, tag::writer>>* = nullptr>
			void set(const Pixel& pixel, point2D position)
			{
				if constexpr(std::is_same_v<Pixel,RawType>)
					*this[position] = pixel;
				else
				{
					position.x() *= ratio;
					memcpy(&(*this)[position], &(pixel), sizeof(Pixel));
				}
			}

			explicit impl(const impl & other)
			: _raw(other._raw), _raw_size(other._raw_size), _pitch(other._pitch)
			{}

			explicit impl(const impl & other, range2D range) :
				_raw(other._raw + to_index(range.lower(), other._pitch)),
				_raw_size(range.upper() - range.lower()),
				_pitch(other._pitch)
			{}

			protected:

			impl(pixel_byte* target, point2D size, int pitch = 0)
			: _raw(target), _raw_size(size), _pitch(pitch ? pitch : size.x() * sizeof(RawType))
			{}

			int pitch() const noexcept { return _pitch; }

			template<typename OtherTag>
			explicit impl(const impl<OtherTag, Pixel, RawType> & other)
			: _raw(other._raw), _raw_size(other._raw_size), _pitch(other._pitch)
			{}

			template<typename OtherTag>
			explicit impl(const impl<OtherTag, Pixel, RawType> & other, range2D range) :
				_raw(other._raw + to_index(range.lower(), other._pitch)),
				_raw_size(range.upper() - range.lower()),
				_pitch(other._pitch)
			{}

			private:
			pixel_byte* _raw;
			point2D _raw_size;
			int _pitch;

			static int to_index(const point2D& position, int pitch) noexcept
			{
				return position.x() * sizeof(RawType) + position.y() * pitch;
			}

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

	class pixel_format;

	template<typename Pixel, typename RawType = Pixel>
	class pixel_writer :
		public pixel_view_details::impl<pixel_view_details::tag::writer, Pixel, RawType>
	{
		public:
		using impl = pixel_view_details::impl<pixel_view_details::tag::writer, Pixel, RawType>;
		using impl::impl;

		private:
		pixel_writer(pixel_byte* target, point2D size, int pitch = 0)
			: impl(target, size, pitch)
		{}
		friend pixel_view_variant<pixel_writer> pixel_writer_from_format(pixel_byte*, point2D, int, int bpp);
	};

	template<typename Pixel, typename RawType = Pixel>
	class pixel_reader :
		public pixel_view_details::impl<pixel_view_details::tag::reader, Pixel, RawType>
	{
		public:
		using impl = pixel_view_details::impl<pixel_view_details::tag::reader, Pixel, RawType>;
		using impl::impl;

		explicit pixel_reader
		(const pixel_writer<Pixel, RawType> & other)
		: impl(other)
		{}

		explicit pixel_reader
		(
		 	const pixel_writer<Pixel, RawType> & other,
			range2D range
		)
		: impl(other, range)
		{}
	};

	using pixel_reader_variant = pixel_view_variant<pixel_reader>;
	using pixel_writer_variant = pixel_view_variant<pixel_writer>;

} // namespace simple::graphical

#endif /* end of include guard */
