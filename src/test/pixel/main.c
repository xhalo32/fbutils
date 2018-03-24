#include "pixel.h"



int main()
{
	if (open_framebuffer())
	{
		return 1;
	}
	clear_framebuffer();
	
	pixel(0,0,0x0000FFFF);
	pixel(xres-2,0,0x0000FFFF);
	pixel(0,yres-2,0x0000FFFF);
	pixel(xres-2,yres-2,0x0000FFFF);

	write_framebuffer();
	
	usleep(4000*1000);

	close_framebuffer();

	return 0;
}
