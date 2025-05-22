#include "utils.h"

#include <stdint.h>

int16_t limit(int16_t val, int16_t low, int16_t high) {
    if (val > high)
        return high;
    else if (val < low)
        return low;
    else
        return val;
}

int16_t map(int16_t var, int16_t low1, int16_t high1, int16_t low2, int16_t high2) {
    int16_t d1 = high1 - low1, d2 = high2 - low2, res;
    
    if (d2 >= d1)
        res = low2 + (var - low1) * (d2 / d1);
    else
        res = low2 + (var - low1) / (d1 / d2);
    
    return limit(res, low2, high2);
    
//     if (high1 == low1 && high2 == l2 && var == low1)
//         return l2;
//     else if (high1 > l1 && high2 > l2 && var >= l1 && var <= high1)
//         return (var - l1) * (high2 - l2) / (high1 - l1) + l2;
}