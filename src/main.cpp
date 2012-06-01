#include <QtGui/QApplication>
#include "mainwindow.h"
#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow w;
    w.setCentralWidget(new GlWidget(&w));
    w.setWindowTitle("Snakes on a Sphere");
    w.resize(1024, 768);
    w.show();

    return app.exec();
}
