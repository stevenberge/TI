#ifdef linux

#include "capture.h"

Capture::Capture( char * device)
{
    v4l_device = device;
    fd_v4l = 0;
}

Capture::~Capture()
{
    if(ptcur)
        delete [] ptcur;
    postprocess();
}

int Capture::preprocess(char *device)
{
    printf("Capture preprocess capDevice:%s\n",device);
    if(v4l2Setup(device) < 0)
        return -1;
    if(v4l2Start() < 0)
        return -1;
    printInfo();
    return 0;
}

int Capture::process()
{
    struct v4l2_buffer buf;

    memset(&buf, 0, sizeof (buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (ioctl (fd_v4l, VIDIOC_DQBUF, &buf) < 0)
    {
        printf("VIDIOC_DQBUF failed.\n");
        return -1;
    }

    //printf("buf.index %d\n", buf.index);
    memcpy(ptcur, buffers[buf.index].start, g_size);

    if (ioctl (fd_v4l, VIDIOC_QBUF, &buf) < 0)
    {
        printf("VIDIOC_QBUF failed\n");
       return -1;
    }

    return 0;
}

int Capture::postprocess()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    return ioctl (fd_v4l, VIDIOC_STREAMOFF, &type);
}

void Capture::setDevice(char *device)
{
    v4l_device = device;
}

int Capture::v4l2Start()
{
    unsigned int i=0;
    struct v4l2_buffer buf;
    enum v4l2_buf_type type;

    for (i = 0; i < BUFFER_NUM; i++)
    {
        memset(&buf, 0, sizeof (buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.index = i;
        if (ioctl(fd_v4l, VIDIOC_QUERYBUF, &buf) < 0)
        {
               printf("%d:VIDIOC_QUERYBUF error\n",i);
               return -1;
        }

        buffers[i].length = buf.length;
        buffers[i].offset = (size_t) buf.m.offset;
        buffers[i].start = (unsigned char*)mmap (NULL, buffers[i].length,
           PROT_READ | PROT_WRITE, MAP_SHARED,
           fd_v4l, buffers[i].offset);
        if (buffers[i].start == MAP_FAILED) {
            printf("mmap error\n");
            return -1;
        }
        memset(buffers[i].start, 0xFF, buffers[i].length);
    }

    for (i = 0; i < BUFFER_NUM; i++)
    {
           memset(&buf, 0, sizeof (buf));
           buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
           buf.memory = V4L2_MEMORY_MMAP;
           buf.index = i;
           buf.m.offset = buffers[i].offset;

           if (ioctl (fd_v4l, VIDIOC_QBUF, &buf) < 0) {
                   printf("VIDIOC_QBUF error\n");
                   return -1;
           }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl (fd_v4l, VIDIOC_STREAMON, &type) < 0)
    {
        printf("VIDIOC_STREAMON error\n");
        return -1;
    }
    return 0;
}

int Capture::setFmt()
{
    struct v4l2_format fmt;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_OV511;
    fmt.fmt.pix.field= V4L2_FIELD_INTERLACED;
    fmt.fmt.pix.width = g_width;
    fmt.fmt.pix.height = g_height;
    if (ioctl(fd_v4l, VIDIOC_S_FMT, &fmt) < 0)
    {
        printf("set format V4L2_PIX_FMT_YUYV failed\n");
        perror("VIDIOC_S_FMT");
        return -1;
    }
    else printf("set format V4L2_PIX_FMT_YUYV success\n");
    return 0;
}

int Capture::v4l2Setup(char *device)
{
    printf("v4l2Setup capDevice:%s\n",device);
    setDevice(device);
    if ((fd_v4l = open(v4l_device, O_RDWR, 0)) < 0)
    {
        printf("Unable to open %s\n", v4l_device);
        return -1;
    }

    //设置摄像头尺寸
    setWidth(320);
    setHeight(240);
    setSize();

    if(setFmt() < 0)return -1;

    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof (req));
    req.count = BUFFER_NUM;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd_v4l, VIDIOC_REQBUFS, &req) < 0)
    {
        printf("v4l_capture_setup: VIDIOC_REQBUFS failed\n");
        return -1;
    }
    else printf("v4l_capture_setup: VIDIOC_REQBUFS success\n");

    return fd_v4l;
}


int Capture::printInfo()
{
    struct v4l2_format fmt;

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fd_v4l, VIDIOC_G_FMT, &fmt) < 0)
    {
            printf("get format failed\n");
            return -1;
    }
    else
    {
        printf("Width = %d\n", fmt.fmt.pix.width);
        printf("Height = %d\n", fmt.fmt.pix.height);
        printf("Image size = %d\n", fmt.fmt.pix.sizeimage);
        printf("pixelformat = %d\n", fmt.fmt.pix.pixelformat);
        printf("pixelformat(a) = %c\n", fmt.fmt.pix.pixelformat&0x0ff);
        printf("pixelformat(b) = %c\n", (fmt.fmt.pix.pixelformat&0x0ff00)>>8);
        printf("pixelformat(c) = %c\n", (fmt.fmt.pix.pixelformat&0x0ff0000)>>16);
        printf("pixelformat(d) = %c\n", (fmt.fmt.pix.pixelformat&0x0ff000000)>>24);
    }
    return 0;
}

void Capture::setWidth(int width)
{
    g_width = width;
}

void Capture::setHeight(int height)
{
    g_height = height;
}

int Capture::getWidth()
{
    return g_width;
}

int Capture::getHeight()
{
    return g_height;
}

void Capture::setSize()
{
    g_size = getWidth() * getHeight() * 2;
    ptcur = new unsigned char[g_size];
}

unsigned char *Capture::getData()
{
    return ptcur;
}

#endif

