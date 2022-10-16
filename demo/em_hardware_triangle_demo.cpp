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
		.title = "Emerald Triangle Demo (Modern OpenGL)",
		.intent = eld::WindowRenderingIntent::HardwareAccelerated,
		.details =
		{
			.hardware_api = eld::HardwareGraphicsAPI::OpenGL,
			.ogl_details =
			{
				.ogl_major_ver = 1,
				.ogl_minor_ver = 2,
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 1.0, 1.0);

	while(!wnd.is_close_requested())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.0, -1.0, 0.0);
		glVertex3f( 0.0,  1.0, 0.0);
		glVertex3f( 1.0, -1.0, 0.0);
		glEnd();
		wnd.update();
	}
}
