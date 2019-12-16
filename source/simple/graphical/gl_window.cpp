#include "gl_window.h"

using namespace simple::graphical;

using attribute = gl_window::attribute;

SDL_GLattr gl_window::to_sdl_glatrr(attribute attr)
{
	switch(attr)
	{
		case attribute::red :                      return SDL_GL_RED_SIZE;
		case attribute::green :                    return SDL_GL_GREEN_SIZE;
		case attribute::blue :                     return SDL_GL_BLUE_SIZE;
		case attribute::alpha :                    return SDL_GL_ALPHA_SIZE;
		case attribute::buffer :                   return SDL_GL_BUFFER_SIZE;
		case attribute::double_buffer :            return SDL_GL_DOUBLEBUFFER;
		case attribute::depth :                    return SDL_GL_DEPTH_SIZE;
		case attribute::stencil :                  return SDL_GL_STENCIL_SIZE;
		case attribute::accum_red :                return SDL_GL_ACCUM_RED_SIZE;
		case attribute::accum_green :              return SDL_GL_ACCUM_GREEN_SIZE;
		case attribute::accum_blue :               return SDL_GL_ACCUM_BLUE_SIZE;
		case attribute::accum_alpha :              return SDL_GL_ACCUM_ALPHA_SIZE;
		case attribute::stereo :                   return SDL_GL_STEREO;
		case attribute::msaa_buffers :             return SDL_GL_MULTISAMPLEBUFFERS;
		case attribute::msaa_samples :             return SDL_GL_MULTISAMPLESAMPLES;
		case attribute::force_acceleration :       return SDL_GL_ACCELERATED_VISUAL;
		case attribute::major_version :            return SDL_GL_CONTEXT_MAJOR_VERSION;
		case attribute::minor_version :            return SDL_GL_CONTEXT_MINOR_VERSION;
		case attribute::context_flags :            return SDL_GL_CONTEXT_FLAGS;
		case attribute::context_profile :          return SDL_GL_CONTEXT_PROFILE_MASK;
		case attribute::share_context :            return SDL_GL_SHARE_WITH_CURRENT_CONTEXT;

#if SDL_VERSION_ATLEAST(2,0,1)
		case attribute::srgb_framebuffer :         return SDL_GL_FRAMEBUFFER_SRGB_CAPABLE;
#endif

#if SDL_VERSION_ATLEAST(2,0,4)
		case attribute::release_context :          return SDL_GL_CONTEXT_RELEASE_BEHAVIOR;
#endif
	}
	throw std::logic_error("simple::graphical::gl_window::attrribute invalid!");
}

void gl_window::config::register_window() noexcept
{
	++active_windows;
}

void gl_window::config::unregister_window() noexcept
{
	--active_windows;
}

gl_window::config gl_window::global = config{};

gl_window::gl_window(std::string title, int2 size, flags windowflags, int2 position)
	: window(title, size, windowflags | window::flags::opengl, position),
	context(SDL_GL_CreateContext(guts().get()), SDL_GL_DeleteContext)
{
	global.register_window();
	global.ensure_requirements();
}

gl_window::~gl_window() noexcept
{
	global.unregister_window();
}

void gl_window::update() noexcept
{
	SDL_GL_SwapWindow(guts().get());
}

auto gl_window::vsync() const noexcept -> vsync_mode
{
	return static_cast<vsync_mode>(SDL_GL_GetSwapInterval());
}

void gl_window::require_vsync(vsync_mode mode)
{
	sdlcore::utils::throw_error(
		SDL_GL_SetSwapInterval(support::to_integer(mode)) );
}

bool gl_window::request_vsync(vsync_mode mode) noexcept
{
	return !sdlcore::utils::check_error(
		SDL_GL_SetSwapInterval(support::to_integer(mode)) );
}

int2 gl_window::framebuffer_size() const noexcept
{
	int2 size;
	SDL_GL_GetDrawableSize(guts().get(), &size.x(), &size.y());
	return size;
}

