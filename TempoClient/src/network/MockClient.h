#pragma once

#include <queue>

#include "MoveEvent.h"

class MockClient {
public:
    static MockClient *client;

    MoveEvent *recv();
    void send(const MoveEvent &evt);
    void sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight);

private:
    std::queue<MoveEvent> buffer;
};
