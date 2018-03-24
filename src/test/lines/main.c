#include "line.h"
#include <time.h>
#include <signal.h>

#define NUM_LINES 2000

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
		int x1, x2, y1, y2, w, h, r;
		clear_tempbuffer();
		for (int i = 0; i < NUM_LINES; ++i)
		{
			x1 = rand() % xres; x2 = rand() % xres;
			y1 = rand() % yres; y2 = rand() % yres;

			line(x1, y1, x2, y2, (uint32_t) rand());
		}

		write_framebuffer();
	}
}
