#include "header.h"
#include <EEPROM.h>
#include "LiquidCrystal_I2C.h"
#include "timing.h"
#include "commutation.h"
#include "iarduino_DHT.h"
#include "iarduino_RTC.h"

#include "menu.h"
#include "menuIO/serialOut.h"
#include "menuIO/serialIn.h"
#include "menuIO/altkeyIn.h"
#include "menuIO/chainStream.h"

using namespace Menu;

#define MAX_DEPTH 4

keyMap joystickBtn_map[] = {
    {btn_enter, defaultNavCodes[enterCmd].ch, INPUT_PULLUP},
    {btn_escape, defaultNavCodes[escCmd].ch, INPUT_PULLUP},
    {btn_up, defaultNavCodes[upCmd].ch, INPUT_PULLUP},
    {btn_down, defaultNavCodes[downCmd].ch, INPUT_PULLUP},
    {btn_right, defaultNavCodes[rightCmd].ch, INPUT_PULLUP},
    {btn_left, defaultNavCodes[leftCmd].ch, INPUT_PULLUP},
};

keyIn<6> joystickBtns(joystickBtn_map);

serialIn serial(Serial);
MENU_INPUTS(in, &joystickBtns, &serial);

MENU_OUTPUTS(out, MAX_DEPTH
             //  ,LIQUIDCRYSTAL_OUT(lcd,{0, 0, lcdColumns, lcdRows})
             ,
             SERIAL_OUT(Serial), NONE);

boolean NeedUpdate = true;
uint8_t NumUdp = 0;
result showEvent(eventMask e, navNode &nav, prompt &item)
{
  if (e == enterEvent)
  {
    NeedUpdate = false;
    NumUdp = 0;
  };
  if (e == exitEvent)
  {
    //	  rtc.adjust(DateTime(2021, 7, 21, timeHP, timeMP, timeSP));
    NeedUpdate = true;
  };
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}

result action2(eventMask e, navNode &nav, prompt &item)
{
  Serial.print("action2 event: ");
  Serial.print(e);
  Serial.println(", quiting menu.");
  Serial.flush();
  return quit;
  //return proceed;
}

char *const digit = "0123456789";
char *constMEM hexDigit MEMMODE = "0123456789";
char *constMEM hexNr[] MEMMODE = {"012", digit, ":", "012345", digit, " ", digit, digit, digit, digit, "m"};

char buf1[] = "00:00  0000s";

prompt *mainData223[24] = {
    new textField("t1", buf1, 7, hexNr, doNothing, noEvent),
    new textField("t2", buf1, 7, hexNr, doNothing, noEvent),
    new textField("t3", buf1, 7, hexNr, doNothing, noEvent),
    new textField("t4", buf1, 7, hexNr, doNothing, noEvent),
};

menuNode &subMenu221 = *new menuNode("K01 12:00 00c 24c87%", sizeof(mainData223) / sizeof(prompt *), mainData223, showEvent, anyEvent, showTitle);
menuNode &subMenu222 = *new menuNode("K02 12:00 00c 24c87%", sizeof(mainData223) / sizeof(prompt *), mainData223, showEvent, anyEvent, showTitle);
menuNode &subMenu223 = *new menuNode("K03 12:00 00c 24c87%", sizeof(mainData223) / sizeof(prompt *), mainData223, showEvent, anyEvent, showTitle);
menuNode &subMenu224 = *new menuNode("K04 12:00 00c 24c87%", sizeof(mainData223) / sizeof(prompt *), mainData223, showEvent, anyEvent, showTitle);

int isOn_1 = LOW; // Разрешено ли открытие 1 клапана
int isOn_2 = LOW; // Разрешено ли открытие 2 клапана
int isOn_3 = LOW; // Разрешено ли открытие 3 клапана
int isOn_4 = LOW; // Разрешено ли открытие 4 клапана

int ledCtrl_1 = LOW;
int ledCtrl02 = LOW;
int ledCtrl53 = LOW;
int ledCtrlt4 = LOW;

