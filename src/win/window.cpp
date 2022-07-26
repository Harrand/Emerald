#if ELD_WIN
#define OEMRESOURCE
#include "win/window.hpp"
#include <utility>
#include <cassert>

namespace eld
{
	namespace win_impl
	{
		static bool wndclass_registered_software = false;
		static bool wndclass_registered_hardware = false;

		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		WindowWin32* get_window(HWND hwnd)
		{
			return reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}
	}

	WindowWin32::WindowWin32(WindowInfo info):
	hwnd(nullptr),
	render_intent(info.intent)
	{
		// Regardless of the number of windows, a wndclass must exist. Let's make sure we only ever have exactly one.
		constexpr char wnd_class_name_software[] = "Emerald Window Class (Software Rendering)";
		constexpr char wnd_class_name_hardware[] = "Emerald Window Class";
		switch(info.intent)
		{
			case WindowRenderingIntent::SoftwareRendering:
				if(!win_impl::wndclass_registered_software)
				{
					WNDCLASSEXA wc
					{
						.cbSize = sizeof(WNDCLASSEXA),
						.style = 0,
						.lpfnWndProc = win_impl::wnd_proc,
						.cbClsExtra = 0,
						.cbWndExtra = 0,
						.hInstance = GetModuleHandle(nullptr),
						.hIcon = nullptr,
						.hCursor = nullptr,
						.hbrBackground = nullptr,
						.lpszMenuName = nullptr,
						.lpszClassName = wnd_class_name_software,
						.hIconSm = nullptr
					};
					RegisterClassExA(&wc);
					win_impl::wndclass_registered_software = true;
				}
			break;
			case WindowRenderingIntent::HardwareAccelerated:
				if(!win_impl::wndclass_registered_hardware)
				{
					WNDCLASSEXA wc
					{
						.cbSize = sizeof(WNDCLASSEXA),
						.style = CS_OWNDC,
						.lpfnWndProc = win_impl::wnd_proc,
						.cbClsExtra = 0,
						.cbWndExtra = 0,
						.hInstance = GetModuleHandle(nullptr),
						.hIcon = nullptr,
						.hCursor = nullptr,
						.hbrBackground = nullptr,
						.lpszMenuName = nullptr,
						.lpszClassName = wnd_class_name_hardware,
						.hIconSm = nullptr
					};
					RegisterClassExA(&wc);
					win_impl::wndclass_registered_hardware = true;
				}
			break;
			default:
				assert(false && "Unrecognised WindowRenderingIntent");
			break;
		}

		// Actually create the window now.
		this->hwnd = CreateWindowExA(
			0,
			(info.intent == WindowRenderingIntent::SoftwareRendering) ? wnd_class_name_software : wnd_class_name_hardware,
			info.title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(info.width), static_cast<int>(info.height),
			nullptr,
			nullptr,
			GetModuleHandle(nullptr),
			this
		);
		assert(this->hwnd != nullptr);
		ShowWindow(this->hwnd, SW_SHOW);
	}

	WindowWin32::WindowWin32(WindowWin32&& move):
	hwnd(move.hwnd),
	render_intent(move.render_intent),
	close_requested(move.close_requested),
	window_text(std::move(move.window_text))
	{
		move.hwnd = nullptr;
		move.window_text = "";
	}

	WindowWin32& WindowWin32::operator==(WindowWin32&& rhs)
	{
		std::swap(this->hwnd, rhs.hwnd);
		std::swap(this->render_intent, rhs.render_intent);
		std::swap(this->close_requested, rhs.close_requested);
		std::swap(this->window_text, rhs.window_text);
		return *this;
	}

	unsigned int WindowWin32::get_width() const
	{
		return static_cast<unsigned int>(this->get_window_size().first);
	}

	void WindowWin32::set_width(unsigned int w)
	{
		bool res = SetWindowPos(this->hwnd, nullptr, 0, 0, static_cast<int>(w), this->get_window_size().second, SWP_NOMOVE);
		assert(res);
	}

	unsigned int WindowWin32::get_height() const
	{
		return static_cast<unsigned int>(this->get_window_size().second);
	}

	void WindowWin32::set_height(unsigned int h)
	{
		bool res = SetWindowPos(this->hwnd, nullptr, 0, 0, this->get_window_size().first, static_cast<int>(h), SWP_NOMOVE);
		assert(res);
	}

	const char* WindowWin32::get_title() const
	{
		this->window_text.resize(GetWindowTextLengthA(this->hwnd) + 1);
		GetWindowTextA(this->hwnd, this->window_text.data(), this->window_text.size());
		return this->window_text.c_str();
	}

	void WindowWin32::set_title(const char* title)
	{
		this->window_text = title;
		SetWindowTextA(this->hwnd, title);
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

	WindowRenderingIntent WindowWin32::get_rendering_type() const
	{
		return this->render_intent;
	}

	ContextWin32 WindowWin32::get_context() const
	{
		assert(this->get_rendering_type() == WindowRenderingIntent::HardwareAccelerated && "In order to retrieve a context, the window must support hardware-accelerated rendering.");
		return {this->get_hdc()};
	}

	std::pair<int, int> WindowWin32::get_window_size() const
	{
		RECT rect;
		if(GetWindowRect(hwnd, &rect))
		{
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			return {width, height};
		}
		return {-1, -1};
	}

	MSG WindowWin32::get_message()
	{
		MSG msg;
		this->close_requested = (GetMessage(&msg, this->hwnd, 0, 0) <= 0);
		return msg;
	}

	HDC WindowWin32::get_hdc() const
	{
		return GetDC(this->hwnd);
	}

	namespace win_impl
	{

		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			switch(msg)
			{
				case WM_CREATE:
				{
					// Get the WindowWin32 and set it as the userdata for its HWND.
					CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lparam);
					WindowWin32* wnd = reinterpret_cast<WindowWin32*>(create->lpCreateParams);
					SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);

				}
				break;
				case WM_PAINT:
				{
					// Draw with whatever colour is in the user settings.
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hwnd, &ps);

					WindowWin32* wnd = win_impl::get_window(hwnd);
					assert(wnd != nullptr && "WindowWin32 userdata not setup properly. userdata was nullptr.");
					if(wnd->get_rendering_type() == WindowRenderingIntent::SoftwareRendering)
					{
						// TODO: Do software rendering.
					}

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
