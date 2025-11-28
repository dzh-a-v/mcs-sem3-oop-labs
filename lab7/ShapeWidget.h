#pragma once
#include "Shape.h"
#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class ShapeWidget : public QWidget {
    Q_OBJECT

        QVector<Shape*> shapes;   // коллекция фигур
    Shape* active = nullptr;  // активная фигура
    QPoint lastPos;           // последняя позиция мыши

public:
    ShapeWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    void addShape(Shape* s) {
        shapes.push_back(s);
        update();
    }

    void removeActiveShape() {
        if (active) {
            shapes.removeOne(active);
            delete active;
            active = nullptr;
            update();
        }
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        for (auto s : shapes) s->draw(&p);
    }

    void mousePressEvent(QMouseEvent* e) override {
        for (int i = shapes.size() - 1; i >= 0; --i) {
            if (shapes[i]->contains(e->pos())) {
                active = shapes[i];
                shapes.remove(i);
                shapes.push_back(active); // поднять на передний план
                lastPos = e->pos();
                update();
                return;
            }
        }
        active = nullptr;
    }

    void mouseMoveEvent(QMouseEvent* e) override {
        if (active) {
            QPoint delta = e->pos() - lastPos;
            active->move(delta.x(), delta.y());
            lastPos = e->pos();
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent*) override {
        active = nullptr;
    }
};
