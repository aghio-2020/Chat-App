#include <ui/appgui.h>

#include <string>

namespace core::ui
{

    AppGUI::AppGUI(std::string const& appName)
        : m_Renderer(appName)
        , m_ChatLayout(static_cast<events::EventRelay<events::GUIEvents>&>(*this))
    {
        m_State.windowType = WindowType::MAIN_WINDOW;
    }

    AppGUI::~AppGUI()
    {
        closeApp();
    }

    void AppGUI::closeApp()
    {
        // TODO: call exit app event
        m_Renderer.destroy();
    }

    void AppGUI::init()
    {
        m_Renderer.initWindow();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

        ImGui::StyleColorsDark();
    }

    void AppGUI::onUpdate()
    {
        if (!m_Renderer.createFrame())
        {
            // TODO: exit app
            return;
        }
        ImGui::NewFrame();

        if (m_State.windowType == WindowType::MAIN_WINDOW)
        {
            showMainWindowUI();
        }
        else if (m_State.windowType == WindowType::BOOT_WINDOW)
        {
            showBootWindowUI();
        }

        ImGui::Render();
        if (!m_Renderer.renderFrame())
        {
            // TODO: exit application
            return;
        }
    }

    void AppGUI::onNewChatMessage(core::messages::ChatMessage const& msg)
    {
        core::ui::utils::ChatMessageInfo msgInfo;
        msgInfo.text = msg.text;
        msgInfo.userName = msg.username;
        msgInfo.hostID = msg.hostID;

        m_ChatLayout.addMessageToUI(msgInfo);
    }

    void AppGUI::showBootWindowUI()
    {
    }

    void AppGUI::showMainWindowUI()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::Begin("Chat Up!", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        m_ChatLayout.update();

        ImGui::End();
    }

}
