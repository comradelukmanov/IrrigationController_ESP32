#include "header.h"
#include <EEPROM.h>
#include "LiquidCrystal_I2C.h"
#include "timing.h"
#include "commutation.h"
#include "iarduino_DHT.h"
#include "iarduino_RTC.h"

#include <menu.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#include <menuIO/altkeyIn.h>
#include <menuIO/chainStream.h>

event timing11[32]; // Расписание 1 режима работы 1 клапана
event timing12[32]; // Расписание 1 режима работы 2 клапана
event timing13[32]; // Расписание 1 режима работы 3 клапана
event timing14[32]; // Расписание 1 режима работы 4 клапана

event timing21[32]; // Расписание 2 режима работы 1 клапана
event timing22[32]; // Расписание 2 режима работы 2 клапана
event timing23[32]; // Расписание 2 режима работы 3 клапана
event timing24[32]; // Расписание 2 режима работы 4 клапана

// Режим работы
bool mode = 0;

bool isOn1 = 0; // Разрешено ли открытие 1 клапана
bool isOn2 = 0; // Разрешено ли открытие 2 клапана
bool isOn3 = 0; // Разрешено ли открытие 3 клапана
bool isOn4 = 0; // Разрешено ли открытие 4 клапана

hexTime dst1; // Время закрытия 1 клапана
hexTime dst2; // Время закрытия 2 клапана
hexTime dst3; // Время закрытия 3 клапана
hexTime dst4; // Время закрытия 4 клапана

// Объект часов реального времени
iarduino_RTC watch(RTC_DS1307);

// Таймер
unsigned long timer1 = millis();

// Таймер обработки клавиатуры
unsigned long timer2 = millis();

// Курсор (Номер активного элемента интерфейса)
uint8_t UIpoint;

// Номер отображаемой страницы
uint8_t page = 0;

// Массив хранящий последнее и предыдущее состояние клавиатуры
bool keyboard[6][2];

// Объект дисплея LCD2004
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Объект 1 датчика DHT
iarduino_DHT dht1(DHT1pin);
// Объект 2 датчика DHT
iarduino_DHT dht2(DHT2pin);
// Объект 3 датчика DHT
iarduino_DHT dht3(DHT3pin);
// Объект 4 датчика DHT
iarduino_DHT dht4(DHT4pin);

// Отсчёт виртуального времени
void timeCounting()
{
    watch.gettime();
    watch.Hours = watch.hours;
    watch.minutes = watch.minutes;
}

void saveData()
{
    EEPROM.put(sizeof(timing11) * 0, timing11);
    EEPROM.put(sizeof(timing11) * 1, timing12);
    EEPROM.put(sizeof(timing11) * 2, timing13);
    EEPROM.put(sizeof(timing11) * 3, timing14);
    EEPROM.put(sizeof(timing11) * 4, timing21);
    EEPROM.put(sizeof(timing11) * 5, timing22);
    EEPROM.put(sizeof(timing11) * 6, timing23);
    EEPROM.put(sizeof(timing11) * 7, timing24);
    EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn1) * 1, isOn1);
    EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn1) * 2, isOn2);
    EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn1) * 3, isOn3);
    EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn1) * 4, isOn4);
    EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn1) * 5, mode);
}

void readData()
{
    EEPROM.get(sizeof(timing11) * 0, timing11);
    EEPROM.get(sizeof(timing11) * 1, timing12);
    EEPROM.get(sizeof(timing11) * 2, timing13);
    EEPROM.get(sizeof(timing11) * 3, timing14);
    EEPROM.get(sizeof(timing11) * 4, timing21);
    EEPROM.get(sizeof(timing11) * 5, timing22);
    EEPROM.get(sizeof(timing11) * 6, timing23);
    EEPROM.get(sizeof(timing11) * 7, timing24);
    EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn1) * 1, isOn1);
    EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn1) * 2, isOn2);
    EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn1) * 3, isOn3);
    EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn1) * 4, isOn4);
    EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn1) * 5, mode);
}

void setup()
{
    // pinMode(RELAY_PIN0, OUTPUT);
    // pinMode(RELAY_PIN1, OUTPUT);
    // pinMode(RELAY_PIN2, OUTPUT);
    // pinMode(RELAY_PIN3, OUTPUT);
    // pinMode(RELAY_PIN4, OUTPUT);
    // pinMode(btn_left, INPUT_PULLUP);
    // pinMode(btn_right, INPUT_PULLUP);
    // pinMode(btn_up, INPUT_PULLUP);
    // pinMode(btn_down, INPUT_PULLUP);
    // pinMode(btn_enter, INPUT_PULLUP);
    // pinMode(btn_escape, INPUT_PULLUP);

    EEPROM.begin(1024);
    readData();

    Serial.begin(115200);

    watch.begin();

    lcd.init();
    lcd.init();
    lcd.backlight();
}

void loop()
{

    for (int i = 0; i < 6; i++)
    {
        keyboard[i][0] = keyboard[i][1];
    }
    keyboard[0][1] = digitalRead(btn_left);
    keyboard[1][1] = digitalRead(btn_right);
    keyboard[2][1] = digitalRead(btn_up);
    keyboard[3][1] = digitalRead(btn_down);
    keyboard[4][1] = digitalRead(btn_enter);
    keyboard[5][1] = digitalRead(btn_escape);

    // Обновление времени выключения насоса и закрытия клапанов
    if (mode == 1)
    {
        updateDstTime(&dst1, timing11, watch.Hours, watch.minutes);
        updateDstTime(&dst2, timing12, watch.Hours, watch.minutes);
        updateDstTime(&dst3, timing13, watch.Hours, watch.minutes);
        updateDstTime(&dst4, timing14, watch.Hours, watch.minutes);
    }
    else
    {
        updateDstTime(&dst1, timing21, watch.Hours, watch.minutes);
        updateDstTime(&dst2, timing22, watch.Hours, watch.minutes);
        updateDstTime(&dst3, timing23, watch.Hours, watch.minutes);
        updateDstTime(&dst4, timing24, watch.Hours, watch.minutes);
    }

    if (millis() - timer1 >= 250)
    {
        dht1.read();
        dht2.read();
        dht3.read();
        dht4.read();

        watch.gettime();

        timeCounting();
        timer1 = millis();
    }

    // if (isActive(&dst1, watch.Hours, watch.minutes, isOn1) || isActive(&dst2, watch.Hours, watch.minutes, isOn2) || isActive(&dst3, watch.Hours, watch.minutes, isOn3) || isActive(&dst4, watch.Hours, watch.minutes, isOn4))
    // {
    //     digitalWrite(RELAY_PIN0, 1);
    // }
    // digitalWrite(RELAY_PIN1, !isActive(&dst1, watch.Hours, watch.minutes, isOn1));
    // digitalWrite(RELAY_PIN2, !isActive(&dst2, watch.Hours, watch.minutes, isOn2));
    // digitalWrite(RELAY_PIN3, !isActive(&dst3, watch.Hours, watch.minutes, isOn3));
    // digitalWrite(RELAY_PIN4, !isActive(&dst4, watch.Hours, watch.minutes, isOn4));
}