#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QMainWindow>
#include <QtDebug>
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
QT_FORWARD_DECLARE_CLASS(QMouseEvent)
class MyCanvas : public QWidget
{
 Q_OBJECT //// this is too kengdie

public:
 MyCanvas(int width,int height,QWidget *parent=0);
 MyCanvas(int width,int height,int brushWidth,QWidget *parent=0);
 void paintLine(QPoint l,QPoint r,QColor color);
 void paintLine(QLine&,QColor color);
 void reload(){
   image.fill(0x00000000);
   update();
 }

signals:
 void mouseMoveSig(QMouseEvent *e);
 void mousePressSig(QMouseEvent *e);
 void mouseReleaseSig(QMouseEvent *e);
private slots:
//  void paintSlot(QPoint l,QPoint r){
//    paintLine(l,r);
//  }
 void slot1(QMouseEvent *e){
   qDebug() << "slot"<<" ....";
 }
 void slot2(QMouseEvent *e){
   qDebug() << "slot"<<" ....";
 }
 void slot3(QMouseEvent *e){
   qDebug() << "slot"<<" ....";
 }
 void mouseMoveSlot(QMouseEvent *e){
   qDebug() << "MyCanvas.mouseMoveSlot"<<"";
   mouseMoveEvent(e);
 }

protected:
 void paintEvent(QPaintEvent *);   //ػ¼
 void mousePressEvent(QMouseEvent *);  //갴¼
 void mouseMoveEvent(QMouseEvent *);   //ƶ¼
 void mouseReleaseEvent(QMouseEvent *);   //ͷ¼
 void paint(QImage& theImage);   //л
private:
 QImage image;    //QImageϻͼ
 int brushWidth;
 QRgb backColor;  //QRgbɫ󣬴洢imageıɫ
 QPoint lastPoint,endPoint; //ָǰ
};

#endif
