#pragma once
#include <QMainWindow>
#include "ShapeWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
        ShapeWidget* widget;

public:
    MainWindow(QWidget* parent = nullptr);
};