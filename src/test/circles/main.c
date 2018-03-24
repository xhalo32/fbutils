#include "circle.h"
#include "rectangle.h"

#include <time.h>
#include <signal.h>

#define NUM_CIRCLES 50

static void sig(int signum)
{
	close_framebuffer();
	fflush(stderr);
	printf("signal %d caught\n", signum);
	fflush(stdout);
	exit(1);
}

int main()
{
	signal(SIGSEGV, sig);
	signal(SIGINT,  sig);
	signal(SIGTERM, sig);

	if (open_framebuffer())
	{
		return 1;
	}
	while (1)
	{
		clear_tempbuffer();
		for (int i = 0; i < NUM_CIRCLES; ++i)
		{
			int r = 2*yres/5;
			rectangle(xres/2-r,yres/2-r,2*r,2*r,(uint32_t) rand());
			uint32_t col = (uint32_t) rand();
			circle_fill(xres/2,yres/2,r,col);
			circle(xres/2,yres/2,r,~col);
		}

		write_framebuffer();

		usleep(100*1000);
	}
}
