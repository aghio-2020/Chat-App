#include "serverapplication.h"

ServerApplication::ServerApplication()
	: IApplication()
	, m_ServerComp(m_Broadcaster)
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
