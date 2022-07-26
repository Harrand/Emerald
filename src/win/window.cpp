#if ELD_WIN
#define OEMRESOURCE
#include "win/window.hpp"
#include <utility>
#include <cassert>

namespace eld
{
	namespace win_impl
	{
		static bool wndclass_registered = false;

		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	}

	WindowWin32::WindowWin32(WindowInfo info):
	hwnd(nullptr)
	{
		// Regardless of the number of windows, a wndclass must exist. Let's make sure we only ever have exactly one.
		constexpr char wnd_class_name[] = "Emerald Window Class";
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
				.lpszClassName = wnd_class_name,
				.hIconSm = nullptr
			};
			RegisterClassEx(&wc);
			win_impl::wndclass_registered = true;
		}

		// Actually create the window now.
		this->hwnd = CreateWindowExA(
			0,
			wnd_class_name,
			info.title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(info.width), static_cast<int>(info.height),
			nullptr,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr
		);
		assert(this->hwnd != nullptr);
		ShowWindow(this->hwnd, SW_SHOW);
	}

	WindowWin32::WindowWin32(WindowWin32&& move):
	hwnd(move.hwnd),
	close_requested(move.close_requested)
	{
		move.hwnd = nullptr;
	}

	WindowWin32& WindowWin32::operator==(WindowWin32&& rhs)
	{
		std::swap(this->hwnd, rhs.hwnd);
		std::swap(this->close_requested, rhs.close_requested);
		return *this;
	}

	void WindowWin32::update()
	{
		MSG msg = this->get_message();
		if(this->is_close_requested())
		{
			return;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	bool WindowWin32::is_close_requested() const
	{
		return this->close_requested;
	}

	MSG WindowWin32::get_message()
	{
		MSG msg;
		this->close_requested = (GetMessage(&msg, this->hwnd, 0, 0) <= 0);
		return msg;
	}

	namespace win_impl
	{

		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			switch(msg)
			{
				case WM_PAINT:
				{
					// Draw with whatever colour is in the user settings.
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hwnd, &ps);
					FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));
					EndPaint(hwnd, &ps);
					return FALSE;
				}
				break;
				case WM_CLOSE:
					DestroyWindow(hwnd);
					return FALSE;
				break;
				case WM_DESTROY:
					PostQuitMessage(0);
					return FALSE;
				break;
				case WM_SETCURSOR:
				{
					if(LOWORD(lparam) == HTCLIENT)
					{
						HANDLE cursor = LoadImage(nullptr, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
						SetCursor((HCURSOR)cursor);
						return TRUE;
					}
				}
				break;
			}
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}

#endif // ELD_WIN
