#ifndef SIMPLE_GRAPHICAL_INITIALIZER_H
#define SIMPLE_GRAPHICAL_INITIALIZER_H
#include "simple/sdlcore/initializer.h"

namespace simple::graphical
{

	class initializer : private sdlcore::initializer
	{
		public:
		initializer();
	};

} // namespace simple::graphical

#endif /* end of include guard */
