#include "window.hpp"

int main()
{
	eld::Window wnd{eld::WindowInfo
	{
		.width = 800,
		.height = 600,
		.title = "Emerald Window Demo"
	}};
	while(!wnd.is_close_requested())
	{
		wnd.update();
	}
}
