#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab8cw.h"

class lab8cw : public QMainWindow
{
    Q_OBJECT

public:
    lab8cw(QWidget *parent = nullptr);
    ~lab8cw();

private:
    Ui::lab8cwClass ui;
};

