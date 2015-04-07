#include "MockClient.h"

#include "MoveEvent.h"

MockClient *MockClient::client = new MockClient;

MoveEvent *MockClient::recv() {
    if (buffer.empty()) {
        return NULL;
    }

    MoveEvent bufferEvent = buffer.front();

    MoveEvent *evt = new MoveEvent;
    memcpy(evt, &bufferEvent, sizeof(MoveEvent));

    buffer.pop();

    return evt;
}

void MockClient::send(const MoveEvent &evt) {
    buffer.push(evt);
}

void MockClient::sendMoveEvent(bool movingForward, bool movingBackward, bool movingLeft, bool movingRight) {
    MoveEvent evt;
    evt.movingForward = movingForward;
    evt.movingBackward = movingBackward;
    evt.movingLeft = movingLeft;
    evt.movingRight = movingRight;

    send(evt);
}
