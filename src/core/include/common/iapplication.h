#pragma once

#include "icomponent.h"

namespace core
{
    class IApplication
    {
    public:
        virtual ~IApplication() {};

        virtual void init() = 0;
        virtual void run() = 0;
        virtual void addComponent(IComponent&) = 0;

    protected:
        std::vector<IComponent> m_Components;
    };
}
