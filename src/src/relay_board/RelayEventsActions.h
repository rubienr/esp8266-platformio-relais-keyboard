#pragma once

#include <type_traits>

#include <HardwareSerial.h>

#include "keyboard/KeyEvent.h"
#include "RelayTypes.h"

template<uint8_t __relays_count>
struct RelaysActuationTemplate
{
    constexpr static const uint8_t RELAYS_COUNT{__relays_count};
    Relay::Actuation relay_actuation[RELAYS_COUNT]{static_cast<Relay::Actuation>(0)};
};

template<uint8_t __key_event_types_count, uint8_t __relays_count>
struct EventTypeRelaysActionTemplate
{
    using Wrapped = RelaysActuationTemplate<__relays_count>;
    constexpr static const uint8_t KEY_EVENT_TYPES_COUNT{__key_event_types_count};

    Wrapped event_type[KEY_EVENT_TYPES_COUNT];
};

template<uint8_t __keys_count, uint8_t __key_event_types_count, uint8_t __relays_count>
struct KeyEventRelaysActionTemplate
{
    using Wrapped = EventTypeRelaysActionTemplate<__key_event_types_count, __relays_count>;
    constexpr static const uint8_t KEYS_COUNT{__keys_count};

    Wrapped key_code[KEYS_COUNT];
};

using KeyEventsRelaysAction = KeyEventRelaysActionTemplate
        <
                KeyEvent::keyEnumerationsCount(),
                KeyEvent::uint8FromKeyType(KeyEvent::Type::Released),
                8
        >;

