#include "myglwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
    w.setWindowTitle("cg_final_proj");
    w.show();
    w.setBaseSize(1280, 720);
    return a.exec();
}
