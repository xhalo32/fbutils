#include "fbuffer.h"

#if (RELEASE_VT == 1)
static int con_fd;
static int last_vt;
#endif // (RELEASE_VT == 1)

static int fb_fd;
static struct fb_fix_screeninfo fix;
static struct fb_var_screeninfo var;
uint32_t xres, yres;

// only support for 4byte color mode
uint32_t *framebuffer;
uint32_t *tempbuffer;

static char *fbdevice = "/dev/fb0";
static char *consoledevice = "/dev/tty";

int open_framebuffer()
{
#if (RELEASE_VT == 1)
	struct vt_stat vts;
	char vtname[128];
	int fd, nr;

	sprintf(vtname, "%s%d", consoledevice, 1);
	fd = open(vtname, O_WRONLY);
	if (fd < 0)
	{
		perror("Could not open console device");
		return 1;
	}
	// get first available non-opened VT to int nr
	if (ioctl(fd, VT_OPENQRY, &nr) < 0)
	{
		close(fd);
		perror("Ioctl could not read VT_OPENQRY");
		return 1;
	}
	close(fd);

	sprintf(vtname, "%s%d", consoledevice, nr);
	con_fd = open(vtname, O_RDWR | O_NDELAY);
	if (con_fd < 0)
	{
		perror("Could not open tty");
		return 1;
	}
	// save last open VT
	if (ioctl(con_fd, VT_GETSTATE, &vts) == 0)
		last_vt = vts.v_active;

	// change VT to nr
	if (ioctl(con_fd, VT_ACTIVATE, nr) < 0)
	{
		perror("VT_ACTIVATE failed");
		close(con_fd);
		return 1;
	}
	if (ioctl(con_fd, KDSETMODE, KD_GRAPHICS) < 0)
	{
		perror("KDSETMODE failed");
		close(con_fd);
		return 1;
	}
#endif // (RELEASE_VT == 1)

	fb_fd = open(fbdevice, O_RDWR);
	if (fb_fd == -1)
	{
		perror("Could not open fb device");
		return 1;
	}
	if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &fix) < 0)
	{
		perror("Ioctl could not read fixed info");
		close(fb_fd);
		return 1;
	}
	if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &var) < 0)
	{
		perror("Ioctl could not read variable info");
		close(fb_fd);
		return 1;
	}
	if (var.bits_per_pixel != 32)
	{
		perror("Only 32bit color mode supported");
		close(fb_fd);
		return 1;
	}
	xres = var.xres;
	yres = var.yres;

	framebuffer = mmap(NULL,
		fix.smem_len,
		PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED,
		fb_fd, 0);
	if (framebuffer == (uint32_t *)-1) {
		perror("Could not mmap framebuffer");
		close(fb_fd);
		return 1;
	}
	tempbuffer = malloc(xres * yres * sizeof(*tempbuffer));

	return 0;
}
void close_framebuffer()
{
	clear_framebuffer();
	munmap(framebuffer, fix.smem_len);
	close(fb_fd);

	free(tempbuffer);

#if (RELEASE_VT == 1)
	if (strcmp(consoledevice, "none") != 0)
	{
		if (ioctl(con_fd, KDSETMODE, KD_TEXT) < 0)
		perror("KDSETMODE failed");

		if (last_vt >= 0)
		if (ioctl(con_fd, VT_ACTIVATE, last_vt))
			perror("VT_ACTIVATE failed");

		close(con_fd);
	}
#endif // (RELEASE_VT == 1)

	xres = 0;
	yres = 0;
}
void clear_framebuffer()
{
	memset(framebuffer, 0, fix.smem_len);
}
void clear_tempbuffer()
{
	memset(tempbuffer, 0, fix.smem_len);
}
void write_framebuffer()
{
	memcpy(framebuffer, tempbuffer, fix.smem_len);
}
