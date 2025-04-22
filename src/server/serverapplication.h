#include <common/iapplication.h>
#include <common/icomponent.h>

class ServerApplication : public core::IApplication
{
public:
	ServerApplication();

	void init() override;
	void run() override;
	void addComponent(core::IComponent&) override;

private:

};
