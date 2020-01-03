#pragma once

#include <array>
#include "expansion_board/ExpansionBoard.h"
#include "relay_board/settings/KeyRelaySettings.h"
#include "settings/RelayTypes.h"

struct RelaysBoard {
    using RelayActuationType = KeyRelaySettings::KeyCodeElementType::ElementType::ArrayType;

    explicit RelaysBoard(ExpansionBoard &sink);

    void setup();

    void setup(uint16_t initial_state_flags);

    uint16_t flags();

    void actuate(const RelayActuationType &action);

protected:
    ExpansionBoard &sink;

    static constexpr const uint8_t RELAYS_COUNT{static_cast<uint8_t >(std::tuple_size<RelayActuationType>::value)};
    static const std::array<uint8_t, RELAYS_COUNT> RELAY_ID_TO_PIN_MAPPING;

    constexpr static const uint8_t ENERGIZE{1};
    constexpr static const uint8_t RELEASE{0};
    static const std::array<uint8_t, 2> RELAY_ACTION_MAPPING;

    void energize(uint_fast8_t relay_id);

    void release(uint_fast8_t relay_id);

    void invert(uint_fast8_t relay_id);

    void leaveUnchanged(uint_fast8_t relay_id);

    void writeToHardware(uint8_t relay_id, uint8_t digital_pin_value);

    uint8_t readFromHardware(uint8_t relay_id);
};

