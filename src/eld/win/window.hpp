#ifndef EMERALD_SRC_WIN_WINDOW_HPP
#define EMERALD_SRC_WIN_WINDOW_HPP
#if ELD_WIN
#include "eld/api/window.hpp"
#include "eld/api/draw.hpp"
#include "eld/win/context.hpp"

#include <windows.h>

#include <utility>
#include <string>

namespace eld
{
	class WindowWin32
	{
	public:
		static std::size_t alive_window_count_hardware;
		static std::size_t alive_window_count_software;
		WindowWin32(WindowInfo info);
		WindowWin32(const WindowWin32& copy) = delete;
		WindowWin32(WindowWin32&& move);
		~WindowWin32();
		WindowWin32& operator==(const WindowWin32& rhs) = delete;
		WindowWin32& operator==(WindowWin32&& rhs);

		unsigned int get_width() const;
		void set_width(unsigned int w);
		unsigned int get_height() const;
		void set_height(unsigned int h);
		const char* get_title() const;
		void set_title(const char* title);

		void update();
		bool is_close_requested() const;
		void request_close();
		WindowRenderingIntent get_rendering_type() const;
		ContextWin32 get_context() const;
		using NativeType = HWND;
		NativeType native() const;

		DrawCommandList& impl_command_list();
	private:
		std::pair<int, int> get_window_size() const;
		MSG get_message();
		HDC get_hdc() const;
		void on_alive();
		void on_death();

		HWND hwnd;
		HDC hdc;
		HGLRC modern_ogl_context = nullptr;
		WindowRenderingIntent render_intent;
		HardwareGraphicsAPI hardware_api;
		bool close_requested = false;
		mutable std::string window_text = "";
		DrawCommandList software_draws = {};
		void* headless = nullptr;
	};

	static_assert(WindowType<WindowWin32>);
}

#endif // ELD_WIN
#endif // EMERALD_SRC_WIN_WINDOW_HPP
