#ifndef EMERALD_SRC_API_DRAW_HPP
#define EMERALD_SRC_API_DRAW_HPP
#include <concepts>
#include <variant>

namespace eld
{
	struct Point
	{
		int x = 0, y = 0;
	};

	enum class DrawPrimitive
	{
		Polygon,
		Line
	};

	template<DrawPrimitive D>
	struct DrawCommand{};

	template<>
	struct DrawCommand<DrawPrimitive::Line>
	{
		Point begin;
		Point end;
		float width = 1.0f;
	};

	using DrawCommandVariant = std::variant<DrawCommand<DrawPrimitive::Line>, DrawCommand<DrawPrimitive::Polygon>>;
}

#endif // EMERALD_SRC_API_DRAW_HPP
