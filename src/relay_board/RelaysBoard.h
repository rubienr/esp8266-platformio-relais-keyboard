#pragma once

#include <array>
#include <expansion_board/ExpansionBoard.h>
#include <relay_board/settings/RelayEventsActions.h>

#include "settings/RelayTypes.h"

struct RelaysBoard
{
    explicit RelaysBoard(ExpansionBoard &sink) : sink(sink)
    {}

    void setup()
    {}

    void actuate(const KeyEventsRelaysAction::ElementType::ElementType::ArrayType &action)
    {
        Serial.printf("RelayBoardTemplate::actuate: [");
        String delimiter{""};

        uint8_t relay_id = 0;
        for (const auto &a : action)
        {
            if (Relay::Actuation::NoAction != a)
            {
                uint8_t relay_action = Relay::uint8FromActuation(a);
                uint8_t mapped_action = action_mapping[relay_action];
                uint8_t p = expanded_pin_to_relay_id_mapping[relay_id];
                Serial.printf("%s[%d]=%d", delimiter.c_str(), p, mapped_action);
                sink.digitalWrite(p, mapped_action);

                if (delimiter.length() == 0)
                    delimiter += ", ";

            }
            relay_id++;
        }
        Serial.printf("]\n");
    }

protected:
    ExpansionBoard &sink;

    static constexpr const uint8_t capacity
            {static_cast<uint8_t >(std::tuple_size<KeyEventsRelaysAction::ElementType::ElementType::ArrayType>::value)};
    std::array<uint8_t, capacity> expanded_pin_to_relay_id_mapping{{0, 1, 2, 3, 4, 5, 6, 7}};

    constexpr static const uint8_t ACTUATE{1};
    constexpr static const uint8_t RELEASE{0};
    std::array<uint8_t, 2> action_mapping{{ACTUATE, RELEASE}};
};
