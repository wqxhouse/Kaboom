#pragma once

template <typename T>
class EventHandler {
public:
    virtual ~EventHandler() {}

    virtual void handle(const T &evt) = 0;
};
