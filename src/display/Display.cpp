#include "Display.h"

#include <Adafruit_GFX.h>

namespace
{
    GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c)
    {
#ifdef __AVR__
        return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
        // expression in __AVR__ section may generate "dereferencing type-punned pointer will break strict-aliasing rules" warning
        // In fact, on other platforms (such as STM32) there is no need to do this pointer magic as program memory may be read in a usual way
        // So expression may be simplified
        return gfxFont->glyph + c;
#endif //__AVR__
    }
}

Display::Display() : Adafruit_SSD1306(0)
{
    char_dimension = getCharDimension();
    status_bar_dimension = Dimension{width(), 8};
    print_dimension = Dimension(width(), height() - status_bar_dimension.height);
    print_area = Rectangle(0, status_bar_dimension.height, width(), height() - status_bar_dimension.height);
}

void Display::setup()
{
    begin(SSD1306_SWITCHCAPVCC, 0x3C);
    dim(true);
    setTextSize(1);
    setTextColor(WHITE);
    display();
    setCursor(print_area.x, print_area.y);
    dim(false);
}

void Display::reset()
{
    Rectangle &s = print_area;
    fillRect(s.x, s.y, s.width, s.height, BLACK);
    setCursorPosition(0, 8);
}


void Display::setCursorPosition(int16_t x, int16_t y)
{
    if (x < 0)
    {
        x = width() + x;
    }

    if (y < 0)
    {
        y = height() + y;
    }
    setCursor(x, y);
}

Dimension Display::getCharDimension(char c)
{
    int16_t x = 0, y = 0, minx = 0, miny = 0, maxx = 0, maxy = 0;
    charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    int16_t width = maxx - minx;
    int16_t height = maxy - miny;

    return Dimension{width, height};
}




