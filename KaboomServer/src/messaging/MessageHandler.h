#pragma once

class Message;

class MessageHandler {
public:
    virtual ~MessageHandler() {}

    virtual bool handle(const Message &message) const = 0;
};
