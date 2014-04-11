#ifdef linux
#include "capture.h"
#endif
#include "cameraprocess.h"
#include "formatconvert.h"
//#include "readimage.h"
#include <sys/time.h>

CameraProcess::CameraProcess()
{
#ifdef linux
    capture = new Capture();
    fmtCovt = new FormatConvert();
#endif

//    readImage = new ReadImage();

    framecount = 0;  //count the number of the frame
    bcount = false;  //判断是否是第一帧
    f = false;       //判断是否找到侦查为0的帧
    frame_max = 15;  //the number of the frame that the process can hold
    col_diff = 7;
    camera_focus = false;
}

#ifdef linux
void CameraProcess::runCamera()
{
    if (capture->preprocess(capDevice) < 0)
        return;
    fmtCovt->preprocess(capture->getWidth(), capture->getHeight());
    cols = capture->getWidth();
    rows = capture->getHeight();
    this->preprocess(fmtCovt->getoWidth(), fmtCovt->getoHeight());

    struct timeval t1, t2, t3, t4, t5;
    double timeuse;

    while (!stopped)
    {
        if (pause) continue;

       // gettimeofday(&t1,0);

        if (capture->process() < 0)
            return;

        //gettimeofday(&t2,0);
        //timeuse = 1000000*(t2.tv_sec - t1.tv_sec) + t2.tv_usec - t1.tv_usec;
        //timeuse /= 1000;
        //printf("capture time:%lfms\n", timeuse);

        fmtCovt->process(capture->getData());

        //gettimeofday(&t3,0);
        //timeuse = 1000000*(t3.tv_sec - t2.tv_sec) + t3.tv_usec - t2.tv_usec;
        //timeuse /= 1000;
        //printf("format convert time:%lfms\n", timeuse);

        this->process(fmtCovt->getRgbData(), fmtCovt->getGrayData());

        //gettimeofday(&t4,0);
        //timeuse = 1000000*(t4.tv_sec - t3.tv_sec) + t4.tv_usec - t3.tv_usec;
        //timeuse /= 1000;
        //printf("process time:%lfms\n", timeuse);

        if ( camera_focus == true )  //判断是否需要显示图片
        emit sendData(this->getOutData(), imgwidth, imgheight);
        //emit sendRect(centerx-box_width/2, centery-box_height/2,centerx+box_width/2, centery+box_height/2);

        //gettimeofday(&t5,0);
        //timeuse = 1000000*(t5.tv_sec - t4.tv_sec) + t5.tv_usec - t4.tv_usec;
        //timeuse /= 1000;
        //printf("send data time:%lfms\n", timeuse);

        //if (index == 0) pause = true;
        //index ++;

    }
    capture->postprocess();
    fmtCovt->postprocess();
    this->postprocess();
    stopped = true;
}
#endif

/*void DataProcess::runFiles()
{
    this->preprocess(readImage->getWidth(), readImage->getHeight());
    struct timeval t1, t2, t3, t4, t5;
    double timeuse;

    while (!stopped)
    {
        if (pause) continue;

        //gettimeofday(&t2,0);
            if (readImage->loadImage() == false) break;
        //gettimeofday(&t3,0);
        //timeuse = 1000000*(t3.tv_sec - t2.tv_sec) + t3.tv_usec - t2.tv_usec;
        //timeuse /= 1000;
        //printf("loadImage time:%lfms\n", timeuse);
            this->process(readImage->getRgbData(), readImage->getGrayData());

        //gettimeofday(&t4,0);
        //timeuse = 1000000*(t4.tv_sec - t3.tv_sec) + t4.tv_usec - t3.tv_usec;
        //timeuse /= 1000;
        //printf("process time:%lfms\n", timeuse);
            emit sendQImage(readImage->getQImage());
        //gettimeofday(&t5,0);
        //timeuse = 1000000*(t5.tv_sec - t4.tv_sec) + t5.tv_usec - t4.tv_usec;
        ///timeuse /= 1000;
        //printf("send image time:%lfms\n", timeuse);

        if (index == 0) pause = true;
        index ++;
        //sleep(1);

    }

    this->postprocess();
    stopped = true;

}*/

void CameraProcess::run()
{
#ifdef linux
    if (isCapture) this->runCamera();
#endif
//    if (isReadFiles) this->runFiles();
}

void CameraProcess::preprocess(int w, int h)
{
    imgwidth = w;
    imgheight = h;
    //isChange = false;
    //isSetbox = false;
    //isTrack = false;
    isCapture = false;
    //isReadFiles = false;
    stopped = false;
    pause = false;
    //index = 0;

    //left = 0;
    //top = 0;
    //right = 0;
    //down = 0;
}

