#ifndef EMERALD_SRC_WIN_CONTEXT_HPP
#define EMERALD_SRC_WIN_CONTEXT_HPP
#if ELD_WIN
#include "eld/api/context.hpp"

#include <windows.h>

namespace eld
{
	class ContextWin32
	{
	public:
		ContextWin32(HDC device_context, HGLRC ctx);

		static ContextWin32 null();
		bool is_null() const;
		bool is_headless() const;
		bool is_current() const;
		void make_current();
	private:
		ContextWin32() = default;

		HGLRC ctx = nullptr;
		HDC device_ctx = nullptr;
	};

	static_assert(ContextType<ContextWin32>);
}

#endif // ELD_WIN
#endif // EMERALD_SRC_WIN_CONTEXT_HPP
