#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication App(argc, argv);
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    MainWindow window;
    window.show();

    return App.exec();
}
