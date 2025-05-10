#include "clientapplication.h"

ClientApplication::ClientApplication()
	: IApplication()
	, m_UIComp(m_Broadcaster)
	, m_NetworkComp(m_Broadcaster)
{
	
}

void ClientApplication::init()
{
	m_Broadcaster.subscribeToEvent(core::events::EventType::EXIT_APPLICATION,
		[this](core::events::ApplicationEvent const& event)
		{
			handleExitApplication();
		}
	);

	m_UIComp.init();
	m_NetworkComp.init();
}

void ClientApplication::run()
{
	while (!m_Quit)
	{
		m_UIComp.update();
		m_NetworkComp.update();
	}
}

void ClientApplication::handleExitApplication()
{
	m_Quit = true;
}
