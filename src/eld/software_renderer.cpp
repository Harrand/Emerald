#include "eld/software_renderer.hpp"

namespace eld
{
	const DrawCommandList& SoftwareRenderer::get_command_list() const
	{
		return this->draws;
	}

	DrawCommandList& SoftwareRenderer::get_command_list()
	{
		return this->draws;
	}
}
