#ifndef SIMPLE_GRAPHICAL_COLOR_H
#define SIMPLE_GRAPHICAL_COLOR_H

#include <cstdint>
#include "simple/geom/vector.hpp"
#include "simple/support/int_literals.hpp"
#include "simple/support/range.hpp"


namespace simple::graphical
{

	template<typename Type, size_t Size, std::enable_if_t<Size == 3 || Size == 4>* = nullptr>
	struct color_vector : public geom::vector<Type, Size>
	{
		using base = geom::vector<Type, Size>;
		using int_type = uint32_t;
		constexpr static auto value_limits()
		{
			if constexpr(std::is_floating_point_v<Type>)
				return support::range<Type>{static_cast<Type>(0), static_cast<Type>(1)};
			else
				return support::range<Type>::limit();
		}

		using base::base;

		template<size_t S = Size, std::enable_if_t<S == 4>* = nullptr>
		color_vector(const color_vector<Type, 3>& other)
		: color_vector(other.r(), other.g(), other.b(), value_limits().upper())
		{}

		constexpr Type& r() { return this->x(); }
		constexpr Type& g() { return this->y(); }
		constexpr Type& b() { return this->z(); }
		constexpr Type& a() { return this->w(); }

		constexpr Type r() const { return const_cast<color_vector*>(this)->r(); };
		constexpr Type g() const { return const_cast<color_vector*>(this)->g(); };
		constexpr Type b() const { return const_cast<color_vector*>(this)->b(); };
		constexpr Type a() const { return const_cast<color_vector*>(this)->a(); };

		template<typename T = Type, size_t S = Size,
			std::enable_if_t<S == 3 && std::is_same_v<Type, T>>* = nullptr>
		constexpr static color_vector gray(T value)
		{
			return {value, value, value};
		};

		template<typename T = Type, size_t S = Size,
			std::enable_if_t<S == 4 && std::is_same_v<Type, T>>* = nullptr>
		constexpr static color_vector gray(T value, T alpha = value_limits().upper())
		{
			return {value, value, value, alpha};
		}

		template <int_type value>
		constexpr static color_vector from() noexcept
		{ return from(value); }

		constexpr static color_vector from(int_type value) noexcept
		{
			color_vector result{};
			for(size_t i = 0; i < Size; ++i)
				result[i] = (value >> (((Size-1)-i) * 8)) & 0xFF;
			return result;
		}

	};

	using rgb_pixel = color_vector<uint8_t, 3>;
	using rgba_pixel = color_vector<uint8_t, 4>;
	using rgb_vector = color_vector<float, 3>;
	using rgba_vector = color_vector<float, 4>;

	class pixel_format;

	class color
	{
		uint32_t raw;

		public:
		color() = default;
		constexpr explicit color(uint32_t raw) : raw(raw) {};
		color(const pixel_format& format, const rgb_pixel& values);
		color(const pixel_format& format, const rgba_pixel& values);
		explicit operator uint32_t() const;
	};

namespace color_literals
{

namespace details
{
	using simple::support::literals::details::parse_first;
} // namespace details

	template<char... chars>
	constexpr rgb_pixel operator"" _rgb() noexcept
	{
		constexpr auto value = details::parse_first<rgb_pixel::int_type, chars...>();
		static_assert(value <= 0xFF'FF'FF, "RGB color literal out of bounds");
		return rgb_pixel::from<value>();
	}

	template<char... chars>
	constexpr rgba_pixel operator"" _rgba() noexcept
	{
		constexpr auto value = details::parse_first<rgba_pixel::int_type, chars...>();
		return rgba_pixel::from<value>();
	}

} // namespace color_literals

} // namespace simple::graphical

#endif /* end of include guard */
