#include "circle.h"

float sin_chebyshev(float a)
{
	if (a>M_PI/2) a = M_PI - a;
	if (a<-M_PI/2) a = - M_PI - a;

	float a3 = a*a*a, a5 = a3*a*a, a7 = a5*a*a;
	
	return 0.99999660*a - 0.16664824*a3 + 0.00830629*a5 - 0.00018363*a7;
}
void circle(int x, int y, int r, uint32_t color)
{
	if (r < 1) return;
	for (float a = 0; a < 2*M_PI; a += 1 / (float) r)
	{
		pixel(x + round(r*sin_chebyshev(M_PI/2-a)-0.5),
			y + round(r*sin_chebyshev(a)-0.5), color);
	}
}
void circle_fill(int x, int y, int r, uint32_t color)
{
	if (r < 1) return;
	int r2 = r*r;
	for (int _y = 1; _y < r; ++_y)
	{
		for (int _x = r; _x > 0; --_x)
		{
			if (_x*_x + _y*_y <= r2)
			{
				for (int i = 0; i < 2*_x; ++i)
				{
					tempbuffer[(y - _y)*xres + x - _x + i] = color;
					tempbuffer[(y + _y)*xres + x - _x + i] = color;
				}
				break;
			}
		}
	}
	// This doesn't run any quicker
	/*
	for (int _y = 1; _y < r*M_SQRT1_2; ++_y)
	{
		for (int _x = r; _x > 0; --_x)
		{
			if (_x*_x + _y*_y <= r2)
			{
				for (int i = 0; i < 2*_x+1; ++i)
				{
					tempbuffer[(y - _y)*xres + x - _x + i] = color;
					tempbuffer[(y + _y)*xres + x - _x + i] = color;
				}
				for (int i = 0; i < 2*_y+1; ++i)
				{
					tempbuffer[(y - _x)*xres + x - _y + i] = color;
					tempbuffer[(y + _x)*xres + x - _y + i] = color;
				}
				break;
			}
		}
	}
	*/
	// middle row
	for (int i = 0; i < 2*r; ++i)
	{
		tempbuffer[y*xres + x - r + i] = color;
	}
}

