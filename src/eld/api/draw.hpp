#ifndef EMERALD_SRC_API_DRAW_HPP
#define EMERALD_SRC_API_DRAW_HPP
#include <concepts>
#include <variant>
#include <vector>
#include <span>

namespace eld
{
	struct Point
	{
		int x = 0, y = 0;
	};

	enum class DrawPrimitive
	{
		Line,
		Text,
		Polygon,
		FilledPolygon,
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

	template<>
	struct DrawCommand<DrawPrimitive::Polygon>
	{
		std::vector<Point> positions;
		float width = 1.0f;
		unsigned int colour = 0xffffffff;
	};

	template<>
	struct DrawCommand<DrawPrimitive::FilledPolygon>
	{
		std::vector<Point> positions;
		unsigned int colour;
	};

	using DrawCommandVariant = std::variant<DrawCommand<DrawPrimitive::Line>, DrawCommand<DrawPrimitive::Text>, DrawCommand<DrawPrimitive::Polygon>, DrawCommand<DrawPrimitive::FilledPolygon>>;

	class DrawCommandList
	{
	public:
		template<DrawPrimitive P>
		void add(const DrawCommand<P>& cmd)
		{
			this->commands.push_back({cmd});
		}
		void clear()
		{
			this->commands.clear();
		}

		std::span<const DrawCommandVariant> span() const {return this->commands;}
		std::span<DrawCommandVariant> span() {return this->commands;}

	private:
		std::vector<DrawCommandVariant> commands;
	};
}

#endif // EMERALD_SRC_API_DRAW_HPP
