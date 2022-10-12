#include "window.hpp"
#include "context.hpp"
#include "software_renderer.hpp"

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
	renderer.get_command_list().add<eld::DrawPrimitive::Line>
	({
		.begin = {200, 200},
		.end = {300, 400},
		.width = 3.0f
	});
	renderer.get_command_list().add<eld::DrawPrimitive::Line>
	({
		.begin = {300, 400},
		.end = {400, 200},
		.width = 3.0f,
	});
	renderer.get_command_list().add<eld::DrawPrimitive::Line>
	({
		.begin = {400, 200},
		.end = {200, 200},
		.width = 3.0f
	});
	renderer.get_command_list().add<eld::DrawPrimitive::Text>
	({
		.location = {420, 300},
		.data = "hello, world",
		.text_size = 64
	});

	while(!wnd.is_close_requested())
	{
		renderer.render(wnd);
		wnd.update();
	}
}
