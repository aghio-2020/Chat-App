#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>

class UIComponent : public core::IComponent
{
public:
	UIComponent(core::Broadcaster& broadcaster);

	void init() override;
	void update() override;

private:
};