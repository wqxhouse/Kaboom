#pragma once

class EntitySystem {
public:
    virtual ~EntitySystem() {}

    virtual void process() = 0;
};
