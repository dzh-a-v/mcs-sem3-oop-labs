#pragma once
#include <QMainWindow>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void addRectangle();
    void addEllipse();
    void addTriangle();
    void deleteSelected();

private:
    QGraphicsScene* scene;
};
