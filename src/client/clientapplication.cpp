#include "clientapplication.h"

ClientApplication::ClientApplication()
	: m_Broadcaster()
	, m_UIComp(m_Broadcaster)
	, m_NetworkComp(m_Broadcaster)
{
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
