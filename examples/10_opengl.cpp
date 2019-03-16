#include <cstdio>
#include <cerrno>

#if ENABLE_OPENGL_EXAMPLES

#include "simple/graphical/initializer.h"
#include "simple/graphical/gl_window.h"
#include "simple/graphical/algorithm.h"

#include <GL/gl.h>

using namespace simple::graphical;

int main() try
{
	initializer init;

	gl_window win("OpenGL", int2(640, 480), window::flags::borderless);

	glClearColor(0.7,0,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT);

	win.update();
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

#else
int main() { std::puts("Example not enabled!"); return 0; }
#endif
