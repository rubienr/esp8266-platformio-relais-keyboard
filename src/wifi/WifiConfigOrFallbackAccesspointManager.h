#pragma once

#include "display/Display.h"

struct WifiConfigOrFallbackAccesspointManager
{
    explicit WifiConfigOrFallbackAccesspointManager(Display &display);

protected:
    Display &display;

    void printConnection(const char *ssid, const char *address);
};
