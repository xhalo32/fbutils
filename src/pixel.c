#include "pixel.h"

void pixel(int x, int y, uint32_t color)
{
	*(tempbuffer + y*xres + x) = color;
}
