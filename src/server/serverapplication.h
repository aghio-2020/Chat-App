#pragma once

#include <common/iapplication.h>
#include <common/icomponent.h>
#include "servercomponent.h"

class ServerApplication : public core::IApplication
{
public:
	ServerApplication();

	void init() override;
	void run() override;

private:
	ServerComponent m_ServerComp;
};
