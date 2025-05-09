#pragma once

#include <communication/internalevent.h>
#include <ui/uiutils.h>
#include <string>

namespace core::events
{
    class GUIEvents
    {
    public:
        InternalEvent<ui::utils::ChatMessageInfo const&> onMessagePostedToChat;
    };
}