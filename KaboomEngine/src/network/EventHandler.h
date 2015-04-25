#pragma once

class Event;

class EventHandler {
public:
    virtual ~EventHandler() {}

    virtual void handle(const Event &e) const = 0;
};
