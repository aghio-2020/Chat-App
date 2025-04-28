#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <net/client.h>

class NetworkComponent : 
	public core::IComponent, core::Client
{
public:
	NetworkComponent(core::Broadcaster& broadcaster);
	
	void init() override;
	void update() override;

private:

};