TOGGLE(ledCtrl_1, setLed31, "Led: ", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       ,
       VALUE("On", HIGH, doNothing, noEvent), VALUE("Off", LOW, doNothing, noEvent));

//TOGGLE(isOn1,setLed1,"K1: ",doNothing,noEvent,noStyle//,doExit,enterEvent,noStyle
//  ,VALUE("On",HIGH,doNothing,noEvent)
//  ,VALUE("Off",LOW,doNothing,noEvent)
//);

TOGGLE(ledCtrl02, setLed_2, "Med: ", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       ,
       VALUE("On", HIGH, doNothing, noEvent), VALUE("Off", LOW, doNothing, noEvent));

TOGGLE(ledCtrl53, setLed_3, "Sed: ", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       ,
       VALUE("On", HIGH, doNothing, noEvent), VALUE("Off", LOW, doNothing, noEvent));

TOGGLE(ledCtrlt4, setLed_4, "Ned: ", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       ,
       VALUE("On", HIGH, doNothing, noEvent), VALUE("Off", LOW, doNothing, noEvent));

result doAlert(eventMask e, prompt &item)
{
  //  nav.idleOn(alert);

  return proceed;
}

MENU(subMenu225, "Clapans: ", showEvent, anyEvent, noStyle
     //,SUBMENU(setLed)
     ,
     SUBMENU(setLed31), SUBMENU(setLed_2), SUBMENU(setLed_3), SUBMENU(setLed_4), EXIT("<Back"));

prompt *mainData22[5] = {
    &subMenu221,
    &subMenu222,
    &subMenu223,
    &subMenu224,
    &subMenu225};

//menuNode& mainMenu22=*new menuNode("Yasn 23:12:15 Nas:off",sizeof(mainData22)/sizeof(prompt*),mainData22,action2,anyEvent,wrapStyle);
menuNode &mainMenu22 = *new menuNode("Yasn 23:12:15 Nas:off", sizeof(mainData22) / sizeof(prompt *), mainData22, doNothing, noEvent, wrapStyle);

NAVROOT(nav, mainMenu22, MAX_DEPTH, in, out); //the navigation root object
navNode path[MAX_DEPTH];

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
  EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn_1) * 1, isOn_1);
  EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn_1) * 2, isOn_2);
  EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn_1) * 3, isOn_3);
  EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn_1) * 4, isOn_4);
  EEPROM.put(sizeof(timing11) * 8 + sizeof(isOn_1) * 5, mode);
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
  EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn_1) * 1, isOn_1);
  EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn_1) * 2, isOn_2);
  EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn_1) * 3, isOn_3);
  EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn_1) * 4, isOn_4);
  EEPROM.get(sizeof(timing11) * 8 + sizeof(isOn_1) * 5, mode);
}

void setup()
{
  // pinMode(RELAY_PIN0, OUTPUT);
  // pinMode(RELAY_PIN1, OUTPUT);
  // pinMode(RELAY_PIN2, OUTPUT);
  // pinMode(RELAY_PIN3, OUTPUT);
  // pinMode(RELAY_PIN4, OUTPUT);

  EEPROM.begin(1024);
  readData();

  Serial.begin(115200);
  Serial.println("OK");
  watch.begin();

  lcd.init();
  lcd.init();
  lcd.backlight();

  for (int x = 0; x <= 23; x++)
  {
    //strcpy(&buf1[x], "00:00 0000c");
    //mainData223[x]= new textField("t1",&buf1[1],7,hexNr,doNothing,noEvent);
  }

  //mainMenu22=*new menuNode("Yasn 20:02:10 Nas:off",sizeof(mainData22)/sizeof(prompt*),mainData22,action2,anyEvent,wrapStyle);

  //nav.useMenu(mainMenu22);
  nav.timeOut = 3; //made very short for demo purposes
  //nav.idleChanged = true;
  nav.showTitle = true;
  joystickBtns.begin();
}

void loop()
{
  nav.poll();

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