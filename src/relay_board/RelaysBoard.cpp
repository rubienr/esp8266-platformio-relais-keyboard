#include "RelaysBoard.h"

const std::array<uint8_t, RelaysBoard::RELAYS_COUNT> RelaysBoard::RELAY_ID_TO_PIN_MAPPING{{0, 1, 2, 3, 4, 5, 6, 7}};
const std::array<uint8_t, 2> RelaysBoard::RELAY_ACTION_MAPPING{{ENERGIZE, RELEASE}};

RelaysBoard::RelaysBoard(ExpansionBoard &sink) : sink(sink) {}

void RelaysBoard::setup() {}

void RelaysBoard::setup(uint16_t initial_state_flags) {
    sink.write(initial_state_flags);
    setup();
}

void RelaysBoard::actuate(const RelayActuationType &action) {
    Serial.printf("RelaysBoard::actuate: [");
    String delimiter{""};

    uint8_t relay_id = 0;
    for (const auto &a : action) {

        uint8_t relay_action = Relay::uint8FromActuation(a);
        uint8_t mapped_action = RELAY_ACTION_MAPPING[relay_action];
        uint8_t p = RELAY_ID_TO_PIN_MAPPING[relay_id];
        Serial.printf("%s[%d]=%d", delimiter.c_str(), p, mapped_action);
        if (delimiter.length() == 0)
            delimiter += ", ";

        switch (a) {
            case Relay::Actuation::Activate:
                energize(relay_id);
                break;

            case Relay::Actuation::Release:
                release(relay_id);
                break;

            case Relay::Actuation::Invert:
                invert(relay_id);
                break;

            case Relay::Actuation::NoAction:
                leaveUnchanged(relay_id);
                break;
        }
        relay_id++;
    }
    Serial.printf("]\n");
}

void RelaysBoard::energize(uint_fast8_t relay_id) {
    uint8_t relay_action = Relay::uint8FromActuation(Relay::Actuation::Activate);
    uint8_t hardware_action = RELAY_ACTION_MAPPING[relay_action];
    writeToHardware(relay_id, hardware_action);
}

void RelaysBoard::release(uint_fast8_t relay_id) {
    uint8_t relay_action = Relay::uint8FromActuation(Relay::Actuation::Release);
    uint8_t hardware_action = RELAY_ACTION_MAPPING[relay_action];
    writeToHardware(relay_id, hardware_action);
}

void RelaysBoard::invert(uint_fast8_t relay_id) {
    if (readFromHardware((relay_id)) != 0) {
        energize(relay_id);
    } else {
        release(relay_id);
    }
}

void RelaysBoard::leaveUnchanged(uint_fast8_t) {
    // noop
}

void RelaysBoard::writeToHardware(uint8_t relay_id, uint8_t digital_pin_value) {
    uint8_t hardware_pin = RELAY_ID_TO_PIN_MAPPING[relay_id];
    sink.digitalWrite(hardware_pin, digital_pin_value);
}

uint8_t RelaysBoard::readFromHardware(uint8_t relay_id) {
    uint8_t hardware_pin = RELAY_ID_TO_PIN_MAPPING[relay_id];
    return sink.digitalRead(hardware_pin);
}

uint16_t RelaysBoard::flags() {
    return sink.read();
}


