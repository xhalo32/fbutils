#include "rectangle.h"
#include <time.h>
#include <signal.h>

#define NUM_RECTS 50

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

	srand(0);
	if (open_framebuffer())
	{
		return 1;
	}
	while (1)
	{
		int x1, x2, y1, y2, w, h;
		clear_tempbuffer();
		for (int i = 0; i < NUM_RECTS; ++i)
		{
			x1 = rand() % xres; x2 = rand() % xres;
			y1 = rand() % yres; y2 = rand() % yres;

			w = abs(x2-x1);
			h = abs(y2-y1);
			if (x1 > x2) x1 = x2;
			if (y1 > y2) y1 = y2;
			if (rand()%2)
				rectangle(x1, y1, w, h, (uint32_t) rand());
			else
				rectangle_fill(x1, y1, w, h, (uint32_t) rand());
		}

		write_framebuffer();

		usleep(100*1000);
	}
}
