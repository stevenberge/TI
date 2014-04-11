
#include "mycanvas.h"
#include <QPainter>

//set style of the canvas
void init(QWidget* obj){
    obj->setStyleSheet("border:1px solid black");
    obj->setAutoFillBackground(true);
}

MyCanvas::MyCanvas(int width,int height,QWidget *parent)
  :QWidget(parent)
{
  brushWidth=8;
  //set a canvas with width and height
  //attention:Format_argb32 is transparent mode
  image = QImage(width,height,QImage::Format_ARGB32);
  init(this);
 // backColor = qRgb(20,20,20);
 // image.fill(backColor);
  //connect(this,SIGNAL(mouseMoveSig(QMouseEvent*)),this,SLOT(slot1(QMouseEvent*)));
  //connect(this,SIGNAL(mousePressSig(QMouseEvent*)),this,SLOT(slot2(QMouseEvent*)));
  //connect(this,SIGNAL(mouseReleaseSig(QMouseEvent*)),this,SLOT(slot3(QMouseEvent*)));
}

MyCanvas::MyCanvas(int width,int height,int brushWidth,QWidget *parent)
  :QWidget(parent)
{
  image = QImage(width,height,QImage::Format_ARGB32);
  this->brushWidth=brushWidth;
  init(this);
}
void MyCanvas::paintEvent(QPaintEvent* e){
  QPainter* paint=new QPainter(this);
  paint->drawImage(0,0,image);
}
void MyCanvas::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
  {
    lastPoint = event->pos();
    emit mousePressSig(event);
  }
}
//handle with mouse move
void MyCanvas::mouseMoveEvent(QMouseEvent *event)
{
  if(event->buttons()&Qt::LeftButton)
  {
    endPoint = event->pos();
    qDebug()<<"MyCanvas.mouseMoveEvent";
    paint(image);
    lastPoint=endPoint;
    emit mouseMoveSig(event);
  }
}
void MyCanvas::mouseReleaseEvent(QMouseEvent *event)
{
//  if(event->button() == Qt::LeftButton)
//  {
//    endPoint = event->pos();
//    paint(image);
//    lastPoint=endPoint;
//    emit MyCanvas::mouseReleaseSig(event);
//  }
}
void MyCanvas::paintLine(QPoint l,QPoint r, QColor color){
  QPainter pp(&image);
  pp.setPen(QPen(color,brushWidth,Qt::SolidLine));
  qDebug()<<"MyCanvas.paintLine:"<<l.x()<<","<<l.y()<<" - "<<r.x()<<","<<r.y();
  pp.drawLine(l,r);
  update();
}
void MyCanvas::paintLine(QLine &line,QColor color){
  QPoint l=line.p1(),r=line.p2();
  MyCanvas::paintLine(l,r,color);
}
void MyCanvas::paint(QImage &theImage)
{
  paintLine(lastPoint,endPoint,Qt::yellow);
//  QPainter pp(&theImage);
//  pp.setPen(QPen(Qt::yellow,8,Qt::SolidLine));
//  pp.drawLine(lastPoint,endPoint);
}
