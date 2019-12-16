#ifndef SIMPLE_GRAPHICAL_PIXELS_HPP
#define SIMPLE_GRAPHICAL_PIXELS_HPP
#include "pixels.h"

#include <memory>
#include <cassert>

#include "common_def.h"
#include "color_vector.hpp"
#include "simple/support/algorithm.hpp"
#include "color.h"

namespace simple::graphical
{

	namespace pixel_view_details
	{

		template<typename T, typename P, typename R>
		const int2& impl<T,P,R>::raw_size() const noexcept
		{ return _raw_size; }
		template<typename T, typename P, typename R>
		int2 impl<T,P,R>::size() const noexcept
		{ return _raw_size / ratio; }

		template<typename T, typename P, typename R>
		auto impl<T,P,R>::row(int index) const noexcept
		-> raw_type*
		{ return reinterpret_cast<raw_type*>(_raw + index * _pitch); }

		template<typename T, typename P, typename R>
		auto impl<T,P,R>::row_offset(raw_type* row, int offset) const noexcept
		-> raw_type*
		{ return reinterpret_cast<raw_type*>(
			reinterpret_cast<byte_type*>(row) +
			offset * _pitch
		); }

		template<typename T, typename P, typename R>
		auto impl<T,P,R>::next_row(raw_type* row) const noexcept
		-> raw_type*
		{ return reinterpret_cast<raw_type*>(
			reinterpret_cast<byte_type*>(row) + _pitch
		); }

		template<typename T, typename P, typename R>
		auto impl<T,P,R>::prev_row(raw_type* row) const noexcept
		-> raw_type*
		{ return reinterpret_cast<raw_type*>(
			reinterpret_cast<byte_type*>(row) - _pitch
		); }

		template<typename T, typename P, typename R>
		auto impl<T,P,R>::operator[](int2 position) const noexcept
		-> raw_type&
		{
			assert(int2::zero() <= position && position < raw_size());
			return row(position.y())[position.x()];
		}

		template<typename T, typename P, typename R>
		auto impl<T,P,R>::raw_range() const noexcept
		-> simple::support::range<raw_type*>
		{
			return {row(), row_offset(row(),raw_size().y())};
		}

		template<typename T, typename Pixel, typename RawType>
		auto impl<T,Pixel,RawType>::get(int2 position) const
		-> std::conditional_t<std::is_same_v<Pixel,RawType>, const Pixel&, Pixel>
		{
			assert(int2::zero() <= position && position < size());
			if constexpr(std::is_same_v<Pixel,RawType>)
				return (*this)[position];
			else
			{
				Pixel pixel;
				position.x() *= ratio.x();
				memcpy(&pixel, &(*this)[position], sizeof(Pixel));
				return pixel;
			}
		}

		template<typename T, typename P, typename R>
		impl<T,P,R>::impl(const impl & other, range2D range) :
			_raw(reinterpret_cast<tag::select_raw_type<T,pixel_byte>*>(other.row())
				+ to_index(range.lower(), other.pitch())),
			_raw_size((range.upper() - range.lower())*ratio),
			_pitch(other.pitch())
		{}

		template<typename T, typename P, typename R>
		int impl<T,P,R>::pitch() const noexcept { return _pitch; }

		template<typename T, typename P, typename RawType>
		impl<T,P,RawType>::impl(tag::select_raw_type<T,pixel_byte>* target, int2 size, int pitch)
		: _raw(target), _raw_size(size), _pitch(pitch ? pitch : size.x() * sizeof(RawType))
		{}

		template<typename T, typename P, typename R>
		template<typename OtherTag>
		impl<T,P,R>::impl(const impl<OtherTag, P, R> & other) :
			_raw(reinterpret_cast<tag::select_raw_type<T,pixel_byte>*>(other.row())),
			_raw_size(other.raw_size()),
			_pitch(other.pitch())
		{}

