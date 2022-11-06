#include "eld/window.hpp"
#include "eld/context.hpp"
#include <cassert>
#include <wingdi.h>

void* GetAnyGLFuncAddress(const char* name);

#if ELD_WIN
	#include <gl/GL.h>
	#include <gl/glext.h>

	void *GetAnyGLFuncAddress(const char *name)
	{
	  void *p = (void *)wglGetProcAddress(name);
	  if(p == 0 ||
	    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
	    (p == (void*)-1) )
	  {
	    HMODULE module = LoadLibraryA("opengl32.dll");
	    p = (void *)GetProcAddress(module, name);
	  }

	  return p;
	}

#elif ELD_LINUX

#endif

PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

void load_ogl_funcs()
{
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
}


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
				.ogl_major_ver = 4,
				.ogl_minor_ver = 5,
				.debug_context = true,
			}
		},
		.headless = true
	}};

	assert(eld::Context::null().is_current());

	eld::Context ctx = wnd.get_context();
	assert(!ctx.is_null());

	ctx.make_current();
	std::printf("OGL Version: %s", glGetString(GL_VERSION));
	assert(ctx.is_current());
	assert(!eld::Context::null().is_current());
	
	// OGL 4.5 stuff.
	load_ogl_funcs();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const GLchar* vtx_src = R"-(
		#version 450 core
		void main()
		{
 			const vec3 verts[3] = vec3[](
				vec3(-0.5, -0.5, 0.0), vec3(0.5, -0.5, 0.0), vec3(0.0f, 0.5, 0.0)
			);
			gl_Position = vec4(verts[gl_VertexID], 1.0);

		}
	)-";
	const GLchar* frg_src = R"-(
		#version 450 core
		layout(location = 0) out vec4 frag_colour;
		void main()
		{
			frag_colour = vec4(1.0, 0.0, 0.0, 1.0);
		}
	)-";

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vtx_src, nullptr);
	glCompileShader(vertex_shader);
	int success_v;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success_v);
	assert(success_v && "Failed to compile vertex shader");


	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &frg_src, nullptr);
	glCompileShader(fragment_shader);
	int success_f;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success_f);
	assert(success_f && "Failed to compile fragment shader");

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	int success_p;
	glGetProgramiv(program, GL_LINK_STATUS, &success_p);
	assert(success_p && "Failed to link shader program");
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success_p);
	assert(success_p && "Failed to validate shader program");

	glClearColor(0.0, 0.0, 1.0, 1.0);

	while(!wnd.is_close_requested())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		wnd.update();
	}

	glDeleteVertexArrays(1, &vao);
}
