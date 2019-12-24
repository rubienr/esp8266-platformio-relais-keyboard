#include "ExpansionBoard.h"

//#include <pcf8574_esp.h>
#include <Wire.h>

ExpansionBoard::ExpansionBoard(uint8_t address, uint8_t chip) : m_address(address), m_chip(chip)
{}

void ExpansionBoard::setup(uint16_t output_pin_values)
{
    Wire.setClock(100000L);
    begin(m_address, m_chip);
    setModeAllOutput(0xffff);
}

void ExpansionBoard::setModeAllOutput(uint16_t initial_output_pin_values)
{
    _DDR = 0xff;
    _PORT = initial_output_pin_values;
    updateGPIO();
}
