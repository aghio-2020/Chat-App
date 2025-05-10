#include <ui/appgui.h>

#include <string>

namespace core::ui
{
    const int k_BootWinHeight = 400;
    const int k_BootWinWidth = 400;

    const int k_MainWinHeight = 720;
    const int k_MainWinWidth = 720;

    AppGUI::AppGUI(std::string const& appName)
        : m_Renderer(appName)
        , m_ChatLayout(static_cast<events::EventRelay<events::GUIEvents>&>(*this))
        , m_BootLayout(static_cast<events::EventRelay<events::GUIEvents>&>(*this))
    {
    }

    AppGUI::~AppGUI()
    {
        closeApp();
    }

    void AppGUI::closeApp()
    {
        if (m_AppState.closing)
        {
            return;
        }
        m_Renderer.destroy();
        getRelay().onShouldQuit();
    }

    void AppGUI::init()
    {
        m_Renderer.initViewport();
        m_Renderer.setViewportSize(k_BootWinWidth, k_BootWinHeight);

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

        ImGui::StyleColorsClassic();
    }

    void AppGUI::onUpdate()
    {
        if (m_AppState.closing)
        {
            return;
        }

        if (!m_Renderer.createFrame())
        {
            closeApp();
            return;
        }
        ImGui::NewFrame();

        if (m_AppState.windowType == WindowType::MAIN_WINDOW)
        {
            showMainWindowUI();
        }
        else if (m_AppState.windowType == WindowType::BOOT_WINDOW)
        {
            //m_Renderer.setViewportSize(k_BootWinWidth, k_BootWinHeight);
            showBootWindowUI();
        }

        ImGui::Render();
        if (!m_Renderer.renderFrame())
        {
            closeApp();
            return;
        }
    }

    AppStateData& AppGUI::getAppState()
    {
        return m_AppState;
    }

    void AppGUI::onNewChatMessage(core::messages::ChatMessage const& msg)
    {
        core::ui::utils::ChatMessageInfo msgInfo;
        msgInfo.text = msg.text;
        msgInfo.userName = msg.username;
        msgInfo.hostID = msg.hostID;

        m_ChatLayout.addMessageToUI(msgInfo);
    }

    void AppGUI::onWindowChange(WindowType win)
    {
        m_AppState.windowType = win;

        if (m_AppState.windowType == WindowType::MAIN_WINDOW)
        {
            m_Renderer.setViewportSize(k_MainWinWidth, k_MainWinHeight);
        }
        else if (m_AppState.windowType == WindowType::BOOT_WINDOW)
        {
            m_Renderer.setViewportSize(k_BootWinWidth, k_BootWinHeight);
        }
    }

    void AppGUI::showBootWindowUI()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Chat Up!", nullptr, 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        m_BootLayout.update();

        ImGui::End();
    }

    void AppGUI::showMainWindowUI()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Chat Up!", nullptr, 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        m_ChatLayout.update();

        ImGui::End();
    }

}
