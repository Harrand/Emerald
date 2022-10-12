#ifndef EMERALD_SRC_WIN_SOFTWARE_RENDERER_HPP
#define EMERALD_SRC_WIN_SOFTWARE_RENDERER_HPP
#if ELD_WIN
#include "eld/common/draw.hpp"
#include "eld/api/software_renderer.hpp"
#include "eld/common/draw.hpp"
#include "eld/win/window.hpp"

namespace eld
{
	class SoftwareRendererWin32
	{
	public:
		void render(WindowWin32& window);
		const DrawCommandList& get_command_list() const;
		DrawCommandList& get_command_list();
	private:
		DrawCommandList draws = {};
	};

	static_assert(SoftwareRendererType<SoftwareRendererWin32>);
}

#endif // ELD_WIN
#endif // EMERALD_SRC_WIN_SOFTWARE_RENDERER_HPP
