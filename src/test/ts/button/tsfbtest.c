#include <linux/fb.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <tslib.h>
#include <time.h>

 
const char *FB_NAME = "/dev/fb0";
void*   m_FrameBuffer;
struct  fb_fix_screeninfo m_FixInfo;
struct  fb_var_screeninfo m_VarInfo;
int 	m_FBFD;

int main() {

    int iFrameBufferSize;
    /* Open the framebuffer device in read write */
    m_FBFD = open(FB_NAME, O_RDWR);
    if (m_FBFD < 0) {
    	printf("Unable to open %s.\n", FB_NAME);
    	return 1;
    }
    /* Do Ioctl. Retrieve fixed screen info. */
    if (ioctl(m_FBFD, FBIOGET_FSCREENINFO, &m_FixInfo) < 0) {
    	printf("get fixed screen info failed: %s\n",
    		  strerror(errno));
    	close(m_FBFD);
    	return 1;
    }
    /* Do Ioctl. Get the variable screen info. */
    if (ioctl(m_FBFD, FBIOGET_VSCREENINFO, &m_VarInfo) < 0) {
    	printf("Unable to retrieve variable screen info: %s\n",
    		  strerror(errno));
    	close(m_FBFD);
    	return 1;
    }
    printf("xres %d yres %d\n",m_VarInfo.xres,m_VarInfo.yres);

    /* Calculate the size to mmap */
    iFrameBufferSize = m_FixInfo.line_length * m_VarInfo.yres;
    printf("Line length %d\n", m_FixInfo.line_length);
    /* Now mmap the framebuffer. */
    m_FrameBuffer = mmap(NULL, iFrameBufferSize, PROT_READ | PROT_WRITE,
    				 MAP_SHARED, m_FBFD,0);
    if (m_FrameBuffer == NULL) {
    	printf("mmap failed:\n");
    	close(m_FBFD);
    	return 1;
    }


    /*
    {
        int *p = (int *) m_FrameBuffer;
        int x,y,t=0;
        while(1) {
            for (y=0; y<m_VarInfo.yres; y++) {
                for (x=0; x<m_VarInfo.xres; x++) {
	            unsigned int color = 0x00001 * y + 0x00001 * x + 0x000000 * t;
                    p[x + y * m_VarInfo.xres] = color;
                }
            }
            t++;
        }
    }*/
	int *p = (int *) m_FrameBuffer;
	unsigned int x,y;
	for (unsigned int i = 0; i < m_VarInfo.yres * m_VarInfo.xres; i++)
	{
		x = i % m_VarInfo.xres; y = i / m_VarInfo.xres;
		unsigned int color = 0x80 * ((float) x / m_VarInfo.xres) +
				0x80 * ((float) y / m_VarInfo.yres);
		p[i] = color;
	}

	struct tsdev *ts;
	ts = ts_open("/dev/input/event4", 1);
	
	struct ts_sample sample;
	ts_config(ts);

	while (1)
	{
		int pressed = 0;
		if (ts)
		{
			while(ts_read(ts, &sample, 1)) 
			{
				if (sample.pressure > 0) pressed = 1;
			}
			if (pressed)
			{
				int x = sample.x, y = sample.y;
				for (unsigned int dx = 0; dx < 10; ++dx)
				for (unsigned int dy = 0; dy < 10; ++dy)
				{
					if ((x+dx >= m_VarInfo.xres) || (y+dy >= m_VarInfo.yres)) continue;
					p[x+dx + 
				m_VarInfo.xres * (y+dy)] = 0xFFFFFF;
				}
			}
		}

		//usleep(20000);
	}
}
