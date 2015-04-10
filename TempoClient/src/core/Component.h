#pragma once

enum ComponentType {
    POSITION
};

class Component {
public:
    virtual ~Component() {}

    virtual ComponentType getType() const = 0;
};
