#if ELD_WIN
#include "api/window.hpp"
#include "win/software_renderer.hpp"
#include <cstdio>
#include <cassert>

namespace eld
{
	void SoftwareRendererWin32::render(WindowWin32& window)
	{
		assert("Error: Attempting to use a SoftwareRenderer on a window which does not support software rendering. Did you accidentally provide a hardware-accelerated rendering intent?" && window.get_rendering_type() == WindowRenderingIntent::SoftwareRendering);
		window.impl_command_list() = this->draws;
	}

	const DrawCommandList& SoftwareRendererWin32::get_command_list() const
	{
		return this->draws;
	}

	DrawCommandList& SoftwareRendererWin32::get_command_list()
	{
		return this->draws;
	}
}

#endif // ELD_WIN