		template<typename T, typename P, typename R>
		template<typename OtherTag>
		impl<T,P,R>::impl(const impl<OtherTag, P, R> & other, range2D range) :
			_raw(reinterpret_cast<tag::select_raw_type<T,pixel_byte>*>(other.row())
				+ to_index(range.lower(), other.pitch())),
			_raw_size((range.upper() - range.lower())*ratio),
			_pitch(other.pitch())
		{}

		template<typename T, typename P, typename RawType>
		int impl<T,P,RawType>::to_index(const int2& position, int pitch) noexcept
		{
			return position.x() * ratio.x() * sizeof(RawType) + position.y() * pitch;
		}

	} // namespace pixel_view_details


	template <typename P, typename R>
	pixel_writer<P,R>::pixel_writer(pixel_byte* target, int2 size, int pitch)
		: impl(target, size, pitch)
	{}

	template<typename Pixel, typename RawType>
	void pixel_writer<Pixel,RawType>::set(const Pixel& pixel, int2 position) const
	{
		assert(int2::zero() <= position && position < this->size());
		if constexpr(std::is_same_v<Pixel,RawType>)
			(*this)[position] = pixel;
		else
		{
			position.x() *= pixel_writer::ratio.x();
			memcpy(&(*this)[position], &(pixel), sizeof(Pixel));
		}
	}

	template<typename Pixel, typename R>
	template <typename ColorVector>
	void pixel_writer<Pixel,R>::set(const Pixel& pixel, float2 position) const
	{
		set(static_cast<ColorVector>(pixel), position);
	}

	template<typename Pixel, typename R>
	template <typename ColorVector>
	void pixel_writer<Pixel,R>::set(const ColorVector& pixel, float2 position) const
	{
		auto floor = int2(position);
		auto fraction = position - float2(floor);

		auto i = float2::zero();

		// 2D specific bound checking TODO: try to move this out if possible, make algorithm N dimensional
		auto begin = (floor[0] != this->size()[0] - 1) ? i.begin() : i.begin() + 1;
		auto end = (floor[1] != this->size()[1] - 1) ? i.end() : i.end() - 1;

		do
		{
			// the actual magic
			auto ratio = (float2::one() - fraction)*(float2::one() - i) + fraction * i;
			auto opacity = std::accumulate(ratio.begin(), ratio.end(), 1.f, std::multiplies{});

			// alpha blending TODO: move out
			if constexpr (ColorVector::dimensions >= 4)
				opacity *= pixel.a();
			assert(opacity >= 0.f && opacity <= 1.f);
			ColorVector old_color {this->get(floor + int2(i))};
			// TODO: consider skipping setting pixels with opacity 0(or near 0)
			// can help performace in cases where the pixel lies on a single row
			// line drawing, scan conversion
			// need to benchmark
			// this can be done at a lower level of set or in a "blender", by checking if what we set is equivalent to what we got, once alpha blending is moved out
			set(Pixel(pixel * opacity + old_color * (1 - opacity)), floor + int2(i));
		}
		while(simple::support::next_number(begin, end) != end);
	}

	template<typename Pixel, typename R>
	void pixel_writer<Pixel,R>::set(const color& pixel, int2 position) const
	{
		assert(int2::zero() <= position && position < this->size());
		static_assert( sizeof(color) >= sizeof(Pixel), "color should fill a pixel, at least" );
		position.x() *= pixel_writer::ratio.x();
		memcpy(&(*this)[position], &(pixel), sizeof(Pixel));
	}

	template <typename Pixel, typename RawType>
	pixel_reader<Pixel,RawType>::pixel_reader
	(const pixel_writer<Pixel, RawType> & other)
	: impl(other)
	{}

	template <typename Pixel, typename RawType>
	pixel_reader<Pixel,RawType>::pixel_reader
	(
		const pixel_writer<Pixel, RawType> & other,
		range2D range
	)
	: impl(other, range)
	{}


} // namespace simple::graphical

#endif /* end of include guard */
