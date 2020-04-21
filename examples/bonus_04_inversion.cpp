#include <cstdio>
#include <cerrno>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>

#include "simple/support/enum.hpp"
#include "simple/geom/algorithm.hpp"
#include "simple/graphical/initializer.h"
#include "simple/graphical/pixels.hpp"
#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm/blit.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

using namespace std::string_literals;
using namespace simple::graphical;
using namespace color_literals;

using rgba_pixels = pixel_writer<rgba_pixel, pixel_byte>;
using simple::support::to_integer;

float2 invert(float2 in, float2 center, float radiance)
{
	auto relative = in - center;
	return center + relative * radiance/relative.quadrance();
}

const float tau = 2*std::acos(-1);

surface stb_load_surface(const char* filename);

enum class commands
{
	help,
	invert,
	mode,
	blend,
	radius,
	center,
	pan,
	// TODO:
	// zoom,
	// size(window),
	// view(current state),
	// resolution?,
	save,
	commit,
	reset,
	timer,
	invalid
};
using command = simple::support::mapped_enum<commands, commands::invalid, 2>;
template <> command::guts::map_type command::guts::map
{{
	{ "h"s, "help"s },
	{ "i"s, "invert"s },
	{ "m"s, "mode"s },
	{ "b"s, "blend-toggle"s },
	{ "r"s, "radius"s },
	{ "c"s, "center"s },
	{ "p"s, "pan"s },
	{ "s"s, "save"s },
	{ "cmt"s, "commit"s },
	{ "reset"s, ""s },
	{ "t"s, "timer-toggle"s },
}};

enum class modes
{
	invert_floor,
	invert_round,
	invert_linear,
	outvert_floor,
	outvert_round,
	outvert_linear,
	invalid
};
using mode = simple::support::mapped_enum<modes, modes::invalid, 2>;
template <> mode::guts::map_type mode::guts::map
{{
	{ "if"s, "invert-floor"s },
	{ "ir"s, "invert-round"s },
	{ "il"s, "invert-linear"s },
	{ "of"s, "outvert-floor"s },
	{ "or"s, "outvert-round"s },
	{ "ol"s, "outvert-linear"s },
}};

constexpr auto inverter_func = std::array
{
	+[](rgba_pixels src, rgba_pixels dest, int2 from, float2 to)
	{
		const float2 size{dest.size()};
		if(float2::zero() <= to && to < size)
			dest.set(src.get(from), int2(to));
	},
	+[](rgba_pixels src, rgba_pixels dest, int2 from, float2 to)
	{
		const float2 size{dest.size()};
		if(float2::zero() <= to && to < size)
			dest.set(src.get(from), round(to));
	},
	+[](rgba_pixels src, rgba_pixels dest, int2 from, float2 to)
	{
		const float2 size{dest.size()};
		if(float2::one(-1) < to && to < size)
			dest.set(src.get(from), to);
	},
	+[](rgba_pixels src, rgba_pixels dest, int2 from, float2 to)
	{
		const float2 size{src.size()};
		if(float2::zero() <= to && to < size)
			dest.set(src.get(int2(to)), from);
	},
	+[](rgba_pixels src, rgba_pixels dest, int2 from, float2 to)
	{
		const float2 size{src.size()};
		if(float2::zero() <= to && to < size)
			dest.set(src.get(round(to)), from);
	},
	+[](rgba_pixels src, rgba_pixels dest, int2 from, float2 to)
	{
		const float2 size{src.size()};
		if(float2::one(-1) < to && to < size)
			dest.set(src.get(to), from);
	},
};

