#include "line.h"

uint32_t fast_sqrt(uint32_t a_nInput)
{
    uint32_t op  = a_nInput;
    uint32_t res = 0;
    uint32_t one = 1uL << 30; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type
    // "one" starts at the highest power of four <= than the argument.
    while (one > op)
    {
        one >>= 2;
    }

    while (one != 0)
    {
        if (op >= res + one)
        {
            op = op - (res + one);
            res = res +  2 * one;
        }
        res >>= 1;
        one >>= 2;
    }
    return res;
}

void line(int x1, int y1, int x2, int y2, uint32_t color)
{
	int w = x2-x1, h = y2-y1;
	int len = fast_sqrt(w*w + h*h);
	
	float xp1 = w / (float) len, yp1 = h / (float) len;
	for (int i = 0; i < len; ++i)
	{
		pixel(round(x1+i*xp1), round(y1+i*yp1), color);
	}
}
