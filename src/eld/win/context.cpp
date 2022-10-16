#if ELD_WIN
#include "eld/win/context.hpp"
#include <cassert>

namespace eld
{
	namespace win_impl
	{
		void swap(HDC& a, HDC& b)
		{
			HDC t = a;
			a = b;
			b = t;
		}

		void swap(HGLRC& a, HGLRC& b)
		{
			HGLRC t = a;
			a = b;
			b = t;
		}

	}
	ContextWin32::ContextWin32(HDC device_context, HGLRC ctx):
	ctx(ctx),
	device_ctx(device_context)
	{
		assert(ctx != nullptr);
		assert(device_context != nullptr);
	}


	/*static*/ ContextWin32 ContextWin32::null()
	{
		return {};
	}

	bool ContextWin32::is_null() const
	{
		return this->ctx == nullptr;
	}

	bool ContextWin32::is_headless() const
	{
		// TODO: implementation for headless contexts.
		return false;
	}

	bool ContextWin32::is_current() const
	{
		return wglGetCurrentContext() == this->ctx;
	}

	void ContextWin32::make_current()
	{
		if(this->is_null())
		{
			assert(false && "Cannot make current the null context");
		}
		wglMakeCurrent(this->device_ctx, this->ctx);
	}
}

#endif // ELD_WIN
