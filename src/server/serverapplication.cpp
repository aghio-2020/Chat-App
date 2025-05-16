#include "serverapplication.h"

ServerApplication::ServerApplication(const char* bindingAddress, const uint16_t port)
	: IApplication()
	, m_ServerComp(m_Broadcaster, bindingAddress, port)
{
	init();
	run();
}

void ServerApplication::init()
{
	m_ServerComp.init();
}

void ServerApplication::run()
{
	while (true)
	{
		m_ServerComp.update();
	}
}