int main(int argc, char** argv) try
{

	if(argc < 2)
	{
		std::puts("Image not specified.");
		return -1;
	}

	initializer init;

	auto image = stb_load_surface(argv[1]);
	std::cout << image.size() << '\n';
	auto pixels = std::get<rgba_pixels>(image.pixels());

	float2 image_size(image.size());

	software_window win("inversion", image.size(), window::flags::borderless);

	auto inverted_image = surface(image);
	fill(inverted_image, inverted_image.format().color(0x0_rgba));
	auto inverted_pixels = std::get<rgba_pixels>(inverted_image.pixels());

	auto center = image_size/2;
	float radiance = (image_size.x() * image_size.y())/tau;

	std::string current_command;
	int2 offset{};
	mode current_mode = modes::outvert_linear;
	bool timer = false;
	do
	{
		switch(command(current_command))
		{
			case commands::help:
			{
				auto print = [](const auto& map)
				{
					for(auto&& i : map)
					{
						std::cout << '\t';
						for(auto&& j : i)
							std::cout << j << ' ';
						std::cout << '\n';
					}
				};
				std::cout << "Commands:" << '\n';
				print(command::guts::map);
				std::cout << "Modes:" << '\n';
				print(mode::guts::map);
			}
			break;

			case commands::invert:
			{
				using namespace std::chrono;
				auto start = steady_clock::now();
				fill(inverted_image, inverted_image.format().color(0x0_rgba));
				if(inverted_image.blend() != blend_mode::none)
					fill(win.surface(), win.surface().format().color(0x0_rgba));
				simple::geom::loop(int2::zero(), image.size(), int2::one(),
					[&](auto i)
				{
					auto inverted = invert(float2(i + offset),center,radiance);
					inverter_func[to_integer(current_mode)]
					(
						pixels, inverted_pixels,
						i,      inverted
					);
				});
				blit(inverted_image, win.surface());
				win.update();
				if(timer)
					std::cout
						<< duration_cast<milliseconds>(
							steady_clock::now() - start).count()
						<< "ms\n";
			}
			break;

			case commands::mode:
			{
				std::string mode_str;
				if(std::cin >> mode_str)
				{
					const auto new_mode = mode(mode_str);
					if(new_mode != modes::invalid)
						current_mode = new_mode;
					else
						std::cerr << "Invalid mode!" << '\n';
				}
				else
				{
					std::cerr << "Invalid command!" << '\n';
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			}
			break;

			case commands::blend:
				inverted_image.blend(
					inverted_image.blend() != blend_mode::none
						? blend_mode::none
						: blend_mode::alpha
				);
			break;


			case commands::radius:
			{
				float radius;
				if(std::cin >> radius)
				{
					radiance = radius * radius;
				}
				else
				{
					std::cerr << "Invalid current_command!" << '\n';
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			}
			break;

			case commands::center:
				if(!(std::cin >> center.x() >> center.y()))
				{
					std::cerr << "Invalid command!" << '\n';
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			break;

			case commands::pan:
				if(!(std::cin >> offset.x() >> offset.y()))
				{
					std::cerr << "Invalid command!" << '\n';
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			break;

			case commands::save:
			{
				std::string savename;
				if(std::cin >> savename)
					inverted_image.save((savename + ".bmp").c_str());
			}
			break;

			case commands::commit:
				blit(inverted_image, image, blend_mode::none);
			break;

			case commands::reset:
				if(image.blend() != blend_mode::none)
					fill(win.surface(), win.surface().format().color(0x0_rgba));
				blit(image, win.surface());
				win.update();
			break;

			case commands::timer:
				timer = !timer;
			break;

			case commands::invalid:
				std::cerr << "Invalid command!" << '\n';
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
	}
	while(std::cin >> current_command);

	return 0;
}
catch(...)
{
	if(errno)
		std::perror("ERROR");

	const char* sdl_error = SDL_GetError();
	if(*sdl_error)
		std::puts(sdl_error);

	throw;
}

surface stb_load_surface(const char* filename)
{
	using namespace std::literals;

	int2 size;
	int orig_format;

	auto data = stbi_load(filename, &size.x(), &size.y(), &orig_format, STBI_rgb_alpha);

	if(data == NULL)
		throw std::runtime_error("Loading image failed: "s + stbi_failure_reason());

	return
	{
		{ data, [](surface::byte* data) { stbi_image_free(data); } },
		size,
		pixel_format(pixel_format::type::rgba32)
	};
}
