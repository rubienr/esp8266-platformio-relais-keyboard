#include "RelaySettingsStorage.h"

#include <stdint.h>
#include <WString.h>

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

void RelaySettingsStorage::readRelayActuation(RelayState::ElementType::ElementType &a, JsonVariant node)
{
    uint8_t i = 0;

    for (auto &r : a.relay_actuation)
    {
        JsonVariant value = node.getElement(i++);
        if (!value.isNull())
        {
            r = Relay::actuationFromUint8(value.as<uint8_t>());
        }
    }
}

void RelaySettingsStorage::writeRelayActuation(const RelayState::ElementType::ElementType &a, JsonVariant node)
{
    uint8_t i = 0;

    for (const auto &r : a.relay_actuation)
    {
        JsonVariant array_element = node.getElement(i++);
        if (array_element.isNull())
        {
            node.add(Relay::uint8FromActuation(r));
        } else
        {
            array_element.set(Relay::uint8FromActuation(r));
        }
    }
}

void RelaySettingsStorage::readKeyEventRelayAction(RelayState::ElementType &a, JsonVariant node)
{
    auto i = 0;

    for (auto &e : a.event_type)
        readRelayActuation(e, node.getOrAddMember(String(i++)));
}

void RelaySettingsStorage::writeKeyEventRelayAction(const RelayState::ElementType &a, JsonVariant node)
{
    auto i = 0;

    for (const auto &e : a.event_type)
        writeRelayActuation(e, node.getOrAddMember(String(i)));
}

void RelaySettingsStorage::readKeyRelayActions(RelayState &a)
{
    JsonVariant node = getDocument();
    uint8_t i = 0;

    for (auto &r : a.key_code)
        readKeyEventRelayAction(r, node.getOrAddMember(String(i++)));
}

void RelaySettingsStorage::writeKeyRelayActions(const RelayState &a)
{
    JsonVariant node = getDocument();
    uint8_t i = 0;

    for (const auto &r : a.key_code)
        writeKeyEventRelayAction(r, node.getOrAddMember(String(i++)));
}

RelaySettingsStorage::RelaySettingsStorage() : Settings("/conf/key_relay_conf.json")
{}
