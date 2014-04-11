#ifndef CAMERAPROCESS_H
#define CAMERAPROCESS_H

#include <QThread>
#include <QImage>

#ifdef linux
class Capture;
class FormatConvert;
#endif
class CameraProcess: public QThread
{
    Q_OBJECT
public:
    CameraProcess();
    void preprocess(int w, int h);
    void process(unsigned char *RgbData, unsigned char *GrayData);
    void postprocess();
//    void drawLine1(unsigned char *RgbData, int x1, int x2, int y, int linewidth);
//    void drawLine2(unsigned char *RgbData, int x, int y1, int y2, int linewidth);
//    void drawRect(unsigned char *RgbData, int l, int t, int r, int d,int linewidth);
//    bool setImageDir();
    void setDevice(char *device);
//    void setTrackBox(int x1, int y1, int x2, int y2);
//    void setInData(unsigned char *data);
    void stopProcess();
    void setPause();
    void mywork( int matt[][640] );
    void printmat( int matt[][640] );
    void thresholdmat( int matt[][640], int s );
    void medianmatblur( int matt[][640], int s );
    void minusmat(  int matt[][640], int mat1[][640] );
    void copymat(  int mat1[][640], int matt[][640] );

    bool camera_focus;


    unsigned char *getOutData();
signals:
    void sendData(unsigned char *data, int width, int height);
    void prevPage();
    void nextPage();

    //    void sendQImage(const QImage &);
protected:
    void run();
private:
#ifdef linux
    void runCamera();
#endif
//    void runFiles();
private:
//    int left, top, right, down;
//    int sleft, stop, sright, sdown;
    int imgwidth;
    int imgheight;
    unsigned char *inData;
    unsigned char *outData;
    volatile bool stopped;
    volatile bool pause;
    unsigned long index;
//    bool isChange;
//    bool isSetbox;
//    bool isTrack;
    bool isCapture;
//    bool isReadFiles;
    char *capDevice;
    int matt[480][640];
    int mat1[480][640];
    int mat2[480][640];
    int mat3[480][640];
    int sum_mat[480][640];
    int width,height;
    int framecount;
    bool bcount;
    bool f;
    int rows, cols;
    int ss[1000000];
    struct node1{
        int x,y;
    }node[1000000];
    int frame_max;
    int col_diff;
#ifdef linux
    Capture *capture;
    FormatConvert *fmtCovt;
#endif
    //Tld tld;
    //Rect target;
//    ReadImage *readImage;

};


#endif // CAMERAPROCESS_H
