#if ELD_WIN
#include "win/context.hpp"
#include <cassert>

namespace eld
{
	ContextWin32::ContextWin32(HDC device_context):
	ctx(wglCreateContext(device_context)),
	device_ctx(device_context)
	{
		volatile int err = GetLastError();
		assert(ctx != nullptr);
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
		return this->device_ctx == nullptr;
	}

	bool ContextWin32::is_current() const
	{
		return wglGetCurrentContext() == this->ctx;
	}

	void ContextWin32::make_current()
	{
		wglMakeCurrent(this->device_ctx, this->ctx);
	}
}

#endif // ELD_WIN
