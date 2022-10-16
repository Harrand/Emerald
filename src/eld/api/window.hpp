#ifndef EMERALD_SRC_API_WINDOW_HPP
#define EMERALD_SRC_API_WINDOW_HPP
#include <concepts>
#include <cstdint>
#include "eld/api/context.hpp"

namespace eld
{
	/**
	 * Specifies how you intend to draw into the window.
	 */
	enum class WindowRenderingIntent
	{
		/// - CPU-side painting functions which eventually call into the OS API. Use this if you want to use the Emerald software rendering API (dev: which doesnt exist yet ;) ).
		SoftwareRendering,
		/// - Window surface will be renderered into through a hardware-accelerated graphics API, such as Vulkan or OpenGL. Use this if you're hooking this upto a graphics engine such as Topaz.
		HardwareAccelerated
	};

	/**
	 * Specifies which hardware-accelerated graphics API you intend to use.
	 */
	enum class HardwareGraphicsAPI
	{
		/// - OpenGL requires bespoke window system integration before an OpenGL context can be created. For this reason, it is essential you use this value if you intend to use OpenGL to draw into the window.
		OpenGL,
		/// - A value representing a graphics-API that is not specified here (e.g vulkan).
		Other,
	};

	struct WindowRenderingDetails
	{
		struct OpenGLDetails
		{
			int ogl_major_ver = 3;
			int ogl_minor_ver = 3;
			bool debug_context = false;
			// todo: possibly support compat contexts?
			bool double_buffer = true;
			std::uint8_t colour_buffer_depth = 32;
			std::uint8_t depth_buffer_size_bits = 24;
			std::uint8_t stencil_buffer_size_bits = 8;
		};
		/// Represents which graphics API is intended to be used for a hardware-accelerated window. If the window's rendering intent is SoftwareRendering, this is ignored.
		HardwareGraphicsAPI hardware_api = HardwareGraphicsAPI::Other;
		/// Specifies specific values used when setting up the OpenGL context. If `hardware_api != HardwareGraphicsAPI::OpenGL` or the window is software rendering, this is ignored.
		OpenGLDetails ogl_details = OpenGLDetails{};
	};
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
		/// Intent of the rendering done into the window. See @ref WindowRenderingIntent for more info.
		WindowRenderingIntent intent;
		/// Describes in more detail what type of rendering is expected to be done for the window. Failing to set the details properly will lead to serious runtime hazards.
		WindowRenderingDetails details = {};
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
		 * Query as to whether the window has been requested to close by the user (such as by clicking the 'x' button at the edge of the window, or via `request_close`).
		 */
		{t.is_close_requested()} -> std::same_as<bool>;
		/**
		 * Request the window to close. This is guaranteed to succeed, but will not happen instantly. It may take a few update cycles to actually close.
		 */
		{t.request_close()} -> std::same_as<void>;
		/**
		 * Retrieve the rendering intent used when creating the window. This essentially lets you query as to whether the window supports hardware-accelerated graphics-APIs or software rendering (these are mutually exclusive).
		 */
		{t.get_rendering_type()} -> std::same_as<WindowRenderingIntent>;
		/**
		 * Attempt to create a new OpenGL context corresponding to the window.
		 */
		{t.get_context()} -> ContextType;
	};
}

#endif // EMERALD_SRC_API_WINDOW_HPP
