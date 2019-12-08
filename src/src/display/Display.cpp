#include "Display.h"

#include <Adafruit_GFX.h>

Display::Display() : Adafruit_SSD1306(0)
{};

void Display::setup()
{
    begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display();
    setTextSize(1);
    setTextColor(WHITE);
    setCursor(0, 0);
}
