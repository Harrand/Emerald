#include "window.hpp"
#include "context.hpp"
#include <cassert>

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
			.hardware_api = eld::HardwareGraphicsAPI::OpenGL
		}
	}};

	assert(eld::Context::null().is_current());

	eld::Context ctx = wnd.get_context();
	assert(!ctx.is_null());

	ctx.make_current();
	assert(ctx.is_current());
	assert(!eld::Context::null().is_current());

	while(!wnd.is_close_requested())
	{
		wnd.update();
	}
}
