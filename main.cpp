#include <QtGui/QApplication>
#include "mainwindow.h"
void setCode()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("System"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));
}
int main(int argc, char *argv[])
{
    setCode();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
