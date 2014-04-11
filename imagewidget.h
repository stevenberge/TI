#ifndef IMAGEWIDGET_H_
#define IMAGEWIDGET_H_

#include <QtGui>

//class DataProcess;
class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget( QWidget *parent = 0 );
    ~ImageWidget();
    void setFocus( int x );
    int getFocus();
    void setImage(unsigned char *data, int width, int height);
    void setBT( int x );

/*    void dataInit();
    void setDevice(QString device);*/
protected:
/*    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void mousePressEvent(QMouseEvent *e);*/
    void paintEvent(QPaintEvent *event);
  /*
signals:
    void sendShowSetting();
private slots:
    void setImageDir();
    void setImage(const QImage &);
    void setImage(unsigned char *data, int width, int height);
    void stopAll();
*/
private:
/*    QString filename;
    QImage qimage;
    bool bFit;
    char *capDevice;
    int grabx[2];
    int graby[2];
    int leftnum;
    DataProcess *dataProcess;
*/
    int show_focus;
    QImage qimage;
    int BTnode;
};

#endif /*IMAGEWIDGET_H_*/
