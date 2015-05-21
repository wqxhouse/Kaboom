#pragma once

#include <network/EventHandler.h>

class ReloadRequestEventHandler : public EventHandler {
public:
    virtual void handle(const Event &e) const;
};
