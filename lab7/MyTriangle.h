#pragma once
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

class MyTriangle : public QGraphicsPolygonItem {
public:
    MyTriangle(const QPolygonF& poly) : QGraphicsPolygonItem(poly) {
        setFlags(QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsFocusable);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        setZValue(1);
        QGraphicsPolygonItem::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        setZValue(0);
        QGraphicsPolygonItem::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (isSelected()) {
            QGraphicsPolygonItem::mouseMoveEvent(event);
        }
        else {
            event->ignore();
        }
    }
};
