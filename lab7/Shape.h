#pragma once
#include <QPainter>
#include <QRect>

class Shape {
protected:
    QRect rect;
    QColor color;
public:
    Shape(const QRect& r, const QColor& c) : rect(r), color(c) {}
    virtual ~Shape() = default;

    virtual void draw(QPainter* p) = 0;
    virtual bool contains(const QPoint& pt) { return rect.contains(pt); }
    void move(int dx, int dy) { rect.translate(dx, dy); }
    QRect boundingRect() const { return rect; }
};

// Rectangle
class Rectangle : public Shape {
public:
    using Shape::Shape;
    void draw(QPainter* p) override {
        p->setBrush(color);
        p->drawRect(rect);
    }
};

// Ellipse
class Ellipse : public Shape {
public:
    using Shape::Shape;
    void draw(QPainter* p) override {
        p->setBrush(color);
        p->drawEllipse(rect);
    }
};

// Triangle
class Triangle : public Shape {
public:
    using Shape::Shape;
    void draw(QPainter* p) override {
        p->setBrush(color);
        QPolygon poly;
        poly << QPoint(rect.center().x(), rect.top())
            << QPoint(rect.left(), rect.bottom())
            << QPoint(rect.right(), rect.bottom());
        p->drawPolygon(poly);
    }
};