void CameraProcess::copymat(  int mat1[][640], int matt[][640] )
{
        for ( int i = 0; i < rows; i++ )
                for ( int j = 0; j < cols; j++ )
                        mat1[i][j] = matt[i][j];
}
void CameraProcess::minusmat(  int matt[][640], int mat1[][640] )
{
        for ( int i = 0; i < rows; i++ )
                for ( int j = 0; j < cols; j++ )
                {
                        matt[i][j] = matt[i][j] - mat1[i][j];
                        if ( matt[i][j] < 0 ) matt[i][j] = -matt[i][j];
                }
}
void CameraProcess::medianmatblur( int matt[][640], int s )
{
    int r = s / 2;
    for ( int i = r; i < rows-r; i++ )
    {
        for ( int j = r; j < cols-r; j++ )
        {
            int sum_count = 0;
            if ( i - r == 0 && j - r == 0 )  sum_count = sum_mat[i+r][j+r];
            else if ( i - r == 0 )  sum_count = sum_mat[i+r][j+r] - sum_mat[i+r][j-r-1];
            else if ( j - r == 0 )  sum_count = sum_mat[i+r][j+r] - sum_mat[i-r-1][j+r];
            else sum_count = sum_mat[i+r][j+r] - sum_mat[i-r-1][j+r] - sum_mat[i+r][j-r-1] + sum_mat[i-r-1][j-r-1];
            if ( sum_count >= s * s - sum_count ) matt[i][j] = 255;
            else matt[i][j] = 0;
        }
    }
}
void CameraProcess::thresholdmat( int matt[][640], int s )
{
        for ( int i = 0; i < rows; i++ )
                for ( int j = 0; j < cols; j++ )
                {
                    sum_mat[i][j] = 0;
                    if ( matt[i][j] <= s )
                    {
                        matt[i][j] = 0;
                    }
                    else
                    {
                        matt[i][j] = 255;
                    }
                }
        for ( int i = 0; i < rows; i++ )
                for ( int j = 0; j < cols; j++ )
                {
                    int t = 0;
                    if ( matt[i][j] == 255 ) t = 1;
                    if ( i == 0 && j == 0 ) sum_mat[i][j] = t;
                    else if ( i == 0 )   sum_mat[i][j] = sum_mat[i][j-1] + t;
                    else if ( j == 0 )   sum_mat[i][j] = sum_mat[i-1][j] + t;
                    else sum_mat[i][j] = t + sum_mat[i-1][j] + sum_mat[i][j-1] - sum_mat[i-1][j-1];
                }
}
void CameraProcess::printmat( int matt[][640] )
{
    for ( int i = 0; i < rows; i++ )
    {
        for ( int j = 0; j < cols; j++ )
            if (matt[i][j] > 20) printf( "i:%d,j:%d,mat:%d\n",i,j, matt[i][j] );
    }
    printf("*****************************************\n");
}
void CameraProcess::mywork( int matt[][640] )
{
    //printmat( matt );
    copymat( mat3, matt );
    if ( bcount == true )
    {
        minusmat( matt, mat1 );

        thresholdmat( matt, 20 );

        medianmatblur( matt, 5);
        //printmat( matt );

        int count = 0;
        double x = 0;
        double y = 0;
        int maxcol = 0;
        int maxc = 0;
        for ( int i = 0; i < rows; i++ )
        {
            int count = 0;
            for ( int j = 0; j < cols; j++ )
            if ( matt[j][i] != 0 ) count++;
            if ( count > maxc )
            {
                maxcol = i;
                maxc = count;
            }
        }

        double crows = 0;
        int q = 0;
        for ( int i = 0; i < rows; i++ )
        if ( matt[i][maxcol] != 0 )
        {
            q++;
            crows += i;
        }
        if ( q != 0 && f )//scan doesn't end
        {
            bcount = true;
            copymat( mat1, mat3 );
            //frame1 = frame;
            return;
        }
        if ( q == 0 )
        {
            crows = 0;
            f = false;
        }
        else crows = crows / q;
        if ( q == 0 ) framecount = 0;
        else
        {
            node[framecount].x = crows;
            node[framecount].y = maxcol;
            framecount++;
            if ( framecount == frame_max )
            {
                int minrow = rows;
                int maxrow = 0;
                int poscol = 0;
                int negcol = 0;
                int midrow = 0;
                int variance = 0;
                for ( int i = 0; i < framecount; i++ )
                {
                    midrow += node[i].x;
                    if ( i > 0 )
                    {
                        if ( node[i].y > node[i-1].y )  poscol++;
                        else negcol++;
                    }
                }
                midrow = midrow / frame_max;
                for ( int i = 0; i < framecount; i++ )
                    variance += ( node[i].x - midrow ) * ( node[i].x - midrow );
                variance /= frame_max;
                if ( variance <= 3000 )   //设置方差
                {
                    int t = poscol - negcol;
                    if ( t < 0 ) t = -t;
                    if ( t >= col_diff )
                    {
                        if ( poscol > negcol )
                        {
                            printf( "from left to right\n");
                            emit prevPage();
                        }
                        else
                        {
                            printf( "from right to left\n");
                            emit nextPage();
                        }
                        f = true;
                    }
                }
              //  printf( "variance: %d\n", variance );
              //  printf( "poscol: %d, negcol: %d\n", poscol, negcol );
                for ( int i = 0; i < framecount - 1; i++ )
                    node[i] = node[i+1];
                    framecount--;
            }
            //printf("x:%.2lf , y:%d\n", crows, maxcol );
            //printf( "framecount: %d\n", framecount );
            //if ( f ) puts( "true" );
            puts("-------------------------------------");
        }
    }
    bcount = true;
    copymat( mat1, mat3 );
}

