#include <cstdio>

#if ENABLE_OPENGL_EXAMPLES
#include <chrono>
#include <string>
#include <thread>

#include "simple/graphical/initializer.h"
#include "simple/graphical/gl_window.h"
#include "simple/support/misc.hpp"
#include "common.h"

#include "external/nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "external/nanovg_gl.h"

using namespace std::literals;

using namespace std::chrono_literals;
using namespace simple::graphical;

using simple::support::ston;

// based on
// https://www.khanacademy.org/computer-programming/bouncing-ball/863892852
class black_ball_sketch
{
	// time
	int t = 0;
	// how high the ball is, where 0 is on the ground
	float y = 0;

	public:
	void draw(NVGcontext*);
	// how many times did the animation loop
	size_t loops = 0;

} black_ball_sketch;


int main(int argc, char const* argv[]) try
{

	switch(argc)
	{
		case 0: std::puts("Command not specified??");
		case 1: std::puts("Number of jumps not specified!");
			return -1;
	}

	const char* record_prefix = argc > 2 ? argv[2] : nullptr;
	const auto jumps = ston<size_t>(argv[1]);

	initializer init;

	gl_window::global.request<gl_window::attribute::major_version>(2);
	gl_window::global.request<gl_window::attribute::stencil>(8);
	gl_window win("nanovg", int2(400, 400), window::flags::borderless);
	surface snapshot(win.size(), pixel_format(pixel_format::type::rgb24));

	glewInit();
	struct NVGcontext* vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

	const auto frame_time = 33ms;
	for
	(
		auto [frame, next_frame] = std::tuple{0, std::chrono::high_resolution_clock::now()};
		black_ball_sketch.loops < jumps;
		next_frame += frame_time,
		++frame,
		record_prefix ? void() : std::this_thread::sleep_until(next_frame)
	)
	{
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		nvgBeginFrame(vg, win.size().x(), win.size().y(), 1);
		black_ball_sketch.draw(vg);
		nvgEndFrame(vg);
		win.update();

		if(record_prefix)
		{
			gl_read_pixels(snapshot);
			snapshot.save((record_prefix + to_string(frame, 4) + ".bmp").c_str());
		}
	}

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

void black_ball_sketch::draw(NVGcontext* vg)
{
	// y follows the path of a parabola with respect to t!
	y = -0.02*t*t + 3.5*t;

	// shadow grows lighter with y
	float grey = (0.1 * y + 200);
	// shadow grows bigger with y
	float shadowSize = 0.2 * y + 50;
	// draw the shadow
	nvgBeginPath(vg);
	nvgEllipse(vg, 200, 290, shadowSize/2, 5);
	nvgFillColor(vg, nvgRGB(grey, grey, grey));
	nvgFill(vg);

	// ball grows fatter with the inverse of y
	float width = -0.1 * y + 70;
	float eye_width= -0.4*y + 100;
	// ball grows taller with y
	float height = 0.1 * y + 50;
	float eye_height= 0.1*y +3;

	// since y is a positive height, we need to flip it
	// to look right on the inverted coordinate plane
	float correctedY = 250 - y;

	// draw the ball
	nvgBeginPath(vg);
	nvgEllipse(vg, 200, correctedY, width/2, height/2);
	nvgFillColor(vg, nvgRGB(0, 0, 0));
	nvgFill(vg);
	nvgBeginPath(vg);
	nvgEllipse(vg, 185, correctedY-10, eye_width*0.2/2, eye_height/2);
	nvgEllipse(vg, 215, correctedY-10, eye_width*0.2/2, eye_height/2);
	nvgFillColor(vg, nvgRGB(250, 247, 247));
	nvgFill(vg);

	if (y < 0) // if y becomes negative, the ball has hit the ground
	{
		t = 0; // reset t to make it "bounce" up again
		++loops;
	}

	t += 5;
}

#else
int main() { std::puts("Example not enabled!"); return 0; }
#endif
