#include "widget.h"
#include<QResource>

#include <QApplication>
#include"config.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
