#include "eld/window.hpp"
#include "eld/context.hpp"

int main()
{
	eld::Window wnd{eld::WindowInfo
	{
		.width = 800,
		.height = 600,
		.title = "Emerald Window Demo",
		.intent = eld::WindowRenderingIntent::HardwareAccelerated,
		.details =
		{
			.hardware_api = eld::HardwareGraphicsAPI::OpenGL,
		}
	}};

	wnd.get_context().make_current();

	while(!wnd.is_close_requested())
	{
		wnd.update();
	}
}
