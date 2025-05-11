#include <ui/appgui.h>

#include <string>

#include <imgui_internal.h>

namespace core::ui
{
    const int k_BootWinHeight = 300;
    const int k_BootWinWidth = 400;

    const int k_MainWinHeight = 720;
    const int k_MainWinWidth = 800;

    const float k_MaxSidebarWidth = 220;

    AppGUI::AppGUI(std::string const& appName)
        : m_Renderer(appName)
        , m_BootLayout(static_cast<events::EventRelay<events::GUIEvents>&>(*this))
        , m_ChatLayout(static_cast<events::EventRelay<events::GUIEvents>&>(*this), m_ChatMessages)
        , m_ParticipantsLayout(static_cast<events::EventRelay<events::GUIEvents>&>(*this), m_Participants)
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
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
            checkWindowSize();
            showMainWindowUI();
        }
        else if (m_AppState.windowType == WindowType::BOOT_WINDOW)
        {
            m_Renderer.setViewportSize(k_BootWinWidth, k_BootWinHeight);
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
        msgInfo.username = msg.username;
        msgInfo.hostID = msg.hostID;

        m_ChatMessages.emplace_back(msgInfo);
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

    void AppGUI::onNewHostInChat(ui::utils::HostInfo const& host)
    {
        m_Participants.emplace_back(host);
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
        //TODO: figure how to do with window docking

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Chat Up!", nullptr, 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        float fullWidth = ImGui::GetContentRegionAvail().x;
        float fullHeight = ImGui::GetContentRegionAvail().y;

        float leftWidth = fullWidth * 0.25f;
        float rightWidth = fullWidth - leftWidth;

        m_ParticipantsLayout.setLayoutSize(leftWidth, fullHeight);
        m_ParticipantsLayout.update();
        ImGui::SameLine();
        m_ChatLayout.setLayoutSize(rightWidth, fullHeight);
        m_ChatLayout.update();

        ImGui::End();
    }

    // TODO:
    // find a better and easier way through the renderer and the viewport
    void AppGUI::checkWindowSize()
    {
        int w, h;
        m_Renderer.getViewportSize(w, h);
        m_Renderer.setViewportSize(w < k_MainWinWidth ? k_MainWinWidth : w, h < k_MainWinHeight ? k_MainWinHeight : h);
    }

}
