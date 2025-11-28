#pragma once
#include "Shape.h"
#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class ShapeWidget : public QWidget {
    Q_OBJECT
    QVector<Shape*> shapes;    // all figures collection; can use std::vector<Shape*> instead in this case
    Shape* active = nullptr;   // active figure
    QPoint lastPos;            // last mouse position

public:
    ShapeWidget(QWidget* parent) : QWidget(parent) {} // where to create it

    void addShape(Shape* s) {
        shapes.push_back(s); // added new figure
        update(); // mark as 'needed for repainting'
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
        QPainter p(this); // create painter for this widget
        p.setRenderHint(QPainter::Antialiasing); // make it smoother

        for (auto s : shapes) {
            s->draw(p); // draw each shape

            // selection boundary
            if (s == active) {
                p.setBrush(Qt::NoBrush);
                p.setPen(QPen(Qt::black, 1, Qt::DashLine));
                QRect r = s->boundingRect();
                p.drawRect(r); // frame
            }
        }
    }

    void mousePressEvent(QMouseEvent* e) override {
        for (int i = shapes.size() - 1; i >= 0; --i) { // from end because there is top
            if (shapes[i]->contains(e->pos())) {
                active = shapes[i];
                shapes.remove(i);
                shapes.push_back(active);
                lastPos = e->pos();
                update();
                return; // later we'll reset 'active' otherwise
            }
        }
        active = nullptr; // if no figure clicked
        update();
    }

    void mouseMoveEvent(QMouseEvent* e) override {
        if (active) {
            QPoint delta = e->pos() - lastPos;
            active->move(delta.x(), delta.y());
            lastPos = e->pos();
            update();
        }
    }
};
