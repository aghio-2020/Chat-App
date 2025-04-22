#include "clientapplication.h"

ClientApplication::ClientApplication()
	: m_Broadcaster()
{
}

void ClientApplication::init()
{
	for (auto& component : m_Components)
	{
		component.init(m_Broadcaster);
	}
}

void ClientApplication::run()
{
	while (true)
	{
		for (auto& component : m_Components)
		{
			component.update();
		}
	}
}

void ClientApplication::addComponent(core::IComponent& component)
{
	m_Components.push_back(component);
}
