#pragma once
#include "header.h"
#include "LiquidCrystal_I2C.h"

// Простая текстовая метка.
// Хранит свою позицию на дисплее и текстовую метку.
struct label
{
    uint8_t x;        // Позиция метки в строке
    uint8_t y;        // Строка, в которой расположена метка
    uint8_t len;      // Длина текста метки
    const char *text; // Текст метки
};

// Ссылка.
// Хранит текстовую метку (тип label) и Указатель на ссылаемую функцию или процедуру.
struct ref
{
    uint8_t x;        // Позиция ссылки в строке
    uint8_t y;        // Строка, в которой расположена ссылка
    uint8_t len;      // Длина текста ссылки
    const char *text; // Текст ссылки
    void (*src)();    // Указатель на ссылаемую функцию или процедуру
};

// Поле ввода данных типа int.
// Хранит свою позизицию на дисплее и указатель на вводимую переменную.
struct inpField
{
    uint8_t x;   // Позиция поля ввода в строке
    uint8_t y;   // Строка, в которой расположено поле ввода
    uint8_t len; // Длина поля ввода
    int *var;    // Вводимая переменная
};

// Вывод элемента интерфейса на экран (текстовой метки label)
void printUIElement(LiquidCrystal_I2C *lcd, label *elem);
// Вывод элемента интерфейса на экран (ссылки label)
void printUIElement(LiquidCrystal_I2C *lcd, ref *elem);
// Вывод элемента интерфейса на экран (поля ввода inpField)
void printUIElement(LiquidCrystal_I2C *lcd, inpField *elem);