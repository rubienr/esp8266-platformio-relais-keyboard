#pragma once

#include <Adafruit_SSD1306.h>

template<uint8_t w, uint8_t h, uint8_t c, const uint8_t *b>
struct SymbolBitmap
{
    constexpr static const int16_t width{w};
    constexpr static const int16_t height{h};
    constexpr static const uint16_t color{c};
    constexpr static const uint8_t *bitmap{b};
};

struct Dimension
{

    Dimension(int16_t width, int16_t height) : width(width), height(height)
    {}

    int16_t width{0};
    int16_t height{0};
};

struct Rectangle : public Dimension
{

    Rectangle(int16_t x, int16_t y, int16_t width, int16_t height) : Dimension(width, height), x(x), y(y)
    {}

    int16_t x{0};
    int16_t y{0};
};

class Display : public Adafruit_SSD1306
{
public:
    //Display(uint8_t address = 0x3C, uint8_t sda = D2, uint8_t scl = D1);
    Display();

    void setup();

    void reset();

    /*!
     * Sets the cursor to the position. Negative coordinates are treated from right to left or bottom to top;
     * @param x new cursor position
     * @param y new cursor position
     */
    void setCursorPosition(int16_t x, int16_t y);

    Dimension getCharDimension(char c = 'm');

    template<class T>
    //, typename std::enable_if<std::is_base_of<SymbolBitmap<T::width, T::height, T::color, T::bitmap>, T>::value>::type>
    void drawBitmap(int16_t x = 0, int16_t y = 0)
    {
        Adafruit_SSD1306::drawBitmap(x, y, T::bitmap, T::width, T::height, T::color);
    }

protected:
    Dimension char_dimension{0, 0};
    Dimension status_bar_dimension{0, 0};
    Dimension print_dimension{0, 0};
    Rectangle print_area{0, 0, 0, 0};
};

