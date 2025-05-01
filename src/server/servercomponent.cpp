#include "servercomponent.h"

ServerComponent::ServerComponent(core::Broadcaster& broadcaster)
	: m_Server()
{
	m_Broadcaster = &broadcaster;
}

void ServerComponent::init()
{
	m_Server.run(6969);
}

void ServerComponent::update()
{
}
