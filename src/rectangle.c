#include "rectangle.h"

void rectangle(int x, int y, int w, int h, uint32_t color)
{
	for (int _x = 0; _x < w; ++_x)
	{
		pixel(x+_x,y,color);
		pixel(x+_x,y+h-1,color);
	}

	for (int _y = 1; _y < h-1; ++_y)
	{
		pixel(x,y+_y,color);
		pixel(x+w-1,y+_y,color);
	}
}
void rectangle_fill(int x, int y, int w, int h, uint32_t color)
{
	for (int _x = 0; _x < w; ++_x)
		for (int _y = 0; _y < h; ++_y)
		{
			pixel(x+_x, y+_y, color);
		}
}
