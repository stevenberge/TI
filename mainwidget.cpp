#include "mainwidget.h"
#include "imagewidget.h"
#include "cameraprocess.h"
#include "bluetoothprocess.h"
#include "textedit.h"
#include <QStackedLayout>

MainWidget::MainWidget()
{
    //set up the button
    calibCamBtn = new QPushButton;
    calibCamBtn->setText("Camera");
    calibBTBtn = new QPushButton;
    calibBTBtn->setText("Bluetooth");
    startBtn = new QPushButton;
    startBtn->setText("Start");
    deviceCombo = new QComboBox;
    addDevices();
    openBtn = new QPushButton;
    openBtn->setText("OpenFile");
    imageViewer = new ImageWidget;

    //set up the layout
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(deviceCombo, 0, 0, 1, 2);
    gridLayout->addWidget(calibCamBtn, 0, 3, 1, 1);
    gridLayout->addWidget(calibBTBtn, 0, 4, 1, 1);
    gridLayout->addWidget(startBtn, 0, 5, 1, 1);
    gridLayout->addWidget(openBtn, 0, 6, 1, 1);
    QVBoxLayout *vlayOut = new QVBoxLayout;

    textEdit = new TextEdit;

    //使用stackedlayout分类，将蓝牙跟摄像头交给imageViewer解决，将文本将给另一个widget解决
    QStackedLayout *stackLayout = new QStackedLayout;
    stackLayout->addWidget(imageViewer);
    stackLayout->addWidget(textEdit);
    connect( this, SIGNAL(changeStack(int)), stackLayout, SLOT(setCurrentIndex(int)));
    vlayOut->addLayout(stackLayout);
    vlayOut->addLayout(gridLayout);
    setLayout(vlayOut);

    //set up the threads
    cameraProcess = new CameraProcess;
    connect(cameraProcess, SIGNAL(sendData(unsigned char*,int ,int)), this, SLOT(setImage(unsigned char*,int,int)));
    connect(calibCamBtn, SIGNAL(clicked()), this, SLOT(setDevice()));

    bluetoothProcess = new BluetoothProcess;
    connect(calibBTBtn, SIGNAL(clicked()), this, SLOT(setBT()));
    connect( bluetoothProcess, SIGNAL(sendData( int, int )), this, SLOT(processBT(int, int )));

    connect( startBtn, SIGNAL(clicked()), this, SLOT(setOpen()));
    connect( openBtn, SIGNAL(clicked()), textEdit, SLOT(fileOpen()));
    emit changeStack(0); //一开始设置显示第一界面


    //connect
    connect( cameraProcess, SIGNAL(prevPage()), textEdit, SLOT(prevPage()));
    connect( cameraProcess, SIGNAL(nextPage()), textEdit, SLOT(nextPage()));

}

void MainWidget::setOpen()
{
    emit changeStack(1);
}

void MainWidget::processBT( int x, int y )
{
    if ( record_BT < 4 )
    {
        node[record_BT].x = x;
        node[record_BT].y = y;
        record_BT++;
        setBluetooth( record_BT );
        if ( record_BT == 4 )
        {
            left_point = ( node[0].y + node[2].y ) / 2;
            right_point = ( node[1].y + node[3].y ) / 2;
            up_point = ( node[0].x + node[1].x ) / 2;
            down_point = ( node[2].x + node[3].x ) / 2;
            printf("left %d,right %d,up %d,down %d\n",left_point,right_point,up_point,down_point);
        }
    }
    else
    {
        //防止点超过边界
        if ( y > left_point ) y = left_point;
        if ( y < right_point ) y = right_point;
        if ( x < up_point ) x = up_point;
        if ( x > down_point ) x = down_point;
        printf( "%d %d\n",x,y);
        //int f = imageViewer->getFocus();
        //if ( f == 3 )
        {
            //发射信号给文档
            double x_ratio = (double)( x - up_point ) / ( down_point - up_point );
            double y_ratio = (double)( y - left_point ) / ( right_point - left_point );
            printf( "trans %.2lf %.2lf\n", x_ratio , y_ratio );

            printf( "trans %d %d\n",(int)(x_ratio*640), (int)(y_ratio*480) );
        }
    }
}

void MainWidget::setBT()
{
    bluetoothProcess->stopProcess();
    emit changeStack(0);
    bluetoothProcess->start();
    imageViewer->setFocus(2);
    record_BT = 0;
    setBluetooth(record_BT);

}

void MainWidget::setBluetooth( int x )
{
    imageViewer->setBT( x );
}


void MainWidget::setImage(unsigned char *data, int width, int height)
{
    imageViewer->setImage( data, width, height );

}

/**************************************************************************
slot function, use by this class, transfer the device to class ImageWidget
**************************************************************************/

void MainWidget::addDevices()
{
    QStringList list;
    list << "/dev/video0";
    list << "/dev/video1";
    list << "/dev/video2";
    list << "/dev/video3";
    list << "/dev/video5";
    list << "/dev/video6";
    list << "/dev/video7";
    list << "/dev/video8";
    list << "/dev/video9";
    list << "/dev/video10";
    deviceCombo->insertItems(0,list);

}

void MainWidget::setDevice()
{
    emit changeStack(0);
    imageViewer->setFocus(1);
    cameraProcess->stopProcess();
    capDevice = deviceCombo->currentText().toLatin1().data();
    printf("ImageWidget capDevice:%s\n",capDevice);
    cameraProcess->setDevice(capDevice);
    cameraProcess->start();
}

