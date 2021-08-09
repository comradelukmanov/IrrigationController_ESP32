#pragma once
#include "header.h"

// Функция, проверяющая истекло ли время события.
// Возвращает 1, если событие ещё происходит.
// Возвращает 0, если событие завершилось.
bool isActive(hexTime *dst, uint8_t h, uint8_t m);