#include "ReloadRequestEventHandler.h"

#include <util/Configuration.h>

#include "../core/EntityConfigLookup.h"

void ReloadRequestEventHandler::handle(const Event &e) const {
    EntityConfigLookup::load();
}
