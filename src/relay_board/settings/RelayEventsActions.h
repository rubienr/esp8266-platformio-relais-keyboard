#pragma once

#include <type_traits>
#include <array>

#include <HardwareSerial.h>
#include <main/static_settings.h>

#include "keyboard/KeyEvent.h"
#include "RelayTypes.h"

template<typename Element, uint8_t elements_count>
struct UsingArrayTemplate
{
    using ElementType = Element;
    using ArrayType = std::array<Element, elements_count>;
};

template<uint8_t relays_count>
struct RelayActuationTemplate : public UsingArrayTemplate<Relay::Actuation, relays_count>
{
    using super = UsingArrayTemplate<Relay::Actuation, relays_count>;
    using ElementType = typename super::ElementType;
    using ArrayType = typename super::ArrayType;

    ArrayType relay_actuation{{static_cast<Relay::Actuation>(0)}};
};

template<uint8_t key_event_types_count, uint8_t relays_count>
struct EventTypeRelayActionTemplate
        : public UsingArrayTemplate<RelayActuationTemplate<relays_count>, key_event_types_count>
{
    using super = UsingArrayTemplate<RelayActuationTemplate<relays_count>, key_event_types_count>;
    using ElementType = typename super::ElementType;
    using ArrayType = typename super::ArrayType;

    ArrayType event_type{{}};
};

template<uint8_t keys_count, uint8_t key_event_types_count, uint8_t relays_count>
struct KeyEventRelayActionTemplate
        : public UsingArrayTemplate<EventTypeRelayActionTemplate<key_event_types_count, relays_count>, keys_count>
{
    using super = UsingArrayTemplate<EventTypeRelayActionTemplate<key_event_types_count, relays_count>, keys_count>;
    using ElementType = typename super::ElementType;
    using ArrayType = typename super::ArrayType;

    ArrayType key_code{{}};
};

using KeyEventRelayAction = KeyEventRelayActionTemplate<KeyEvent::keyEnumerationsCount(),
        KeyEvent::uint8FromKeyType(KeyEvent::Type::Released) + 1, StaticSettings::Relay::COUNT>;
