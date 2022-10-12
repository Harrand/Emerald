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
	ContextWin32::ContextWin32(HDC device_context):
	ctx(wglCreateContext(device_context)),
	device_ctx(device_context)
	{
		volatile int err = GetLastError();
		assert(ctx != nullptr);
	}

	ContextWin32::ContextWin32(ContextWin32&& move):
	ctx(move.ctx),
	device_ctx(move.device_ctx)
	{
		move.ctx = nullptr;
	}

	ContextWin32::~ContextWin32()
	{
		if(this->ctx != nullptr)
		{
			wglDeleteContext(this->ctx);
		}
	}

	ContextWin32& ContextWin32::operator=(ContextWin32&& rhs)
	{
		win_impl::swap(this->ctx, rhs.ctx);
		win_impl::swap(this->device_ctx, rhs.device_ctx);
		return *this;
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
