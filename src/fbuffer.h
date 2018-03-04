#ifndef __FBUFFER_H
#define __FBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>

#include <linux/fb.h>

// set to 0 to not release virtual terminal nicely (1 requires superuser permissions)
#define RELEASE_VT 1
#if (RELEASE_VT == 1)
  #include <linux/vt.h>
  #include <linux/kd.h>
#endif // (RELEASE_VT == 1)

extern uint32_t xres, yres;
extern uint32_t *framebuffer;
extern uint32_t *tempbuffer;

int open_framebuffer();
void close_framebuffer();
void clear_framebuffer();
void clear_tempbuffer();
void write_framebuffer();

#endif // __FBUFFER_H
