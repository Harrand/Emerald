#ifndef EMERALD_SRC_SOFTWARE_RENDERER_HPP
#define EMERALD_SRC_SOFTWARE_RENDERER_HPP
#include "eld/api/draw.hpp"
#include "eld/win/window.hpp"
#include <cassert>

namespace eld
{
	class SoftwareRenderer
	{
	public:
		void render(WindowType auto& window)
		{
			assert("Error: Attempting to use a SoftwareRenderer on a window which does not support software rendering. Did you accidentally provide a hardware-accelerated rendering intent?" && window.get_rendering_type() == WindowRenderingIntent::SoftwareRendering);
			window.impl_command_list() = this->draws;
		}
		const DrawCommandList& get_command_list() const;
		DrawCommandList& get_command_list();
	private:
		DrawCommandList draws = {};
	};
}

#endif // EMERALD_SRC_SOFTWARE_RENDERER_HPP
