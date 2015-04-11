#pragma once

enum ComponentType {
    SCENE_NODE,
    POSITION
};

class Component {
public:
    virtual ~Component() {}

    virtual ComponentType getType() const = 0;
};
