#pragma once

namespace core::ui
{
	class IRenderer
	{
	public:
		virtual bool initWindow() = 0;
		virtual bool createFrame() = 0;
		virtual bool renderFrame() = 0;
		virtual void getWindowSize(int& width, int& height) = 0;
		virtual void destroy() = 0;
	};
}