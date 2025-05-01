#include "clientapplication.h"

ClientApplication::ClientApplication()
	: IApplication()
	, m_UIComp(m_Broadcaster)
	, m_NetworkComp(m_Broadcaster)
{
	init();
	run();
}

void ClientApplication::init()
{
	m_UIComp.init();
	m_NetworkComp.init();
}

void ClientApplication::run()
{
	while (true)
	{
		m_UIComp.update();
		m_NetworkComp.update();
	}
}
