#ifndef EMERALD_SRC_API_CONTEXT_HPP
#define EMERALD_SRC_API_CONTEXT_HPP
#include <concepts>

namespace eld
{
	template<typename T>
	concept ContextType = requires(T t)
	{
		{T::null()} -> std::same_as<T>;
		{t.is_null()} -> std::same_as<bool>;
		{t.is_headless()} -> std::same_as<bool>;
		{t.is_current()} -> std::same_as<bool>;
		{t.make_current()} -> std::same_as<void>;
	};
}

#endif // EMERALD_SRC_API_CONTEXT_HPP
