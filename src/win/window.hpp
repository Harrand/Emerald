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
		WindowWin32(const WindowWin32& copy) = delete;
		WindowWin32(WindowWin32&& move);
		~WindowWin32() = default;
		WindowWin32& operator==(const WindowWin32& rhs) = delete;
		WindowWin32& operator==(WindowWin32&& rhs);

		unsigned int get_width() const;
		unsigned int get_height() const;
		const char* get_title() const;

		void update();
		bool is_close_requested() const;

		void set_width(unsigned int w);
		void set_height(unsigned int h);
		void set_title(const char* title);
	private:
		MSG get_message();
		HWND hwnd;
		bool close_requested = false;
	};

	static_assert(WindowType<WindowWin32>);
}

#endif // ELD_WIN
#endif // EMERALD_SRC_WIN_WINDOW_HPP
