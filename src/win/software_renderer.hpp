#ifndef EMERALD_SRC_WIN_SOFTWARE_RENDERER_HPP
#define EMERALD_SRC_WIN_SOFTWARE_RENDERER_HPP
#include "common/draw.hpp"
#if ELD_WIN
#include "api/software_renderer.hpp"
#include "common/draw.hpp"
#include "win/window.hpp"

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
