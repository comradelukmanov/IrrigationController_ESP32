#include "header.h"
#include <EEPROM.h>
#include "timing.h"
#include "commutation.h"
#include "display.h"
#include "iarduino_DHT.h"
#include "iarduino_RTC.h"

event timing11[32]; // Расписание 1 режима работы 1 клапана
event timing12[32]; // Расписание 1 режима работы 2 клапана
event timing13[32]; // Расписание 1 режима работы 3 клапана
event timing14[32]; // Расписание 1 режима работы 4 клапана

event timing21[32]; // Расписание 2 режима работы 1 клапана
event timing22[32]; // Расписание 2 режима работы 2 клапана
event timing23[32]; // Расписание 2 режима работы 3 клапана
event timing24[32]; // Расписание 2 режима работы 4 клапана

// Режим работы
uint8_t mode = 1;

hexTime dst1; // Время закрытия 1 клапана
hexTime dst2; // Время закрытия 2 клапана
hexTime dst3; // Время закрытия 3 клапана
hexTime dst4; // Время закрытия 4 клапана

// Объект часов реального времени
iarduino_RTC realTime(RTC_DS1307);

int ch; // Текущий час
int cm; // Текущая минута

// Таймер
unsigned long timer = millis();

// Курсор (Номер активного элемента интерфейса)
uint8_t UIpoint;

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
    realTime.gettime();
    ch = realTime.hours;
    cm = realTime.minutes;
}

void drawPage_sensors()
{
    
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
}

void setup()
{
    pinMode(RELAY_PIN0, OUTPUT);
    pinMode(RELAY_PIN1, OUTPUT);
    pinMode(RELAY_PIN2, OUTPUT);
    pinMode(RELAY_PIN3, OUTPUT);
    pinMode(RELAY_PIN4, OUTPUT);

    EEPROM.begin(240);
    readData();

    Serial.begin(115200);

    realTime.begin();

    lcd.init();
    lcd.init();
    lcd.backlight();

    ch = 17;
    cm = 0;
}

void loop()
{
    // Обновление времени выключения насоса и закрытия клапанов
    if (mode == 1)
    {
        updateDstTime(&dst1, timing11, ch, cm);
        updateDstTime(&dst2, timing12, ch, cm);
        updateDstTime(&dst3, timing13, ch, cm);
        updateDstTime(&dst4, timing14, ch, cm);
    }
    else
    {
        updateDstTime(&dst1, timing21, ch, cm);
        updateDstTime(&dst2, timing22, ch, cm);
        updateDstTime(&dst3, timing23, ch, cm);
        updateDstTime(&dst4, timing24, ch, cm);
    }

    if (millis() - timer >= 100)
    {
        drawPage_sensors();
        dht1.read();
        dht2.read();
        dht3.read();
        dht4.read();
        timeCounting();
        timer = millis();
    }

    if (isActive(&dst1, ch, cm) || isActive(&dst2, ch, cm) || isActive(&dst3, ch, cm) || isActive(&dst4, ch, cm))
    {
        digitalWrite(RELAY_PIN0, 1);
    }
    digitalWrite(RELAY_PIN1, !isActive(&dst1, ch, cm));
    digitalWrite(RELAY_PIN2, !isActive(&dst2, ch, cm));
    digitalWrite(RELAY_PIN3, !isActive(&dst3, ch, cm));
    digitalWrite(RELAY_PIN4, !isActive(&dst4, ch, cm));
}