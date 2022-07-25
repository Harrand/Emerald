#ifndef EMERALD_SRC_API_WINDOW_HPP
#define EMERALD_SRC_API_WINDOW_HPP
#include <concepts>

namespace eld
{
	/**
	 * Specifies creation flags for a window.
	 */
	struct WindowInfo
	{
		/// Width of the window, in pixels.
		unsigned int width;
		/// Height of the window, in pixels.
		unsigned int height;
		/// Title of the window. Lifetime needs to be valid until window constructor has completed.
		const char* title;
	};

	/**
	 * Named requirements for an @ref eld::Window
	 */
	template<typename T>
	concept WindowType = requires(T t, unsigned int uint, const char* cstr)
	{
		/**
		 * Retrieve the current width of the window, in pixels.
		 */
		{t.get_width()} -> std::convertible_to<unsigned int>;
		/**
		 * Retrieve the current height of the window, in pixels.
		 */
		{t.get_height()} -> std::convertible_to<unsigned int>;
		/**
		 * Retrieve the current title of the window.
		 */
		{t.get_title()} -> std::same_as<const char*>;

		/**
		 * Set the width of the window, in pixels.
		 */
		{t.set_width(uint)} -> std::same_as<void>;
		/**
		 * Set the height of the window, in pixels.
		 */
		{t.set_height(uint)} -> std::same_as<void>;
		/**
		 * Set the title of the window.
		 */
		{t.set_title(cstr)} -> std::same_as<void>;
		/**
		 * Advance window state. You should call this every single frame.
		 */
		{t.update()} -> std::same_as<void>;
		/**
		 * Query as to whether the window has been requested to close by the user (such as by clicking the 'x' button at the edge of the window).
		 */
		{t.is_close_requested()} -> std::same_as<bool>;
	};
}

#endif // EMERALD_SRC_API_WINDOW_HPP
