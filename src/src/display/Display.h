#pragma once

#include <Adafruit_SSD1306.h>

class Display : public Adafruit_SSD1306
{
public:
    //Display(uint8_t address = 0x3C, uint8_t sda = D2, uint8_t scl = D1);
    Display();

    void setup();
};

