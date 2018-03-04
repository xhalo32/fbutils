#include "fbuffer.h"



int main()
{
	if (open_framebuffer())
	{
		return 1;
	}
	clear_framebuffer();
	
	printf("xres = %d, yres = %d\n",xres,yres);

	close_framebuffer();

	return 0;
}
