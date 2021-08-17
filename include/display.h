#pragma once
#include "header.h"
#include "LiquidCrystal_I2C.h"

void printText(LiquidCrystal_I2C *lcd, uint8_t x, uint8_t y, uint8_t len, const char *text);

void printInt(LiquidCrystal_I2C *lcd, uint8_t x, uint8_t y, uint8_t len, bool spacer, int var);
