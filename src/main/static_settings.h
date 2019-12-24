#pragma once

#include <inttypes.h>

struct StaticSettings {
    struct Relay {
        constexpr static uint8_t COUNT{8};
    };
};