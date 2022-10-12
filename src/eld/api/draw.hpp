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
		Line,
		Text
	};

	template<DrawPrimitive D>
	struct DrawCommand{};

	template<>
	struct DrawCommand<DrawPrimitive::Line>
	{
		Point begin;
		Point end;
		float width = 1.0f;
		/// ARGB
		unsigned int colour = 0xffffffff;
	};

	template<>
	struct DrawCommand<DrawPrimitive::Text>
	{
		Point location;
		const char* data;
		unsigned int text_size = 16;
		/// ARGB
		unsigned int colour = 0xffffffff;
	};

	using DrawCommandVariant = std::variant<DrawCommand<DrawPrimitive::Line>, DrawCommand<DrawPrimitive::Text>, DrawCommand<DrawPrimitive::Polygon>>;
}

#endif // EMERALD_SRC_API_DRAW_HPP
