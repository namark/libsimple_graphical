#include <cstdio>

#include "simple/graphical/initializer.h"
#include "simple/graphical/renderer_window.h"
#include "simple/graphical/renderer.h"
#include "simple/graphical/texture.h"
#include "simple/graphical/surface.h"
#include "simple/graphical/algorithm.h"

using namespace simple::graphical;
using namespace color_literals;

int main(int argc, char const* argv[]) try
{
	initializer init;

	renderer_window win("textures galore", {600,600}, renderer_window::flags::borderless,
			 renderer::flags::accelerated | renderer::flags::v_sync | renderer::flags::texture_target);
	auto& renderer = win.renderer();

	if(argc >= 2)
	{
		surface image(argv[1]);

		auto static_default_scaling = renderer.get_texture(image);
		renderer.render(static_default_scaling);
		renderer.update();
		SDL_Delay(1313);

		auto static_best_scaling = renderer.get_texture(image, quality_hint::best);
		renderer.render(static_best_scaling);
		renderer.update();
		SDL_Delay(1313);
	}

	auto size = int2::one() * 300;
	auto streaming = renderer.get_texture(size, quality_hint::best, pixel_format::type::abgr8888);
	renderer.texture_blend(streaming, blend_mode::alpha);

	float blue = 0;
	auto gradientor_blue = [&blue](auto&& pixels_variant) {
		auto pixels = std::get<pixel_writer<rgba_pixel, pixel_byte>>(pixels_variant);
		for(int2 p{}; p < pixels.size(); ++p.y(), p.x() = 0)
		for(; p < pixels.size(); p += int2::i())
			pixels.set( rgb_pixel(rgb_vector(
							( (float2)p /(float2)pixels.size() ).xyz(blue) )), p );
	};

	for(int angle = 0; blue <= 1.0; blue += 0.01, angle += 5)
	{
		renderer.update(streaming, gradientor_blue);
		fill(renderer, 0x0_rgb);
		renderer.render({streaming, size/2}, win.size()/2 - size/2, angle);
		renderer.update();
	}
	SDL_Delay(1313);

	auto render_tex = renderer.get_render_texture(size, quality_hint::best, pixel_format::type::rgba8888);
	renderer.texture_blend(render_tex, blend_mode::alpha);
	renderer.target(render_tex);
	renderer.render(streaming);
	renderer.update();

	renderer.target_default();
	for(int angle = 0; angle <= 2*360 + 45; angle += 5)
	{
		fill(renderer, 0x0_rgb);
		renderer.render({render_tex, size/2}, win.size()/2 - size/2, angle);
		renderer.update();
	}
	SDL_Delay(1313);

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
