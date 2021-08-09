#pragma once
#include "header.h"

// Функция проверки наличия в расписании timing события с данным временем (h часов, m минут).
// Возвращает продолжительность события в минутах. Если событие не найдено, возвращает 0
uint8_t checkTiming(event *timing, uint8_t h, uint8_t m);

// Процедура обновления времени окончания события.
// Сохраняет в dst сумму данного времени (h часов, m минут) и продолжительности события
void updateDstTime(hexTime *dst, event *timing, uint8_t h, uint8_t m);