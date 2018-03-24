#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <tslib.h>

#define READ_SAMPLES 1
#define MAX_SLOTS 5

static struct tsdev *ts;

static void sig(int signum)
{
	ts_close(ts);
	fflush(stderr);
	printf("signal %d caught\n", signum);
	fflush(stdout);
	exit(1);
}

int main()
{
	signal(SIGSEGV, sig);
	signal(SIGINT,	sig);
	signal(SIGTERM, sig);

	ts = ts_setup(NULL, 0);
	if (!ts) return 1;

	struct ts_sample_mt **samp_mt = NULL;
	int i, j;
	int ret;

	ts = ts_setup(NULL, 0);
	if (!ts)
		return -1;

	samp_mt = malloc(READ_SAMPLES * sizeof(struct ts_sample_mt **));
	for (i = 0; i < READ_SAMPLES; i++) {
		samp_mt[i] = calloc(MAX_SLOTS, sizeof(struct ts_sample_mt));
	}

	while(1) {
		ret = ts_read_mt(ts, samp_mt, MAX_SLOTS, READ_SAMPLES);
		for (i = 0; i < ret; i++) {
			printf("sample nr %d\n", i);
			for (j = 0; j < MAX_SLOTS; j++) {
				printf(
				"%d slot %d X %d Y %d P %d\n",
				samp_mt[i][j].valid,
				samp_mt[i][j].slot,
				samp_mt[i][j].x,
				samp_mt[i][j].y,
				samp_mt[i][j].pressure);
			}
		}
	}
	ts_close(ts);

	return 1;
}
