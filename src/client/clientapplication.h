#include <common/iapplication.h>
#include "uicomponent.h"
#include "networkcomponent.h"

// for now: ui starts everything and network just listens to msgs to act

class ClientApplication : public core::IApplication
{
public:
	ClientApplication();

	void init() override;
	void run() override;

private:
	UIComponent m_UIComp;
	NetworkComponent m_NetworkComp;
};