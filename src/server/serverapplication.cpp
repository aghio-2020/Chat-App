#include "serverapplication.h"

ServerApplication::ServerApplication()
{
}

void ServerApplication::init()
{
	for (auto& component : m_Components)
	{
		component.init();
	}
}

void ServerApplication::run()
{
	while (true)
	{
		for (auto& component : m_Components)
		{
			component.update();
		}
	}
}

void ServerApplication::addComponent(core::IComponent& component)
{
	m_Components.push_back(component);
}
