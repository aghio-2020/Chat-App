#include <ui/openglrenderer.h>

namespace core::ui
{
const int k_DefaultWindowHeight = 720;
const int k_DefaultWindowWidth = 1080;

OpenGLRenderer::OpenGLRenderer(std::string const& appName)
    : m_WindowName(appName)
{
}

bool OpenGLRenderer::initViewport()
{
    if (!glfwInit())
    {
        return false;
    }

    std::string glslVersion;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    glslVersion = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    glslVersion = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    m_Window = glfwCreateWindow(k_DefaultWindowWidth, k_DefaultWindowHeight, m_WindowName.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());

    return true;
}

bool OpenGLRenderer::createFrame()
{
    if (!m_Window || glfwWindowShouldClose(m_Window))
    {
        // TODO: call application exit event with relay
        return false;
    }

    glfwPollEvents();
    if (glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return false;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
	return true;
}

bool OpenGLRenderer::renderFrame()
{
    if (!m_Window)
        return false;

    int height, width;
    glfwGetFramebufferSize(m_Window, &width, &height);
    glViewport(0, 0, width, height);

    const auto cc = m_ClearColor;
    glClearColor(cc.x * cc.w, cc.y * cc.w, cc.z * cc.w, cc.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_Window);

    return true;
}

void OpenGLRenderer::destroy()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    m_Window = nullptr;
    glfwTerminate();
}

void OpenGLRenderer::getViewportSize(int& width, int& height)
{
    if (!m_Window)
        return;

    glfwGetWindowSize(m_Window, &width, &height);
}

void OpenGLRenderer::setViewportSize(int width, int height)
{
    if (!m_Window)
        return;

    glfwSetWindowSize(m_Window, width, height);
}


GLFWwindow* OpenGLRenderer::getWindow()
{
    return m_Window;
}

}
