#include "api/software_renderer.hpp"
#include "window.hpp"
#include "context.hpp"
#include "software_renderer.hpp"
#include <cassert>

int main()
{
	eld::Window wnd{eld::WindowInfo
	{
		.width = 800,
		.height = 600,
		.title = "Emerald Window Demo",
		.intent = eld::WindowRenderingIntent::SoftwareRendering,
	}};

	eld::SoftwareRenderer renderer;
	renderer.get_command_list().add<eld::DrawPrimitive::Line>
	({
		.begin = {0, 0},
		.end = {1000, 150},
	});

	while(!wnd.is_close_requested())
	{
		renderer.render(wnd);
		wnd.update();
	}
}
