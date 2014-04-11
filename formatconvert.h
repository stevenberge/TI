#ifndef FORMATCONVERT_H
#define FORMATCONVERT_H

class FormatConvert
{
public:
    FormatConvert();
    void preprocess(int w, int h);
    void process(unsigned char *data);
    void postprocess();
    void CreateTable();
    void YuyvToGray();
    void YuyvToYuv();
    void YuyvToRgb();
    void Yuv420ToRgb();
    void setiWidth(int w);
    void setiHeight(int h);
    int getoWidth();
    int getoHeight();
    void setInData(unsigned char *data);
    unsigned char *getRgbData();
    unsigned char *getYuvData();
    unsigned char *getGrayData();

private:
    unsigned char rTable[256][256];
    unsigned char g_uvTable[256][256];
    unsigned char gTable[256][256];
    unsigned char bTable[256][256];
    unsigned char *iData;
    unsigned char *rgbData;
    unsigned char *yuvData;
    unsigned char *grayData;
    int iWidth;
    int iHeight;
    int imageSize;
    int oWidth;
    int oHeight;
};

#endif // FORMATCONVERT_H
