#ifndef SIMPLE_GRAPHICAL_LINE_HPP
#define SIMPLE_GRAPHICAL_LINE_HPP

#include "simple/support/rational.hpp"

namespace simple::graphical
{

	using simple::support::rational;

	template<typename Vector>
	struct line
	{
		simple::support::array<Vector, 2> vertex;
		Vector& begin() { return vertex[0]; }
		Vector& end() { return vertex[1]; }
		const Vector& begin() const { return vertex[0]; }
		const Vector& end() const { return vertex[1]; }
	};

	// + looks good with square pixels
	// - slow?
	// - two dimensional
	template <typename Float2, typename Setter>
	line<Float2> dda_line(line<Float2> l, const Setter& setter)
	{
		// TODO: for shorter than one pixel lines, just return them as a tail;
		// if possible using existing calculation without a separate conditional,
		// that might as well go outside the function
		auto diff = l.end() - l.begin();

		int x = Float2::x_index;
		int y = Float2::y_index;

		if(abs(diff[y]) > abs(diff[x]))
			std::swap(x, y);

		if(l.begin()[x] > l.end()[x])
		{
			std::swap(l.begin(), l.end());
			diff = -diff;
		}

		const auto k = diff[y]/diff[x];
		auto last = l.begin();
		for(; l.begin()[x] < l.end()[x]; l.begin()[x] += 1, l.begin()[y] += k)
		{
			setter(l.begin());
			last = l.begin();
		}
		return {last, l.end()};
	}

	// + generic: dimension agnostic, pixel distance adjustable
	// - looks bad with square pixels, ideally should set a radius
	// - slow?
	template <typename Vector, typename Distance, typename Setter>
	line<Vector> vector_line(line<Vector> l, Distance pixel_distance, const Setter& setter)
	{
		// TODO: for shorter than one pixel lines, just return them as a tail;
		// if possible using existing calculation without a separate conditional,
		// that might as well go outside the function
		const auto diff = l.end() - l.begin();
		const auto unit = diff/diff.length() * pixel_distance;
		const auto abs_diff_0 = abs(diff[0]);
		auto last = l.begin();
		for(auto i = Vector{}; abs(i[0]) < abs_diff_0; i += unit)
		{
			last = l.begin() + i;
			setter(last);
		}
		return {last, l.end()};
	};

	template <typename Vector, typename Setter>
	line<Vector> vector_line(line<Vector> l, const Setter& setter)
	{
		return vector_line(l, 1, setter);
	};

	// + fast(?), integer arithmetic only
	// - only integer endpoint positions
	// - two dimensional
	template <typename Int2, typename Setter>
	constexpr void bresenham_line(line<Int2> l, const Setter& setter)
	{
		auto diff = l.end() - l.begin();
		auto abs_diff = abs(diff);

		using Int = typename Int2::coordinate_type;

		Int x = Int2::x_index;
		Int y = Int2::y_index;
		if(abs_diff[y] > abs_diff[x])
			std::swap(x, y);

		if(l.begin()[x] > l.end()[x])
		{
			std::swap(l.begin(), l.end());
			diff = -diff;
		}

		Int direction = diff[y] > 0 ? 1 : -1;
		Int d = -abs_diff[x]; // = -0.5 * 2*dx, midpoint's vertical distance from line, scaled by 2*dx
		while(l.begin()[x] <= l.end()[x])
		{
			if constexpr (std::is_invocable_v<Setter&, Int2&>) // bresenham, just set the pixel
			{
				setter(l.begin());
			}
			else // xiaolin wu, smear the pixel into two
			{
				const Int smear_direction = d < -abs_diff[x] ? -1 : 1;
				const Int ratio = smear_direction * (abs_diff[x] + d);

				if constexpr (std::is_invocable_v<Setter&, Int2, rational<Int>> && std::is_invocable_v<Setter&, Int2&, rational<Int>>)
					// simple callback: you get a position and intensity ratio
				{
					setter(l.begin(), rational{2*abs_diff[x] - ratio, 2*abs_diff[x]});
					setter(l.begin() + Int2::unit(y, smear_direction * direction), rational{ratio, 2*abs_diff[x]});
				}
				else if constexpr (std::is_invocable_v<Setter&, Int2&, Int2, rational<Int>>)
					// knowhow callback: you get 2 position and intensity ratio for the second one
				{
					setter(l.begin(), l.begin() + Int2::unit(y, smear_direction * direction), rational{ratio, 2*abs_diff[x]});
				}
			}

			++l.begin()[x];
			d += 2*abs_diff[y]; // += dy/dx, line continues, distance increases by slope, scaled by 2*dx
			if(d > 0) // zero comparison is not affected by the scaling
			{
				l.begin()[y] += direction;
				d -= 2*abs_diff[x]; // -= 1, midpoint goes up by one, so distance decreases, scaled by 2*dx
			}
		}

		// // dark magic version form internets, looks appealingly compact, but
		// // more comparisons in the loop and a small loss of precision when dividing by two,
		// // not sure if any of that would make a difference though
		// Int& x0 = l.begin().x(), y0 = l.begin().y(), x1 = l.end().x(), y1 = l.end().y();
		// Int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		// Int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
		// Int err = (dx>dy ? dx : -dy)/2, e2;
		//
		// for(;;){
		// 	setter(Int2{x0,y0}, Int2{dx,dy}, err);
		// 	if (x0==x1 && y0==y1) break;
		// 	e2 = err;
		// 	if (e2 >-dx) { err -= dy; x0 += sx; }
		// 	if (e2 < dy) { err += dx; y0 += sy; }
		// }

	}

} // namespace simple::graphical

#endif /* end of include guard */
