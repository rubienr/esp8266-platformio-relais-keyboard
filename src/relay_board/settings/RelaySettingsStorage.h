#pragma once

#include "settings/Settings.h"
#include <stdint.h>
#include "RelayEventsActions.h"

class RelaySettingsStorage : public Settings
{

public:
    using RelayState = KeyEventRelayAction;

    RelaySettingsStorage();

    void storeRelayFlags(uint16_t flags);

    uint16_t readRelayFlags();

    void readKeyRelayActions(RelayState &a);

    void writeKeyRelayActions(const RelayState &a);

protected:
    void readRelayActuation(RelayState::ElementType::ElementType &a, JsonVariant node);

    void writeRelayActuation(const RelayState::ElementType::ElementType &a, JsonVariant node);

    void readKeyEventRelayAction(RelayState::ElementType &a, JsonVariant node);

    void writeKeyEventRelayAction(const RelayState::ElementType &a, JsonVariant node);
};
