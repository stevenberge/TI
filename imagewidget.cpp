#include <QtCore>
#include <QtDebug>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QtGui>
#include <QMessageBox>
#include "imagewidget.h"


ImageWidget::ImageWidget( QWidget *parent )
        : QWidget(parent)
{
       show_focus = 1;
       BTnode = 0;
}

ImageWidget::~ImageWidget()
{
//    delete dataProcess;
}

void ImageWidget::setFocus( int x )
{
    show_focus = x;
}

int ImageWidget::getFocus()
{
    return show_focus;
}

void ImageWidget::setBT( int x )
{
    BTnode = x;
    update();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if ( show_focus == 1 )      //show the information of camera
    {
        painter.drawImage(0, 0, qimage);
    }
    else if ( show_focus == 2 ) //show the information of bluetooth
    {
         painter.setBackground(QBrush(Qt::white));
         QPen pen(Qt::red);
         painter.setPen(pen);
         painter.setBrush(QBrush(Qt::red,Qt::SolidPattern)); //设置画刷形式
         if ( BTnode == 0 )  painter.drawEllipse(0,0,20,20);
         else if ( BTnode == 1 )  painter.drawEllipse( this->width()-20, 0, 20, 20 );
         else if ( BTnode == 2 )  painter.drawEllipse( 0, this->height()-20, 20, 20 );
         else if ( BTnode == 3 )  painter.drawEllipse( this->width()-20, this->height()-20, 20, 20 );
         else QMessageBox::about( this, tr("note"), tr("finish") );
    }
    else   //show the information of textedit
    {

    }

}

/*void ImageWidget::setImageDir()
{
    dataProcess->stopProcess();
    if (dataProcess->setImageDir())
        dataProcess->start(QThread::HighestPriority);
}

/**************************************************************
no used in this class, trigger by class Mainwidget's slot
setDevice, QString device from class Mainwidget, transfer the
device to class DataProcess's setDevice, after it, start
DataProcess's run.
***************************************************************/
/*void ImageWidget::setDevice(QString device)
{
    dataProcess->stopProcess();
    capDevice = device.toLatin1().data();
    printf("ImageWidget capDevice:%s\n",capDevice);
    dataProcess->setDevice(capDevice);
    dataProcess->start(QThread::HighestPriority);
}

/**************************************************************
slot fuction, set the image's data, used by this class,
signal from class DataProcess's sendData
***************************************************************/
void ImageWidget::setImage(unsigned char *data, int width, int height)
{
    qimage = QImage(data, width, height, QImage::Format_RGB888);
    update();
}

/**************************************************************
slot fuction, set the image's data, used by this class,
signal from class MainWidget's close button click signal
***************************************************************/
/*void ImageWidget::stopAll()
{
    dataProcess->stopProcess();
}

void ImageWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QRegExp rx("\\.(txt|dat)$", Qt::CaseInsensitive);
        foreach(QUrl url , event->mimeData()->urls()) {
            filename = url.toLocalFile();
            if(rx.indexIn(filename) >= 0) {
                    event->accept();
                    return;
            }
            else {
                    event->ignore();
            }
        }
    } else {
        event->ignore();
    }
}

void ImageWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QRegExp rx("\\.(txt|dat)$", Qt::CaseInsensitive);
        foreach(QUrl url , event->mimeData()->urls()) {
            filename = url.toLocalFile();
            if(rx.indexIn(filename) >= 0) {
                    event->accept();
                    //importData();
                    return;
            }
            else {
                    event->ignore();
            }
        }
    } else {
        event->ignore();
    }
}

void ImageWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
*/

