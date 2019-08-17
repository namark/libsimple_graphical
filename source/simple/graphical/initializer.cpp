#include "initializer.h"

using namespace simple::graphical;

int initializer::screensaver_control::global_keep_alive_count = 0;

bool initializer::screensaver_control::global_kept_alive() noexcept
{
	return global_keep_alive_count > 0;
}

initializer::screensaver_control::screensaver_control() noexcept
{
	if(!global_kept_alive() && !SDL_IsScreenSaverEnabled())
	{
		SDL_EnableScreenSaver();
	}
}

void initializer::screensaver_control::keep_alive() noexcept
{
	if(!global_kept_alive())
	{
		SDL_DisableScreenSaver();
	}
	++global_keep_alive_count;
	++keep_alive_count;
}

void initializer::screensaver_control::release_one() noexcept
{
	if(kept_alive())
	{
		--keep_alive_count;
		--global_keep_alive_count;
		if(!global_kept_alive())
		{
			SDL_EnableScreenSaver();
		}
	}

}

void initializer::screensaver_control::release_all() noexcept
{
	if(kept_alive())
	{
		global_keep_alive_count -= keep_alive_count;
		if(!global_kept_alive())
		{
			SDL_EnableScreenSaver();
		}
		keep_alive_count = 0;
	}
}

[[nodiscard]] bool initializer::screensaver_control::kept_alive() const noexcept
{
	return keep_alive_count > 0;
}

initializer::screensaver_control::~screensaver_control() noexcept
{
	release_all();
}

initializer::initializer() : sdlcore::initializer(sdlcore::system_flag::video), screensaver()
{}
