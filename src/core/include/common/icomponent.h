#pragma once

#include <communication/broadcaster.h>

namespace core
{
    class IComponent
    {
    public:
        virtual ~IComponent() { m_Broadcaster = nullptr; }

        virtual void init() = 0;
        virtual void update() = 0;

    protected:
        events::Broadcaster* m_Broadcaster;
    };
}
