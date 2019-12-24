#include "RelaysBoard.h"

void RelaysBoard::actuate(const KeyEventRelayAction::ElementType::ElementType::ArrayType &action)
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

void RelaysBoard::setup()
{}

RelaysBoard::RelaysBoard(ExpansionBoard &sink) : sink(sink)
{}
