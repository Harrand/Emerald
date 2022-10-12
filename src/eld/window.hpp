#ifndef EMERALD_SRC_WINDOW_HPP
#define EMERALD_SRC_WINDOW_HPP

#if ELD_WIN
#include "eld/win/window.hpp"
#elif ELD_LINUX

#else
static_assert(false, "Unrecognised OS");
#endif

namespace eld
{
#if ELD_WIN
	using Window = WindowWin32;
#elif ELD_LINUX
	//using Window = WindowLinux;
#endif
}

#endif // EMERALD_SRC_WINDOW_HPP
