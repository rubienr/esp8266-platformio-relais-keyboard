#pragma once

#include <inttypes.h>

struct ScopedPin {

    ScopedPin(uint8_t pin_number, uint8_t alive_value, uint8_t out_of_scope_value);

    ~ScopedPin();

protected:
    uint8_t pin_number;
    uint8_t scope_left_value;
};

struct ScopedTogglePin {

    ScopedTogglePin(uint8_t pin_number);

    ~ScopedTogglePin();

protected:
    uint8_t pin_number;
};
