#include "mainwindow.h"
#include <QApplication>

//use Nvidia GPU render
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
