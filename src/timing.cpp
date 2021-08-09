#include "timing.h"

uint8_t checkTiming(event *timing, uint8_t h, uint8_t m)
{
    for (short i = 0; i < 20; i++)
    {
        if ((h == timing[i].h) && (m == timing[i].m))
        {
            return timing[i].d;
        }
    }
    return 0;
}

void updateDstTime(hexTime *dst, event *timing, uint8_t h, uint8_t m)
{
    uint8_t d = checkTiming(timing, h, m);
    if (d)
    {
        dst->m = m + d;
        dst->h = h + dst->m / 60;
        dst->m = dst->m % 60;
    }
}