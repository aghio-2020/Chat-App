#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "irenderer.h"

#include <string>

namespace core::ui
{
	class OpenGLRenderer : public IRenderer
	{
	public:
		OpenGLRenderer(std::string const& appName = "");

		bool initViewport() override;
		bool createFrame() override;
		bool renderFrame() override;
		void destroy() override;
		void getViewportSize(int& width, int& height) override;
		void setViewportSize(int width, int height) override;


		GLFWwindow* getWindow();

	private:
		GLFWwindow* m_Window = nullptr;
		std::string m_WindowName;

		ImVec4 m_ClearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	};
}