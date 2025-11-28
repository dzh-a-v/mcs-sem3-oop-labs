#pragma once
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

class MyEllipse : public QGraphicsEllipseItem {
public:
    MyEllipse(const QRectF& rect) : QGraphicsEllipseItem(rect) {
        setFlags(QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsFocusable);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        setZValue(1);
        QGraphicsEllipseItem::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        setZValue(0);
        QGraphicsEllipseItem::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (isSelected()) {
            QGraphicsEllipseItem::mouseMoveEvent(event);
        }
        else {
            event->ignore();
        }
    }
};
