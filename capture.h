#ifndef CAPTURE_H
#define CAPTURE_H

#ifdef linux
/* Standard Include Files */
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/types.h>
#include <linux/videodev.h>
#include <sys/mman.h>
#include <string.h>
#include <malloc.h>

class Capture
{
private:
    //set the buffer numbers
    #define BUFFER_NUM 3
    struct testbuffer
    {
        unsigned char *start;
        size_t offset;
        unsigned int length;
    };
    struct testbuffer buffers[BUFFER_NUM];

    int g_width;
    int g_height;
    int g_size;
    int g_capture_count;
    int g_rotate;

    char *v4l_device;               //input
    int fd_v4l;
    unsigned char *ptcur;           //output

public:
    Capture(char * device = "/dev/video0");
    ~Capture();
    int preprocess(char *device);
    int process();
    int postprocess();
    void setDevice(char *device);
    int v4l2Start();
    int v4l2Setup(char *device);
    int printInfo();

    int setFmt();
    void setWidth(int width);
    void setHeight(int height);
    void setSize();
    int getWidth();
    int getHeight();
    unsigned char *getData();



};
#endif

#endif // CAPTURE_H
