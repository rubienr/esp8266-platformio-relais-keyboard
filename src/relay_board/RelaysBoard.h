#pragma once

#include <array>
#include "expansion_board/ExpansionBoard.h"
#include "relay_board/settings/RelayEventsActions.h"
#include "settings/RelayTypes.h"

struct RelaysBoard
{
    explicit RelaysBoard(ExpansionBoard &sink);

    void setup();

    void actuate(const KeyEventRelayAction::ElementType::ElementType::ArrayType &action);

protected:
    ExpansionBoard &sink;

    static constexpr const uint8_t capacity
            {static_cast<uint8_t >(std::tuple_size<KeyEventRelayAction::ElementType::ElementType::ArrayType>::value)};
    std::array<uint8_t, capacity> expanded_pin_to_relay_id_mapping{{0, 1, 2, 3, 4, 5, 6, 7}};

    constexpr static const uint8_t ACTUATE{1};
    constexpr static const uint8_t RELEASE{0};
    std::array<uint8_t, 2> action_mapping{{ACTUATE, RELEASE}};
};
