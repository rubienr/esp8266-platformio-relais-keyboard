#pragma once

#include <type_traits>
#include <array>

#include <HardwareSerial.h>

#include "keyboard/KeyEvent.h"
#include "RelayTypes.h"

template<typename Element, uint8_t elements_count>
struct UsingArrayTemplate
{
    using ElementType = Element;
    using ArrayType = std::array<Element, elements_count>;
};

template<uint8_t relays_count>
struct RelaysActuationTemplate : public UsingArrayTemplate<Relay::Actuation, relays_count>
{
    using super = UsingArrayTemplate<Relay::Actuation, relays_count>;
    using ElementType = typename super::ElementType;
    using ArrayType = typename super::ArrayType;

    ArrayType relay_actuation{{static_cast<Relay::Actuation>(0)}};
};

template<uint8_t key_event_types_count, uint8_t relays_count>
struct EventTypeRelaysActionTemplate
        : public UsingArrayTemplate<RelaysActuationTemplate<relays_count>, key_event_types_count>
{
    using super = UsingArrayTemplate<RelaysActuationTemplate<relays_count>, key_event_types_count>;
    using ElementType = typename super::ElementType;
    using ArrayType = typename super::ArrayType;

    ArrayType event_type{{}};
};

template<uint8_t keys_count, uint8_t key_event_types_count, uint8_t relays_count>
struct KeyEventRelaysActionTemplate
        : public UsingArrayTemplate<EventTypeRelaysActionTemplate<key_event_types_count, relays_count>, keys_count>
{
    using super = UsingArrayTemplate<EventTypeRelaysActionTemplate<key_event_types_count, relays_count>, keys_count>;
    using ElementType = typename super::ElementType;
    using ArrayType = typename super::ArrayType;

    ArrayType key_code{{}};
};

template<uint8_t keys_count, uint8_t key_event_types_count, uint8_t relays_count>
struct KeyEventRelaysActionTemplateDefaults :
        public KeyEventRelaysActionTemplate<keys_count, key_event_types_count, relays_count>
{
    using super = KeyEventRelaysActionTemplate<keys_count, key_event_types_count, relays_count>;
    using ElementType = typename super::ElementType;
    using ArrayType = typename super::ArrayType;

    void setDefaults(KeyEvent::Type event_type, Relay::Actuation default_actuation_action)
    {
        uint8_t key_code = 0;
        for (auto &key : this->key_code)
        {
            auto &event = key.event_type[KeyEvent::uint8FromKeyType(event_type)];
            uint8_t relay_id = 0;

            for (auto &relay_action: event.relay_actuation)
            {
                if (key_code == relay_id)
                {
                    relay_action = default_actuation_action;
                }
                relay_id++;
            }
            key_code++;
        }
    }
};

using KeyEventsRelaysAction = KeyEventRelaysActionTemplateDefaults<KeyEvent::keyEnumerationsCount(),
        KeyEvent::uint8FromKeyType(KeyEvent::Type::Released) + 1, 8>;
