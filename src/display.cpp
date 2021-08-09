#include "display.h"

void printUIElement(LiquidCrystal_I2C *lcd, label *elem)
{
    lcd->setCursor(elem->x, elem->y);
    for (uint8_t i = 0; i < sizeof(elem->len); i++)
    {
        lcd->print(' ');
    }
    lcd->setCursor(elem->x, elem->y);
    lcd->print(elem->text);
}

void printUIElement(LiquidCrystal_I2C *lcd, ref *elem)
{
    lcd->setCursor(elem->x, elem->y);
    for (uint8_t i = 0; i < sizeof(elem->len); i++)
    {
        lcd->print(' ');
    }
    lcd->setCursor(elem->x, elem->y);
    lcd->print(elem->text);
}

void printUIElement(LiquidCrystal_I2C *lcd, inpField *elem)
{
    lcd->setCursor(elem->x, elem->y);
    for (uint8_t i = 0; i < elem->len; i++)
    {
        lcd->print('0');
    }
    uint8_t n = 1;
    int buf = *elem->var;
    for (uint8_t i = 1; i < elem->len; i++)
    {
        buf /= 10;
        if (buf)
        {
            n++;
        }
    }
    lcd->setCursor(elem->x + elem->len - n, elem->y);
    lcd->print(*elem->var);
}