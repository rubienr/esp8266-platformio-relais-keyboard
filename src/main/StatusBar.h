#pragma once

#include <display/Display.h>
#include "OperatingMode.h"
#include <avr/pgmspace.h>

struct StatusBar
{
    StatusBar(const OperatingMode &operating_mode, Display &display);

    void update();

protected:

    const OperatingMode &operating_mode;
    Display &display;

    void printWifiStatus();

    Dimension bar_dimension{0, 0};
    Rectangle print_area{0, 0, 0, 0};

    void reset();
};
