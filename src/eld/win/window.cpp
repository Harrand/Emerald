#if ELD_WIN
#define OEMRESOURCE
#include "eld/win/window.hpp"
#include <cstddef>
#include <cstring>
#include <utility>
#include <cassert>
#include <algorithm>
#include <gdiplus.h>

namespace eld
{
	constexpr char wnd_class_name_software[] = "Emerald Window Class (Software Rendering)";
	constexpr char wnd_class_name_hardware[] = "Emerald Window Class";

	namespace win_impl
	{
		static bool wndclass_registered_software = false;
		static bool wndclass_registered_hardware = false;
		struct SoftwareRenderingInfo
		{
			Gdiplus::GdiplusStartupInput input = {};
			ULONG_PTR gdip_token = {};
			bool initialised = false;
		};

		static SoftwareRenderingInfo sw_info = {};

		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		WindowWin32* get_window(HWND hwnd)
		{
			return reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}
	}

	std::size_t WindowWin32::alive_window_count_hardware = 0;
	std::size_t WindowWin32::alive_window_count_software = 0;

	WindowWin32::WindowWin32(WindowInfo info):
	hwnd(nullptr),
	render_intent(info.intent),
	hardware_api(info.details.hardware_api)
	{
		// Regardless of the number of windows, a wndclass must exist. Let's make sure we only ever have exactly one.
		switch(info.intent)
		{
			case WindowRenderingIntent::SoftwareRendering:
				if(!win_impl::wndclass_registered_software)
				{
					WNDCLASSEXA wc
					{
						.cbSize = sizeof(WNDCLASSEXA),
						.style = CS_HREDRAW | CS_VREDRAW,
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
				{
					auto& sw_impl = win_impl::sw_info;
					if(!sw_impl.initialised)
					{
						Gdiplus::GdiplusStartup(&sw_impl.gdip_token, &sw_impl.input, nullptr);
						sw_impl.initialised = true;
					}
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
		this->on_alive();

		this->hdc = GetDC(this->hwnd);
		ShowWindow(this->hwnd, SW_SHOW);

		if(info.intent == WindowRenderingIntent::HardwareAccelerated && info.details.hardware_api == HardwareGraphicsAPI::OpenGL)
		{
			// If we're gonna use OpenGL, there is some setup to do first.
			// OpenGL moment.
			PIXELFORMATDESCRIPTOR pfd
			{
				.nSize = sizeof(PIXELFORMATDESCRIPTOR),
				.nVersion = 1,
				.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
				.iPixelType = PFD_TYPE_RGBA,
				.cColorBits = info.details.ogl_details.colour_buffer_depth,
				.cRedBits = 0,
				.cRedShift = 0,
				.cGreenBits = 0,
				.cGreenShift = 0,
				.cBlueBits = 0,
				.cBlueShift = 0,
				.cAlphaBits = 0,
				.cAlphaShift = 0,
				.cAccumBits = 0,
				.cAccumRedBits = 0,
				.cAccumGreenBits = 0,
				.cAccumBlueBits = 0,
				.cAccumAlphaBits = 0,
				.cDepthBits = info.details.ogl_details.depth_buffer_size_bits,
				.cStencilBits = info.details.ogl_details.stencil_buffer_size_bits,
				.cAuxBuffers = 0,
				.iLayerType = PFD_MAIN_PLANE,
				.bReserved = 0,
				.dwLayerMask = 0,
				.dwVisibleMask = 0,
				.dwDamageMask = 0
			};
			if(info.details.ogl_details.double_buffer)
			{
				pfd.dwFlags |= PFD_DOUBLEBUFFER;
			}
			int pfn = ChoosePixelFormat(this->get_hdc(), &pfd);
			assert(pfn != 0);
			SetPixelFormat(this->get_hdc(), pfn, &pfd);
		}
	}

	WindowWin32::WindowWin32(WindowWin32&& move):
	hwnd(move.hwnd),
	hdc(move.hdc),
	render_intent(move.render_intent),
	hardware_api(move.hardware_api),
	close_requested(move.close_requested),
	window_text(std::move(move.window_text))
	{
		move.hwnd = nullptr;
		move.window_text = "";
		SetWindowLongPtr(this->hwnd, GWLP_USERDATA, (LONG_PTR)this);
	}

	WindowWin32::~WindowWin32()
	{
		if(this->hwnd != nullptr)
		{
			this->on_death();
			this->hwnd = nullptr;
		}
	}

	WindowWin32& WindowWin32::operator==(WindowWin32&& rhs)
	{
		std::swap(this->hwnd, rhs.hwnd);
		std::swap(this->hdc, rhs.hdc);
		std::swap(this->render_intent, rhs.render_intent);
		std::swap(this->hardware_api, rhs.hardware_api);
		std::swap(this->close_requested, rhs.close_requested);
		std::swap(this->window_text, rhs.window_text);
		SetWindowLongPtr(this->hwnd, GWLP_USERDATA, (LONG_PTR)this);
		SetWindowLongPtr(rhs.hwnd, GWLP_USERDATA, (LONG_PTR)&rhs);
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

	void WindowWin32::request_close()
	{
		ReleaseDC(this->hwnd, this->hdc);
		DestroyWindow(this->hwnd);
	}

	WindowRenderingIntent WindowWin32::get_rendering_type() const
	{
		return this->render_intent;
	}

	ContextWin32 WindowWin32::get_context() const
	{
		assert(this->get_rendering_type() == WindowRenderingIntent::HardwareAccelerated && this->hardware_api == HardwareGraphicsAPI::OpenGL && "In order to retrieve an OGL context, the window must support hardware-accelerated rendering and the chosen API must be OpenGL");
		return {this->get_hdc()};
	}

	WindowWin32::NativeType WindowWin32::native() const
	{
		return this->hwnd;
	}

	DrawCommandList& WindowWin32::impl_command_list()
	{
		return this->software_draws;
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
		return this->hdc;
	}

	void WindowWin32::on_alive()
	{
		switch(this->get_rendering_type())
		{
			case WindowRenderingIntent::SoftwareRendering:
				WindowWin32::alive_window_count_software++;
			break;
			case WindowRenderingIntent::HardwareAccelerated:
				WindowWin32::alive_window_count_hardware++;
			break;
			default:
				assert(false);
			break;
		}
	}

	void WindowWin32::on_death()
	{
		switch(this->get_rendering_type())
		{
			case WindowRenderingIntent::SoftwareRendering:
				assert(WindowWin32::alive_window_count_software > 0);
				WindowWin32::alive_window_count_software--;
				if(WindowWin32::alive_window_count_software == 0)
				{
					UnregisterClassA(wnd_class_name_software, GetModuleHandle(nullptr));
					win_impl::wndclass_registered_software = false;
					Gdiplus::GdiplusShutdown(win_impl::sw_info.gdip_token);
					win_impl::sw_info.initialised = false;
				}
			break;
			case WindowRenderingIntent::HardwareAccelerated:
				assert(WindowWin32::alive_window_count_hardware > 0);
				WindowWin32::alive_window_count_hardware--;
				if(WindowWin32::alive_window_count_hardware == 0)
				{
					UnregisterClassA(wnd_class_name_hardware, GetModuleHandle(nullptr));
					win_impl::wndclass_registered_hardware = false;
				}
			break;
			default:
				assert(false);
			break;
		}
	}

	namespace win_impl
	{

		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			auto get_window = [hwnd]()
			{
				WindowWin32* wnd = win_impl::get_window(hwnd);
				assert(wnd != nullptr && "WindowWin32 userdata not setup properly. userdata was nullptr.");
				return wnd;
			};

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
					FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));

					if(get_window()->get_rendering_type() == WindowRenderingIntent::SoftwareRendering)
					{
						Gdiplus::Graphics graphics(hdc);
						// TODO: Do software rendering.
						for(const DrawCommandVariant& cmd : get_window()->impl_command_list().span())
						{
							std::visit([&graphics, &get_window](auto&& arg)
							{
								using T = std::decay_t<decltype(arg)>;
								if constexpr(std::is_same_v<T, DrawCommand<DrawPrimitive::Line>>)
								{
									auto line_colour = Gdiplus::Color::MakeARGB(arg.colour >> 8*3, arg.colour >> 8*2, arg.colour >> 8*1, arg.colour >> 8*0);
									Gdiplus::Pen pen(line_colour, arg.width);
									Gdiplus::Point points[2];
									const int h = static_cast<int>(get_window()->get_height());
									points[0] = {arg.begin.x, h - arg.begin.y};
									points[1] = {arg.end.x, h - arg.end.y};
									graphics.DrawLines(&pen, points, 2);

								}
								else if constexpr(std::is_same_v<T, DrawCommand<DrawPrimitive::Text>>)
								{
									auto text_colour = Gdiplus::Color::MakeARGB(arg.colour >> 8*3, arg.colour >> 8*2, arg.colour >> 8*1, arg.colour >> 8*0);
									auto brush = Gdiplus::SolidBrush(text_colour);
									const int h = static_cast<int>(get_window()->get_height());
									Gdiplus::FontFamily family(L"Arial");
									Gdiplus::Font font(&family, arg.text_size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
									Gdiplus::PointF point;
									point.X = static_cast<float>(arg.location.x);
									point.Y = static_cast<float>(h - arg.location.y);
									std::size_t len = std::strlen(arg.data);
									std::wstring wstr;
									wstr.resize(len);
									std::size_t wide_len = len * sizeof(wchar_t);
									MultiByteToWideChar(CP_ACP, 0, arg.data, -1, wstr.data(), len);
									graphics.DrawString(wstr.data(), len, &font, point, &brush);

								}
								else if constexpr(std::is_same_v<T, DrawCommand<DrawPrimitive::Polygon>>)
								{
									auto polygon_colour = Gdiplus::Color::MakeARGB(arg.colour >> 8*3, arg.colour >> 8*2, arg.colour >> 8*1, arg.colour >> 8*0);
									const int h = static_cast<int>(get_window()->get_height());
									Gdiplus::Pen pen(polygon_colour, arg.width);
									std::vector<Gdiplus::PointF> points;
									points.resize(arg.positions.size());
									std::transform(arg.positions.begin(), arg.positions.end(), points.begin(), [&h](const Point& p){Gdiplus::PointF f; f.X = p.x; f.Y = h - p.y; return f;});
									graphics.DrawPolygon(&pen, points.data(), points.size());
								}
								else
								{
									assert(false && "DrawCommand encountered with a primitive type that is not supported. Please submit a bug report.");
								}
							}, cmd);
						}
					}

					EndPaint(hwnd, &ps);
					return FALSE;
				}
				break;
				case WM_CLOSE:
					get_window()->request_close();
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
