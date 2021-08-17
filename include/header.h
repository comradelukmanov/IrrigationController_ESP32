#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "types.h"

// Номер пина подключенного к реле насоса
#define RELAY_PIN0 6
// Номер пина подключенного к реле первого клапана
#define RELAY_PIN1 2
// Номер пина подключенного к реле второго клапана
#define RELAY_PIN2 3
// Номер пина подключенного к реле третьего клапана
#define RELAY_PIN3 4
// Номер пина подключенного к реле четвертого клапана
#define RELAY_PIN4 5

// Номер пина подключенного к 1 датчику DHT11
#define DHT1pin 16
// Номер пина подключенного к 2 датчику DHT11
#define DHT2pin 4
// Номер пина подключенного к 3 датчику DHT11
#define DHT3pin 0
// Номер пина подключенного к 4 датчику DHT11
#define DHT4pin 15

#define btn_left 12
#define btn_right 15
#define btn_up 24
#define btn_down 25
#define btn_enter 26
#define btn_escape 27