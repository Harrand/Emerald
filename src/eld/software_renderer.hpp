#ifndef EMERALD_SRC_SOFTWARE_RENDERER_HPP
#define EMERALD_SRC_SOFTWARE_RENDERER_HPP

#if ELD_WIN
#include "eld/win/software_renderer.hpp"
#elif ELD_LINUX

#else
static_assert(false, "Unrecognised OS");
#endif

namespace eld
{
#if ELD_WIN
	using SoftwareRenderer = SoftwareRendererWin32;
#elif ELD_LINUX
	// using SoftwareRenderer = SoftwareRendererLinux;
#endif
}

#endif // EMERALD_SRC_SOFTWARE_RENDERER_HPP
