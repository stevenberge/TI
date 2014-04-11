#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>

class ImageWidget;
class CameraProcess;
class BluetoothProcess;
class TextEdit;
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget();
    void addDevices();
    void addFiles();
    void setBluetooth( int x );   // calibrate BT
    TextEdit *textEdit;
    //void calibrateCam();  //process of calibrating camera
    //void calibrateBT();   //process of calibrating bluetooth
//    void setFile();     // choose file
//    void showSetting(); // show setting
private:
    char *capDevice;
    ImageWidget *imageViewer;
    QComboBox *deviceCombo;  //the list of cameras
    QPushButton *calibCamBtn; //the button of calibration camera
    QPushButton *calibBTBtn;  //the button of calibration bluetooth
    QPushButton *startBtn;   //the button of start reading
    QPushButton *openBtn;   //the button of start reading
    CameraProcess *cameraProcess;

    BluetoothProcess *bluetoothProcess;
    struct nn{
        int x,y;
    }node[4];
    int record_BT;


//蓝牙的四个边界点
    int left_point, right_point, up_point, down_point;


private slots:
    void setDevice();   // choose camera
    void setImage(unsigned char *data, int width, int height);
    void setBT();
    void processBT( int x, int y );
    void setOpen();
signals:
    void changeStack( int x );
    void fileOpen();

};

#endif // MAINWIDGET_H
