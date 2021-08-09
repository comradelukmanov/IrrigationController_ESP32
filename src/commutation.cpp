#include "commutation.h"

bool isActive(hexTime *dst, uint8_t h, uint8_t m)
{
    if (dst->h * 60 + dst->m >= h * 60 + m)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}