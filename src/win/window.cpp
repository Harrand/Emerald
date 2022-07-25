#if ELD_WIN
#include "win/window.hpp"

namespace eld
{
	namespace win_impl
	{
		static bool wndclass_registered = false;

		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			return {};
		}
	}

	WindowWin32::WindowWin32(WindowInfo info):
	hwnd(nullptr)
	{
		// Regardless of the number of windows, a wndclass must exist. Let's make sure we only ever have exactly one.
		if(!win_impl::wndclass_registered)
		{
			WNDCLASSEX wc
			{
				.cbSize = sizeof(WNDCLASSEX),
				.style = 0,
				.lpfnWndProc = win_impl::wnd_proc,
				.cbClsExtra = 0,
				.cbWndExtra = 0,
				.hInstance = GetModuleHandle(nullptr),
				.hIcon = nullptr,
				.hCursor = nullptr,
				.hbrBackground = nullptr,
				.lpszMenuName = nullptr,
				.lpszClassName = "Emerald Window Class",
				.hIconSm = nullptr
			};
			RegisterClassEx(&wc);
			win_impl::wndclass_registered = true;
		}

		// Actually create the window now.
		this->hwnd = CreateWindowEx(
			0,
			info.title,
			"Emerald Window description TODO!",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(info.width), static_cast<int>(info.height),
			nullptr,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr
		);
		ShowWindow(this->hwnd, SW_SHOW);
	}
}

#endif // ELD_WIN
