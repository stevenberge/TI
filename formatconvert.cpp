#include "formatconvert.h"
#include "stdio.h"

FormatConvert::FormatConvert()
{

}

void FormatConvert::preprocess(int w, int h)
{
    this->setiWidth(w);
    this->setiHeight(h);
    imageSize = iWidth * iHeight;
    oWidth = iWidth;
    oHeight = iHeight;
    rgbData = new unsigned char[imageSize * 3];
    yuvData = new unsigned char[imageSize * 3];
    grayData = new unsigned char[imageSize];
    CreateTable();
}

void FormatConvert::process(unsigned char *data)
{
    iData = data;
    //YuyvToGray();
    YuyvToYuv();
    YuyvToRgb();
}

void FormatConvert::postprocess()
{
    delete [] rgbData;
    delete [] yuvData;
    delete [] grayData;
}

void FormatConvert::CreateTable()
{
    for(int ytemp = 0; ytemp < 256; ytemp ++)
    {
        for(int vtemp = 0; vtemp < 256; vtemp ++)
        {
            int rtemp = ytemp + 1.14 * (vtemp - 128);
            if(rtemp < 0)rtemp = 0;
            if(rtemp > 255)rtemp = 255;
            rTable[ytemp][vtemp] = rtemp;
        }

    }

    for(int utemp = 0; utemp < 256; utemp ++)
    {
        for(int vtemp = 0; vtemp < 256; vtemp ++)
        {
            int uvtemp = 0.395 * (utemp - 128) - 0.581 * (vtemp - 128) + 128;
            if(uvtemp < 0)uvtemp = 0;
            if(uvtemp > 255)uvtemp = 255;
            g_uvTable[utemp][vtemp] = uvtemp;
        }
    }

    for(int ytemp = 0; ytemp < 256; ytemp ++)
    {
        for(int uvtemp = 0; uvtemp < 256; uvtemp ++)
        {
            int gtemp = ytemp + uvtemp -128;
            if(gtemp < 0)gtemp = 0;
            if(gtemp > 255)gtemp = 255;
            gTable[ytemp][uvtemp] = gtemp;
        }
    }

    for(int ytemp = 0; ytemp < 256; ytemp ++)
    {
        for(int utemp = 0; utemp < 256; utemp ++)
        {
            int btemp = ytemp + 2.032 * (utemp - 128);
            if(btemp < 0)btemp = 0;
            if(btemp > 255)btemp = 255;
            bTable[ytemp][utemp] = btemp;
        }
    }

}

void FormatConvert::YuyvToYuv()
{
    oWidth = iWidth;
    oHeight = iHeight;

    int i;
    int pos = 0;
    int graypos = 0;

    for(i = 0; i < imageSize * 2;)
    {
        unsigned char ytemp1 = iData[i++];
        unsigned char utemp = iData[i++];
        unsigned char ytemp2 = iData[i++];
        unsigned char vtemp = iData[i++];

        grayData[graypos++] = ytemp1;
        yuvData[pos++] = ytemp1;
        yuvData[pos++] = utemp;
        yuvData[pos++] =  vtemp;

        grayData[graypos++] = ytemp2;
        yuvData[pos++] = ytemp2;
        yuvData[pos++] = utemp;
        yuvData[pos++] =  vtemp;
    }

}

void FormatConvert::YuyvToGray()
{
    oWidth = iWidth;
    oHeight = iHeight;

    int i;
    int pos = 0;

    for(i = 0; i < imageSize * 2;)
    {
        /*
        unsigned char ytemp1 = iData[i++];
        unsigned char utemp = iData[i++];
        unsigned char ytemp2 = iData[i++];
        unsigned char vtemp = iData[i++];
*/
        grayData[pos++] = iData[i++];
        i ++;
        grayData[pos++] = iData[i++];
        i ++;

    }

}
//use searching table method,convert to format RGBRGB
void FormatConvert::YuyvToRgb()
{
    oWidth = iWidth;
    oHeight = iHeight;

    for(int i = 0; i < imageSize * 2; i +=4)
    {
        int pos = i*2/4*3;
        unsigned char ytemp1 = iData[i];
        unsigned char ytemp2 = iData[i+2];
        unsigned char utemp = iData[i+1];
        unsigned char vtemp = iData[i+3];

        rgbData[pos] = rTable[ytemp1][vtemp];
        rgbData[pos + 1] = gTable[ytemp1][g_uvTable[utemp][vtemp]];
        rgbData[pos + 2] = bTable[ytemp1][utemp];

        rgbData[pos+3] = rTable[ytemp2][vtemp];
        rgbData[pos+4] = gTable[ytemp2][g_uvTable[utemp][vtemp]];
        rgbData[pos+5] = bTable[ytemp2][utemp];

    }

}

//use searching table method
void FormatConvert::Yuv420ToRgb()
{
    oWidth = iWidth;
    oHeight = iHeight;

    unsigned char *ydata = iData;
    unsigned char *udata = ydata + imageSize;
    unsigned char *vdata = udata + (imageSize >> 2);

    for(int i = 0; i < iHeight; i ++)
        for(int j = 0; j < iWidth; j ++)
        {
            int pos = i * iWidth + j;
            unsigned char ytemp = ydata[pos];
            unsigned char utemp = udata[i/2 * iWidth/2 + j/2];
            unsigned char vtemp = vdata[i/2 * iWidth/2 + j/2];

            int rtemp = ytemp + 1.14 * (vtemp - 128);
            int gtemp = ytemp + 0.395 * (utemp - 128) - 0.581 * (vtemp - 128);
            int btemp = ytemp + 2.032 * (utemp - 128);

            if(rtemp < 0)rtemp = 0;
            if(rtemp > 255)rtemp = 255;
            if(gtemp < 0)gtemp = 0;
            if(gtemp > 255)gtemp = 255;
            if(btemp < 0)btemp = 0;
            if(btemp > 255)btemp = 255;

            pos *= 3;
            rgbData[pos] = rTable[ytemp][vtemp];

            rgbData[pos + 1] = gTable[ytemp][g_uvTable[utemp][vtemp]];
            rgbData[pos + 2] = bTable[ytemp][utemp];

    }

}

void FormatConvert::setiWidth(int w)
{
    iWidth = w;
}

void FormatConvert::setiHeight(int h)
{
    iHeight = h;
}

int FormatConvert::getoWidth()
{
    return oWidth;
}

int FormatConvert::getoHeight()
{
    return oHeight;
}

void FormatConvert::setInData(unsigned char *data)
{
    iData = data;
}

unsigned char *FormatConvert::getGrayData()
{
    return grayData;
}

unsigned char *FormatConvert::getYuvData()
{
    return yuvData;
}

unsigned char *FormatConvert::getRgbData()
{
    return rgbData;
}

/*
r = 1.1643836*(ytemp- 16) + 1.5960268*(vtemp - 128);
g = 1.1643836*(ytemp- 16) - 0.39176229*(utemp - 128) - 0.81296765*(vtemp - 128);
b = 1.16438356*(ytemp- 16) + 2.0172321*(utemp - 128);

r = 1.16438356*(ytemp- 16) + 1.79274107*(cr - 128);
   g = 1.16438356*(yytemp - 16) - 0.21324861*(utemp - 128) - 0.53290933*(cr - 128);
   b = 1.16438356*(ytemp- 16) + 2.11240179*(utemp - 128);
*/
