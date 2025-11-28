#pragma once
#include <QMainWindow>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void addRectangle();
    void addEllipse();
    void addTriangle();

private:
    QGraphicsScene* scene;
};
