#pragma once

#include "main/Settings.h"
#include <stdint.h>
#include "relay_board/RelayEventsActions.h"

class RelaySettingsStorage : public Settings
{

public:
    using RelayState = KeyEventsRelaysAction;

    RelaySettingsStorage();

    void storeRelayFlags(uint16_t flags);

    uint16_t readRelayFlags();

    void readKeyRelayActions(RelayState &a);

    void writeKeyRelayActions(const RelayState &a);

protected:
    void readRelayActuation(RelayState::Wrapped::Wrapped &a, JsonVariant node);

    void writeRelayActuation(const RelayState::Wrapped::Wrapped &a, JsonVariant node);

    void readKeyEventRelayAction(RelayState::Wrapped &a, JsonVariant node);

    void writeKeyEventRelayAction(const RelayState::Wrapped &a, JsonVariant node);
};
