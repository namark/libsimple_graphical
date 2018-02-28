#include <cstdio>
#include <random>
#include <string>
#include <optional>
#include <functional>

#include "simple/graphical/software_window.h"
#include "simple/graphical/algorithm.h"

#include "external/file.hpp"
#include "external/picopng.h"
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

using namespace simple::graphical;

// stb adapter
surface stb_load_surface(const char* filename);

// picopng adapter
auto picopng_load_image(const char* filename)
-> std::optional<std::pair<std::vector<unsigned char>, point2D>>;

void show_image(const surface& image);

auto random_byte = std::bind(
		std::uniform_int_distribution<surface::byte>(),
		std::default_random_engine(std::random_device{}()) );

int main(int argc, char const* argv[]) try
{
	if(argc < 2)
	{
		std::puts("Image not specified.");
		return -1;
	}

	SDL_Init(SDL_INIT_EVERYTHING);
	{

		// Usage of a unique pointer based adapter is very simple.
		surface stb_image(stb_load_surface(argv[1]));
		show_image(stb_image);

		// However the situation is different with owning containers, such as std::vector in picopng.
		auto picopng_image = picopng_load_image(argv[1]);
		if(picopng_image)
		{

			// You can use the pixel data as is, making sure the actual owner will outlive any such usage,
			surface image(
					picopng_image->first.data(),
					picopng_image->second,
					pixel_format(pixel_format::type::rgba8888));
			show_image(image);

			// or copy the data.
			auto image_data = picopng_image->first;
			auto image_data_copy = std::make_unique<surface::byte[]>(image_data.size());
			std::copy(image_data.begin(), image_data.end(), image_data_copy.get());
			surface picopng_image_copy (
					std::move(image_data_copy),
					picopng_image->second,
					pixel_format(pixel_format::type::rgba8888));
			show_image(picopng_image_copy);

		}
		else
			std::puts("Picopng failed"); // Picopng is expected to fail for formats other than png, that stb can handle.

		// Another possibility is, if the library can extract image information(format, size)
		// without loading it, and allows loading to arbitrary preallocated storage.
		// Lodepng (full version of picopng) might be an example of that kind of API, but not sure.
		// Here I just generate some random pixels as a demo.

		int api_width = stb_image.size().x();
		int api_height = stb_image.size().y();
		pixel_format format(pixel_format::type::rgb24);

		// Create a unique pointer
		auto data_size = api_width * api_height * format.bytes();
		auto image_data = std::make_unique<surface::byte[]>(data_size);
		std::generate(image_data.get(), image_data.get() + data_size, random_byte);

		// and pass it on,
		surface random_image(std::move(image_data), point2D(api_width, api_height), format);
		show_image(random_image);

		// or create a surface and populate it in place.
		surface random_image2(point2D(api_width, api_height), format);
		auto pixels = std::get<pixel_writer<rgb_pixel, surface::byte>>(random_image2.pixels());
		// Note that we have to do this row by row, since SDL might pad the pixels, which might or (more likely) might not be something that an image loading library supports
		surface::byte* row = pixels.row();
		for(int rows = pixels.raw_size().y(); rows --> 0; )
		{
			std::generate(row, row + pixels.raw_size().x(), std::ref(random_byte));
			row = pixels.next_row(row);
		}
		show_image(random_image2);

	}
	SDL_Quit();

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

	point2D size;
	int orig_format;

	auto data = stbi_load(filename, &size.x(), &size.y(), &orig_format, STBI_rgb_alpha);

	if(data == NULL)
		throw std::runtime_error("Loading image failed: "s + stbi_failure_reason());

	return
	{
		{ data, [](surface::byte* data) { stbi_image_free(data); } },
		size,
		pixel_format(pixel_format::type::rgba8888)
	};
}

auto picopng_load_image(const char* filename)
-> std::optional<std::pair<std::vector<unsigned char>, point2D>>
{
	auto png = simple::file::dump( simple::file::bropex(filename) );

	std::vector<surface::byte> image_data;
	simple::geom::vector<unsigned long> image_size;
	int picopng_result = decodePNG(
			image_data, image_size.x(), image_size.y(),
			reinterpret_cast<unsigned char*>(png.data()), png.size());

	return 0 == picopng_result
		? std::make_optional(std::make_pair(std::move(image_data), point2D(image_size)))
		: std::nullopt;
}

void show_image(const surface& image)
{
	software_window win("Image view", image.size(), window::flags::borderless);
	blit(image, win.surface());
	win.update();
	SDL_Delay(1313);
}

