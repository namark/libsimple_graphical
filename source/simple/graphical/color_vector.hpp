#ifndef SIMPLE_GRAPHICAL_COLOR_VECTOR_HPP
#define SIMPLE_GRAPHICAL_COLOR_VECTOR_HPP

#include "simple/geom/vector.hpp"
#include "simple/support/int_literals.hpp"
#include "simple/support/range.hpp"

namespace simple::graphical
{

	// TODO: this is to customize for YUV so this really needs to return a vector not just a number
	// better to support both probably
	template <typename T>
	class default_color_limits
	{
		public:
		constexpr static auto get()
		{
			if constexpr(std::is_floating_point_v<T>)
				return support::range<T>{static_cast<T>(0), static_cast<T>(1)};
			else
				return support::range<T>::limit();
		}
	};

	template<typename Type, size_t Size, typename RGBA_Order = std::make_index_sequence<Size>, typename Limits = default_color_limits<Type>,
	std::enable_if_t<Size == 3 || Size == 4>* = nullptr>
	struct color_vector : public geom::vector<Type, Size, RGBA_Order>
	{
		using base = geom::vector<Type, Size, RGBA_Order>;
		using int_type = uint32_t;
		constexpr static auto value_limits()
		{
			return Limits::get();
		}

		using base::base;

		// TODO: workaround for clang bug, that prevents inheriting the default constructor
		// https://bugs.llvm.org/show_bug.cgi?id=38673
		constexpr color_vector() = default;

		constexpr explicit color_vector(const base& other)
		: base(other)
		{}

		// TODO: account for RGBA_Order in these conversions

		template<size_t S = Size, std::enable_if_t<S == 4>* = nullptr>
		constexpr color_vector(const color_vector<Type, 3>& other)
		: color_vector(other.r(), other.g(), other.b(), value_limits().upper())
		{}

		template<size_t S = Size, std::enable_if_t<S == 3>* = nullptr>
		explicit constexpr color_vector(const color_vector<Type, 4>& other)
		: color_vector(other.r(), other.g(), other.b())
		{}

		template<typename OtherType, typename T = Type,
		std::enable_if_t<std::is_integral_v<OtherType>>* = nullptr,
		std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		explicit constexpr color_vector(const color_vector<OtherType, Size>& other)
		{
			for(size_t i = 0; i < Size; ++i)
				(*this)[i] = static_cast<Type>(other[i]) / static_cast<Type>(other.value_limits().upper());
		}

		template<typename OtherType, typename T = Type,
		std::enable_if_t<std::is_integral_v<T>>* = nullptr,
		std::enable_if_t<std::is_floating_point_v<OtherType>>* = nullptr>
		explicit constexpr color_vector(const color_vector<OtherType, Size>& other)
		{
			using std::round;
			for(size_t i = 0; i < Size; ++i)
				(*this)[i] = std::round(other[i] * static_cast<OtherType>(value_limits().upper()));
		}

		// geting around 1 user defined conversion limit here
		template<typename OtherType, size_t OtherSize, typename OtherOrder, typename T = Type, size_t S = Size,
		std::enable_if_t<!std::is_same_v<T, OtherType> && S != OtherSize>* = nullptr>
		explicit constexpr color_vector(const color_vector<OtherType, OtherSize, OtherOrder>& other)
			: color_vector(static_cast<color_vector<OtherType, Size, RGBA_Order>>(other))
		{
		}

		constexpr Type& r() { return this->x(); }
		constexpr Type& g() { return this->y(); }
		constexpr Type& b() { return this->z(); }
		constexpr Type& a() { return this->w(); }

		constexpr Type r() const { return const_cast<color_vector*>(this)->r(); };
		constexpr Type g() const { return const_cast<color_vector*>(this)->g(); };
		constexpr Type b() const { return const_cast<color_vector*>(this)->b(); };
		constexpr Type a() const { return const_cast<color_vector*>(this)->a(); };

		template<size_t S = Size, std::enable_if_t<S == 3>* = nullptr>
		constexpr static color_vector white(const Type& value = value_limits().upper())
		{
			return static_cast<color_vector>(base::one(value));
		};

		template<size_t S = Size, std::enable_if_t<S == 3>* = nullptr>
		constexpr static color_vector red(const Type& value = value_limits().upper())
		{
			return static_cast<color_vector>(base::i(value));
		};

		template<size_t S = Size, std::enable_if_t<S == 3>* = nullptr>
		constexpr static color_vector green(const Type& value = value_limits().upper())
		{
			return static_cast<color_vector>(base::j(value));
		};

		template<size_t S = Size, std::enable_if_t<S == 3>* = nullptr>
		constexpr static color_vector blue(const Type& value = value_limits().upper())
		{
			return static_cast<color_vector>(base::k(value));
		};

		template<size_t S = Size, std::enable_if_t<S == 4>* = nullptr>
		constexpr static color_vector white(const Type& value = value_limits().upper(), const Type& alpha = value_limits().upper())
		{
			return {value, value, value, alpha};
		}

		template<size_t S = Size, std::enable_if_t<S == 4>* = nullptr>
		constexpr static color_vector red(const Type& value = value_limits().upper(), const Type& alpha = value_limits().upper())
		{
			return static_cast<color_vector>(base::i(value) + base::l(alpha));
		}

		template<size_t S = Size, std::enable_if_t<S == 4>* = nullptr>
		constexpr static color_vector green(const Type& value = value_limits().upper(), const Type& alpha = value_limits().upper())
		{
			return static_cast<color_vector>(base::j(value) + base::l(alpha));
		}

		template<size_t S = Size, std::enable_if_t<S == 4>* = nullptr>
		constexpr static color_vector blue(const Type& value = value_limits().upper(), const Type& alpha = value_limits().upper())
		{
			return static_cast<color_vector>(base::k(value) + base::l(alpha));
		}

		template <int_type value>
		constexpr static color_vector from() noexcept
		{ return from(value); }

		template <typename T = Type, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
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

	// for ADL to find these
	using ::operator+;
	using ::operator-;
	using ::operator*;
	using ::operator/;
	using ::operator%;
	using ::operator&;
	using ::operator|;
	using ::operator^;
	using ::operator+=;
	using ::operator-=;
	using ::operator*=;
	using ::operator/=;
	using ::operator%=;
	using ::operator&=;
	using ::operator|=;
	using ::operator^=;

	using pixel_byte = unsigned char;

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

namespace simple
{
	template<typename C, size_t D, typename O>
	struct support::define_array_operators<graphical::color_vector<C,D,O>> :
	public support::define_array_operators<geom::vector<C,D,O>>
	{
		// revert these to default beahvior, in this context means no type promotion
		template <typename, support::array_operator, typename, bool>
		using result = graphical::color_vector<C,D,O>;
		using compatibility_tag = graphical::color_vector<C,D,O>;
	};
} // namespace simple

#endif /* end of include guard */
