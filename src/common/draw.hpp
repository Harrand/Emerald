#ifndef EMERALD_SRC_COMMON_DRAW_HPP
#define EMERALD_SRC_COMMON_DRAW_HPP
#include "api/draw.hpp"
#include <vector>
#include <span>

namespace eld
{
	class DrawCommandList
	{
	public:
		template<DrawPrimitive P>
		void add(const DrawCommand<P>& cmd)
		{
			this->commands.push_back({cmd});
		}

		std::span<const DrawCommandVariant> span() const {return this->commands;}
		std::span<DrawCommandVariant> span() {return this->commands;}

	private:
		std::vector<DrawCommandVariant> commands;
	};
}

#endif // EMERALD_SRC_COMMON_DRAW_HPP
