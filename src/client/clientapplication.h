#include <common/iapplication.h>
#include <common/icomponent.h>

class ClientApplication : public core::IApplication
{
public:
	ClientApplication();

	void init() override;
	void run() override;
	void addComponent(core::IComponent&) override;

private:
	core::Broadcaster m_Broadcaster;
};