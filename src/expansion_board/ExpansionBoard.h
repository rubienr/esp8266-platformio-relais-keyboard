#pragma once

#include <PCF857X.h>
#include <stdint.h>

struct ExpansionBoard : public PCF857X
{
    explicit ExpansionBoard(uint8_t address = 0x20, uint8_t chip = CHIP_PCF8575);

    void setup(uint16_t initial_output_pin_values = 0x0000);

protected:
    void setModeAllOutput(uint16_t initial_output_pin_values = 0x0000);

    uint8_t m_address{0x21};
    uint8_t m_chip{CHIP_PCF8574};
};