//处理函数
void CameraProcess::process(unsigned char *rgbData, unsigned char *GrayData)
{
    int width = capture->getWidth();
    int height = capture->getHeight();
    for ( int i = 0; i < height; i++ )
        for ( int j = 0; j < width; j++ )
        {
            matt[i][j] = GrayData[i*width+j];
        }
    mywork( matt );
    outData = rgbData;   //要显示的数据付给outData

}

void CameraProcess::postprocess()
{
#ifdef linux
    if (capture) delete [] capture;
    if (fmtCovt) delete [] fmtCovt;
#endif
}


/*void DataProcess::drawLine1(unsigned char *RgbData, int x1, int x2, int y, int linewidth)
{
    int linebegin, lineend;
    int i,j;

    for (j =0; j < linewidth; j++)
    {
        linebegin = ((y+j) * imgwidth + x1) * 3;
        lineend = ((y+j) * imgwidth + x2) * 3;

        for (i = linebegin; i <= lineend+4; )
        {
            RgbData[i++] = 255;
            RgbData[i++] = 0;
            RgbData[i++] = 0;
        }
    }
}

void DataProcess::drawLine2(unsigned char *RgbData, int x, int y1, int y2, int linewidth)
{
    int linebegin, lineend;
    int i,j;

    linebegin = (y1 * imgwidth + x) * 3;
    lineend = (y2 * imgwidth + x) * 3;

    for (i = linebegin; i < lineend; i+=imgwidth*3)
    {
        for (j = 0; j < linewidth; j++)
        {
            RgbData[i+3*j] = 255;
            RgbData[i+1+3*j] = 0;
            RgbData[i+2+3*j] = 0;
        }

    }
}

void DataProcess::drawRect(unsigned char *RgbData, int l, int t, int r, int d,int linewidth)
{
    drawLine1(RgbData, l, r, t, linewidth);
    drawLine1(RgbData, l, r, d, linewidth);
    drawLine2(RgbData, l, t, d, linewidth);
    drawLine2(RgbData, r, t, d, linewidth);
}

bool DataProcess::setImageDir()
{
    isReadFiles = true;
    isCapture = false;
    return readImage->setImageDir();
}
*/
/**************************************************************
no used in this class, trigger by class ImageWidget's slot
setDevice, char *device from class ImageWidget, after it,
start DataProcess's run by ImageWidget's slot setDevice.
***************************************************************/
void CameraProcess::setDevice(char *device)
{
    isCapture = true;
    //isReadFiles = false;
    capDevice = device;
    camera_focus = true;
    printf("DataProcess setDevice capDevice:%s\n",capDevice);
}

/**************************************************************
no used in this class, trigger by class ImageWidget's
mousePressEvent, x1,y1,x2,y2 from class ImageWidget.
***************************************************************/
/*void CameraProcess::setTrackBox(int x1, int y1, int x2, int y2)
{
    int temp;

    left = x1;
    top = y1;
    right = x2;
    down = y2;

    if (left > right)
    {
        temp = left;
        left = right;
        right = temp;
    }
    if (top > down)
    {
        temp = top;
        top = down;
        down = temp;
    }
    /*
    target.left = left;
    target.upper = top;
    target.width = right - left;
    target.height = down - top;

    /* if we set track box when tracking, not the first time,
        then set isChange true.
    if (isTrack) isChange = true;
    isSetbox = true;
    isTrack = false;
}
*/
/*void CameraProcess::setInData(unsigned char *data)
{
    inData = data;
}
*/
void CameraProcess::stopProcess()
{
    stopped = true;
}

/*void CameraProcess::setPause()
{
    if (pause == true) pause = false;
    else if (pause == false) pause = true;
}
*/
unsigned char *CameraProcess::getOutData()
{
    return outData;
}

