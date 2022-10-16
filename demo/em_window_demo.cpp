#include "eld/window.hpp"
#include "eld/context.hpp"
#include <cassert>

#if ELD_WIN
	#include <gl/GL.h>
#elif ELD_LINUX

#endif

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
			.ogl_details =
			{
				.ogl_major_ver = 4,
				.ogl_minor_ver = 5,
				.debug_context = true,
			}
		}
	}};

	assert(eld::Context::null().is_current());

	eld::Context ctx = wnd.get_context();
	assert(!ctx.is_null());

	ctx.make_current();
	std::printf("OGL Version: %s", glGetString(GL_VERSION));
	assert(ctx.is_current());
	assert(!eld::Context::null().is_current());

	while(!wnd.is_close_requested())
	{
		wnd.update();
	}
}
