#ifndef EMERALD_SRC_API_SOFTWARE_RENDERER_HPP
#define EMERALD_SRC_API_SOFTWARE_RENDERER_HPP
#include <concepts>

namespace eld
{
	template<typename T>
	concept SoftwareRendererType = requires(T t)
	{
		//{t.render()} -> std::same_as<void>;
		{true};
	};
}

#endif // EMERALD_SRC_API_SOFTWARE_RENDERER_HPP
