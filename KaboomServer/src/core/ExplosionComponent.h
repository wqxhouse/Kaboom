#pragma once

#include <core/Component.h>

#include "ExplosionHandler.h"

class ExplosionComponent : public Component {
public:
    ExplosionComponent(ExplosionHandler *handler);

    ExplosionHandler *getHandler() const;

private:
    ExplosionHandler *handler;
};
