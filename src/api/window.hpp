#ifndef EMERALD_SRC_API_WINDOW_HPP
#define EMERALD_SRC_API_WINDOW_HPP
#include <concepts>

namespace eld
{
	struct WindowInfo
	{
		unsigned int width;
		unsigned int height;
		const char* title;
	};

	template<typename T>
	concept WindowType = requires(T t, unsigned int uint, const char* cstr)
	{
		{t.get_width()} -> std::convertible_to<unsigned int>;
		{t.get_height()} -> std::convertible_to<unsigned int>;
		{t.get_title()} -> std::same_as<const char*>;

		{t.set_width(uint)} -> std::same_as<void>;
		{t.set_height(uint)} -> std::same_as<void>;
		{t.set_title(cstr)} -> std::same_as<void>;
	};
}

#endif // EMERALD_SRC_API_WINDOW_HPP
