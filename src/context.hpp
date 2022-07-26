#ifndef EMERALD_SRC_CONTEXT_HPP
#define EMERALD_SRC_CONTEXT_HPP

#if ELD_WIN
#include "win/context.hpp"
#elif ELD_LINUX

#else
static_assert(false, "Unrecognised OS");
#endif

namespace eld
{
#if ELD_WIN
	using Context = ContextWin32;
#elif ELD_LINUX
	//using Context = ContextLinux;
#endif
}

#endif // EMERALD_SRC_CONTEXT_HPP

