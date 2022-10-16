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
	renderer.get_command_list().add<eld::DrawPrimitive::FilledPolygon>
	({
		.positions =
		{
			{0, 0},
			{1000, 0},
			{1000, 1000},
			{0, 1000}
		},
		.colour = 0xff0000ff
	});
	renderer.get_command_list().add<eld::DrawPrimitive::FilledPolygon>
	({
		.positions =
		{
			{200, 200},
			{400, 500},
			{600, 200}, 
		},
		.colour = 0xffff0000
	});

	while(!wnd.is_close_requested())
	{
		renderer.render(wnd);
		wnd.update();
	}
}
