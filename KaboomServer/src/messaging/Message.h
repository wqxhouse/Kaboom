#pragma once

class Entity;
class Game;
enum class MessageType;

class Message {
public:
    Message(MessageType type, Game *game, Entity *entity)
            : type(type),
              game(game),
              entity(entity) {
    }

    virtual ~Message() {}

    inline MessageType getType() const {
        return type;
    }

    inline Game *getGame() const {
        return game;
    }

    inline Entity *getEntity() const {
        return entity;
    }

private:
    MessageType type;
    Game *game;
    Entity *entity;
};
