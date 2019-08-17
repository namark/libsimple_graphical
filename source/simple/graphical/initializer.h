#ifndef SIMPLE_GRAPHICAL_INITIALIZER_H
#define SIMPLE_GRAPHICAL_INITIALIZER_H
#include "simple/sdlcore/initializer.h"

namespace simple::graphical
{

	class initializer : private sdlcore::initializer
	{
		private:

		class screensaver_control
		{
			public:
			screensaver_control() noexcept;
			~screensaver_control() noexcept;
			void keep_alive() noexcept;
			void release_one() noexcept;
			void release_all() noexcept;
			[[nodiscard]] bool kept_alive() const noexcept;
			[[nodiscard]] static bool global_kept_alive() noexcept;
			private:
			int keep_alive_count;
			static int global_keep_alive_count;
		};

		public:
		initializer();
		screensaver_control screensaver;
	};

} // namespace simple::graphical

#endif /* end of include guard */
