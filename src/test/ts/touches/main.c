#include "circle.h"
#include <time.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdarg.h>

// touchscreen library headers
#include <tslib.h>

#define BUTTON_PRESSED 1
#define BUTTON_ACTIVATED 2

#define READ_SAMPLES 1
#define MAX_SLOTS 10

int _min(size_t n, int xs[])
{
	if (n == 0) exit(1);

	int m = xs[0];
	for(int i=0; i < n; i++) {
		if (xs[i] < m) m = xs[i];
	}
	return m;
}
#define min(arg1, ...) ({			   \
   int _x[] = { arg1, __VA_ARGS__ };   \
   _min(sizeof(_x)/sizeof(_x[0]), _x); \
})

static struct tsdev *ts;

static pthread_t tid;
static sig_atomic_t running;

struct Circle
{
	int x, y, r;
	uint32_t color;
	sig_atomic_t visible;
} circles[READ_SAMPLES][MAX_SLOTS];

typedef struct { int r; int g; int b; } RGB;
typedef struct { int h; int s; int v; } HSV;
RGB hsv2rgb(HSV hsv)
{
	RGB rgb;
	unsigned char region, remainder, p, q, t;

	if (hsv.s == 0)
	{
		rgb.r = hsv.v;
		rgb.g = hsv.v;
		rgb.b = hsv.v;
		return rgb;
	}
	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6; 
	p = (hsv.v * (255 - hsv.s)) >> 8;
	q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;
	switch (region)
	{
		case 0: rgb.r = hsv.v; rgb.g = t; rgb.b = p; break;
		case 1: rgb.r = q; rgb.g = hsv.v; rgb.b = p; break;
		case 2: rgb.r = p; rgb.g = hsv.v; rgb.b = t; break;
		case 3: rgb.r = p; rgb.g = q; rgb.b = hsv.v; break;
		case 4: rgb.r = t; rgb.g = p; rgb.b = hsv.v; break;
		default: rgb.r = hsv.v; rgb.g = p; rgb.b = q; break;
	}
	return rgb;
}
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
void *draw(void *id)
{
	while (running)
	{
		clear_tempbuffer();

		for (int i = 0; i < READ_SAMPLES; ++i)
			for (int j = 0; j < MAX_SLOTS; ++j)
				if (circles[i][j].visible)
				circle_fill(circles[i][j].x,circles[i][j].y,
					circles[i][j].r,circles[i][j].color);

		write_framebuffer();

		usleep(30*1000);
	}
}
int main()
{
	signal(SIGSEGV, sig);
	signal(SIGINT,	sig);
	signal(SIGTERM, sig);

	ts = ts_setup(NULL, 0);
	if (!ts) return 1;

	struct ts_sample_mt **sample_mt;

	sample_mt = malloc(READ_SAMPLES * sizeof(struct ts_sample_mt**));
	for (int i = 0; i < READ_SAMPLES; i++)
		sample_mt[i] = calloc(MAX_SLOTS, sizeof(struct ts_sample_mt));
	
	if (open_framebuffer()) return 1;

	running = 1;
	pthread_create(&tid, NULL, draw, NULL);
	while (running)
	{
		int x = 0, y = 0, pressure = 0;
		short valid = 0;
		int ret;

		ret = ts_read_mt(ts, sample_mt, MAX_SLOTS, READ_SAMPLES);
		if (ret < 0)
		{
			printf("ts_read returned %d\n", ret);
			running = 0;
			break;
		}
		for (int i = 0; i < READ_SAMPLES; ++i)
		{
			for (int j = 0; j < MAX_SLOTS; ++j)
			{
				x = sample_mt[i][j].x;
				y = sample_mt[i][j].y;
				pressure = sample_mt[i][j].pressure;
				valid = sample_mt[i][j].valid;

				if (!valid) continue;
				
				circles[i][j].visible = pressure;
				circles[i][j].x = x;
				circles[i][j].y = y;
				int close = min(xres-x,yres-y,x,y);

				circles[i][j].r = 20;
				if (close <= 20) circles[i][j].r = close - 1;

				HSV hsv; hsv = (HSV){.h = 255*j/MAX_SLOTS,.s = 255,.v = 255};
				RGB rgb = hsv2rgb(hsv);
				circles[i][j].color = (rgb.r << 16)+(rgb.g << 8)+(rgb.b);
			}
		}
		usleep(1*1000);
	}
	// this runs only when break occurs
	pthread_join(tid, NULL);
	close_framebuffer();
	ts_close(ts);

	return 1;
}
