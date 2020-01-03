#include "pin_utils.h"

#include <Arduino.h>
ScopedPin::ScopedPin(uint8_t pin_number, uint8_t alive_value, uint8_t out_of_scope_value) :
        pin_number(pin_number), scope_left_value(out_of_scope_value) {
    digitalWrite(pin_number, alive_value);
}

ScopedPin::~ScopedPin() {
    digitalWrite(pin_number, scope_left_value);
}


ScopedTogglePin::ScopedTogglePin(uint8_t pin_number) : pin_number(pin_number)
{
    if (digitalRead(pin_number))  digitalWrite(pin_number, 0);
    else digitalWrite(pin_number, 1);
}

ScopedTogglePin::~ScopedTogglePin() {
    if (digitalRead(pin_number))  digitalWrite(pin_number, 0);
    else digitalWrite(pin_number, 1);
}
