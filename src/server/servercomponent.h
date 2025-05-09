#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <net/server.h>

class ServerComponent : public core::IComponent
{
public:
	ServerComponent(core::events::Broadcaster& braodcaster);

	void init() override;
	void update() override;

private:
	core::Server m_Server;
};
