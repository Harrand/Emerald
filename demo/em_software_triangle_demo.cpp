#include "hdk/debug.hpp"
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
	auto update_commands = [&renderer, &wnd]()
	{
		renderer.get_command_list().clear();
		renderer.get_command_list().add<eld::DrawPrimitive::FilledPolygon>
		({
			.positions =
			{
				{0, 0},
				{static_cast<int>(wnd.get_width()), 0},
				{static_cast<int>(wnd.get_width()), static_cast<int>(wnd.get_height())},
				{0, static_cast<int>(wnd.get_height())}
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
	};

	while(!wnd.is_close_requested())
	{
		update_commands();
		renderer.render(wnd);
		wnd.update();
	}
}
