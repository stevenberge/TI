#include <qapplication.h>
#include <QTextCodec>
#include "mainwidget.h"
#include <textedit.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    MainWidget *mainWidget;
    mainWidget = new MainWidget;
    mainWidget->resize(640,480);
    mainWidget->show();


    return app.exec();
}


