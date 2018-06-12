#include <cstdio>

#if ENABLE_OPENGL_EXAMPLES
#include <chrono>
#include <thread>

#include "simple/graphical/initializer.h"
#include "simple/graphical/gl_window.h"
#include "simple/graphical/algorithm.h"

#include <GL/glew.h>
#include "external/nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "external/nanovg_gl.h"

using namespace std::chrono_literals;
using namespace simple::graphical;

// based on
// https://www.khanacademy.org/computer-programming/bouncing-ball/863892852
class black_ball_sketch
{
	// time
	int t = 0;
	// how high the ball is, where 0 is on the ground
	float y = 0;

	public: void draw(NVGcontext*);
} black_ball_sketch;

int main() try
{
	initializer init;

	gl_window::global.request<gl_window::attribute::major_version>(2);
	gl_window::global.request<gl_window::attribute::stencil>(8);
	gl_window win("nanovg", point2D(400, 400), window::flags::borderless);

	glewInit();
	struct NVGcontext* vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

	const auto frame_time = 33ms;
	const auto end_at = std::chrono::high_resolution_clock::now() + 11s;
	for
	(
		auto next_frame = std::chrono::high_resolution_clock::now();
		next_frame < end_at;
		next_frame += frame_time,
		std::this_thread::sleep_until(next_frame)
	)
	{
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		nvgBeginFrame(vg, win.size().x(), win.size().y(), 1);
		black_ball_sketch.draw(vg);
		nvgEndFrame(vg);
		win.update();
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
	nvgEllipse(vg, 200, 300, shadowSize/2, 5);
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
		t = 0; // reset t to make it "bounce" up again

	t += 5;
}

#else
int main() { std::puts("Example not enabled!"); return 0; }
#endif
