#include "RelaySettingsStorage.h"

#include <stdint.h>
#include <WString.h>
#include <Esp.h>

#include "RelayStrings.h"

void RelaySettingsStorage::storeRelayFlags(uint16_t flags)
{
    JsonVariant node = getDocument();
    node = node[relay::settings::names::RelayKeys::RELAY][relay::settings::names::RelayFlagsKeys::ACTIVATION_FLAGS];
    node.set(flags);
}

uint16_t RelaySettingsStorage::readRelayFlags()
{
    JsonVariant node = getDocument();
    return node[relay::settings::names::RelayKeys::RELAY][relay::settings::names::RelayFlagsKeys::ACTIVATION_FLAGS].as<__uint16_t>();
}

void RelaySettingsStorage::readRelayActuation(RelayState::Wrapped::Wrapped &a, JsonVariant node)
{
    for (__uint8_t r = 0; r < RelayState::Wrapped::Wrapped::RELAYS_COUNT; r++)
    {
        JsonVariant value = node.getElement(r);
        if (!value.isNull())
        {
            a.relay_actuation[r] = Relay::actuationFromUint8(value.as<__uint8_t>());
        }
    }
}

void RelaySettingsStorage::writeRelayActuation(const RelayState::Wrapped::Wrapped &a, JsonVariant node)
{
    for (__uint8_t r = 0; r < RelayState::Wrapped::Wrapped::RELAYS_COUNT; r++)
    {
        JsonVariant array_element = node.getElement(r);
        if (array_element.isNull())
        {
            node.add(Relay::uint8FromActuation(a.relay_actuation[r]));
        } else
        {
            array_element.set(Relay::uint8FromActuation(a.relay_actuation[r]));
        }
    }
}

void RelaySettingsStorage::readKeyEventRelayAction(RelayState::Wrapped &a, JsonVariant node)
{
    for (auto e = 0; e < RelayState::Wrapped::KEY_EVENT_TYPES_COUNT; e++)
    {
        readRelayActuation(a.event_type[e], node.getOrAddMember(String(e)));
    }
}

void RelaySettingsStorage::writeKeyEventRelayAction(const RelayState::Wrapped &a, JsonVariant node)
{
    for (auto e = 0; e < a.KEY_EVENT_TYPES_COUNT; e++)
    {
        writeRelayActuation(a.event_type[e], node.getOrAddMember(String(e)));
    }
}

void RelaySettingsStorage::readKeyRelayActions(RelayState &a)
{
    JsonVariant node = getDocument();

    for (__uint8_t r = 0; r < RelayState::KEYS_COUNT; r++)
    {
        ESP.wdtFeed();
        readKeyEventRelayAction(a.key_code[r], node.getOrAddMember(String(r)));
    }
}

void RelaySettingsStorage::writeKeyRelayActions(const RelayState &a)
{
    JsonVariant node = getDocument();

    for (__uint8_t r = 0; r < RelayState::KEYS_COUNT; r++)
    {
        writeKeyEventRelayAction(a.key_code[r], node.getOrAddMember(String(r)));
    }
}

RelaySettingsStorage::RelaySettingsStorage() : Settings("/key-relay-settings.json")
{}
