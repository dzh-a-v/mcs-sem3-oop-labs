//#include <QApplication>
//#include "ShapeWidget.h"
//
//int main(int argc, char* argv[]) {
//    QApplication a(argc, argv);
//    ShapeWidget w;
//    w.resize(600, 400);
//    w.show();
//
//    w.addShape(new Rectangle(QRect(50, 50, 100, 60), Qt::green));
//    w.addShape(new Ellipse(QRect(200, 100, 80, 80), Qt::yellow));
//    w.addShape(new Triangle(QRect(350, 50, 100, 100), Qt::red));
//
//    return a.exec();
//}

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
