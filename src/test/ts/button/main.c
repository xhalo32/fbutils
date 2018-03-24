#include "rectangle.h"
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

// touchscreen library headers
#include <tslib.h>

#define BUTTON_PRESSED 1
#define BUTTON_ACTIVATED 2

#define NUM_BUTTONS 1

#define READ_SAMPLES 1
#define MAX_SLOTS 10

struct Button {
	int x, y, w, h;
	int pressed;
	char text[127];
};
static struct Button **buttonlist;

static struct tsdev *ts;

static pthread_t tid;
static sig_atomic_t running;

static void sig(int signum)
{
	running = 0;
	pthread_join(tid, NULL);
	close_framebuffer();
	ts_close(ts);
	fflush(stderr);
	printf("signal %d caught\n", signum);
	fflush(stdout);
	exit(1);
}
static struct Button *create_button(int x, int y, int w, int h, char* text)
{
	struct Button *temp;
	temp = (struct Button *) malloc(sizeof(struct Button));
	temp->x = x;
	temp->y = y;
	temp->w = w;
	temp->h = h;
	strcpy(temp->text,text);
	return temp;
}
void *draw(void *id)
{
	while (running)
	{
		clear_tempbuffer();

		// draw buttons
		for (int i = 0; i < NUM_BUTTONS; ++i)
		{
			//printf("%d %d %d %d\n",buttonlist[i]->x,buttonlist[i]->y,
			//		buttonlist[i]->w,buttonlist[i]->h);
			uint32_t color;
			if (buttonlist[i]->pressed)
				color = 0xAAAAAA;
			else
				color = 0xFFFFFF;
				
			rectangle_fill(buttonlist[i]->x, buttonlist[i]->y,
				buttonlist[i]->w, buttonlist[i]->h, color);
		}

		write_framebuffer();

		usleep(100*1000);
	}
}

int main()
{
	signal(SIGSEGV, sig);
	signal(SIGINT,	sig);
	signal(SIGTERM, sig);

	ts = ts_setup(NULL, 0);
	if (!ts) return 1;

	// initialize buttonlists
	printf("%d\n",sizeof(struct Button*));
	buttonlist = malloc(NUM_BUTTONS*sizeof(struct Button*));

	//memset(&buttonlist, 0, sizeof(buttonlist));
	struct ts_sample_mt **sample_mt;

	sample_mt = malloc(READ_SAMPLES * sizeof(struct ts_sample_mt**));
	for (int i = 0; i < READ_SAMPLES; i++)
	{
		sample_mt[i] = calloc(MAX_SLOTS, sizeof(struct ts_sample_mt));
	}
	
	if (open_framebuffer()) return 1;

	buttonlist[0] = create_button(
			xres/2 - 150, yres/2 - 100, 300, 200, "PRESS ME");

	running = 1;
	pthread_create(&tid, NULL, draw, NULL);
	while (1)
	{
		int x = 0, y = 0, pressure = 0;
		int ret;

		ret = ts_read_mt(ts, sample_mt, MAX_SLOTS, READ_SAMPLES);
		if (ret < 0)
		{
			printf("ts_read returned %d\n", ret);
			break;
		}
		if (ret == 1)
		{
			x = sample_mt[0][0].x;
			y = sample_mt[0][0].y;
			pressure = sample_mt[0][0].pressure;
			for (int i = 0; i < NUM_BUTTONS; ++i)
			{
				int inside = ((x >= buttonlist[i]->x)
						&& (y >= buttonlist[i]->y)
						&& (x < buttonlist[i]->x+buttonlist[i]->w)
						&& (y < buttonlist[i]->y+buttonlist[i]->h));

				if (pressure > 0) // touching
				{
					if (inside)
						buttonlist[i]->pressed = 1;
					else
						buttonlist[i]->pressed = 0;
				}
				else
					buttonlist[i]->pressed = 0;
			}
		}
	}

	// this only runs when break occurs
	pthread_join(tid, NULL);
	close_framebuffer();
	ts_close(ts);

	return 1;
}
