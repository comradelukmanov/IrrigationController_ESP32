#include "display.h"

void printText(LiquidCrystal_I2C *lcd, uint8_t x, uint8_t y, uint8_t len, const char *text)
{
    lcd->setCursor(x, y);
    for (uint8_t i = 0; i < sizeof(len); i++)
    {
        lcd->print(' ');
    }
    lcd->setCursor(x, y);
    lcd->print(text);
}

void printInt(LiquidCrystal_I2C *lcd, uint8_t x, uint8_t y, uint8_t len, bool spacer, int var)
{
    lcd->setCursor(x, y);
    if (spacer)
    {
        for (uint8_t i = 0; i < len; i++)
        {
            lcd->print(' ');
        }
    }
    else
    {
        for (uint8_t i = 0; i < len; i++)
        {
            lcd->print('0');
        }
    }

    uint8_t n = 1;
    float buf = var;
    for (uint8_t i = 1; i < len; i++)
    {
        buf /= 10;
        if (buf > 0)
        {
            n++;
        }
    }
    lcd->setCursor(x + len - n, y);
    lcd->print(var);
}