#pragma once

namespace core::ui
{
	class IRenderer
	{
	public:
		virtual bool initViewport() = 0;
		virtual bool createFrame() = 0;
		virtual bool renderFrame() = 0;
		virtual void destroy() = 0;
		virtual void getViewportSize(int& width, int& height) = 0;
		virtual void setViewportSize(int width, int height) = 0;
	};
}