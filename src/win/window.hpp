#ifndef EMERALD_SRC_WIN_WINDOW_HPP
#define EMERALD_SRC_WIN_WINDOW_HPP
#if ELD_WIN
#include "api/window.hpp"
#include <windows.h>

namespace eld
{
	class WindowWin32
	{
	public:
		WindowWin32(WindowInfo info);
		unsigned int get_width() const;
		unsigned int get_height() const;
		const char* get_title() const;

		void set_width(unsigned int w);
		void set_height(unsigned int h);
		void set_title(const char* title);
	private:
		HWND hwnd;
	};

	static_assert(WindowType<WindowWin32>);
}

#endif // ELD_WIN
#endif // EMERALD_SRC_WIN_WINDOW_HPP
