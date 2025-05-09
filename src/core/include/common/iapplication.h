#pragma once

#include "icomponent.h"
#include <communication/broadcaster.h>

namespace core
{
    class IApplication
    {
    public:
        virtual ~IApplication() {};

        virtual void init() = 0;
        virtual void run() = 0;

    protected:
        events::Broadcaster m_Broadcaster;
    };
}
