#include "eld/window.hpp"
#include "eld/context.hpp"
#include "eld/software_renderer.hpp"

int main()
{
	eld::Window wnd{eld::WindowInfo
	{
		.width = 800,
		.height = 600,
		.title = "Emerald Triangle Demo (Software Rendering)",
		.intent = eld::WindowRenderingIntent::SoftwareRendering,
	}};

	eld::SoftwareRenderer renderer;
	renderer.get_command_list().add<eld::DrawPrimitive::Polygon>
	({
		.positions =
		{
			{200, 200},
			{300, 400},
			{400, 200}, 
		},
		.width = 3.0f
	});

	while(!wnd.is_close_requested())
	{
		renderer.render(wnd);
		wnd.update();
	}
}
