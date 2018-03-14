#include "rectangle.h"
#include <time.h>
#include <signal.h>

// touchscreen support!
#include <tslib.h>

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

	struct tsdev *ts;
	ts = ts_open("/dev/input/event2", 1);
	
	struct ts_sample sample;
	ts_config(ts);
	
	srand(0);
	if (open_framebuffer())
	{
		return 1;
	}
	while (1)
	{
		int x = 0, y = 0;
		int pressed = 0;
		if (ts)
		{
			while(ts_read(ts, &sample, 1)) 
			{
				if (sample.pressure > 0)
				{
					pressed = 1;
					x = sample.x; y = sample.y;
				}
			}
		}
		clear_tempbuffer();

		for (int i = 0; i < NUM_RECTS; ++i)
		{
			if (pressed)
			{
				rectangle_fill(xres/2-40, yres/4-30, 80, 60, 0x00AFAFAF);
				rectangle_fill(x, yres*3/4-30, 80, 60, 0x00AA0000);
			}
			else
			{
				rectangle_fill(xres/2-40, yres/4-30, 80, 60, 0x00FFFFFF);
			}
		}

		write_framebuffer();

		usleep(100*1000);
	}
}
