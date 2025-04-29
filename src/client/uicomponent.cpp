#include "uicomponent.h"

UIComponent::UIComponent(core::Broadcaster& broadcaster)
{
	m_Broadcaster = &broadcaster;
}

void UIComponent::init()
{
}

void UIComponent::update()
{
}
