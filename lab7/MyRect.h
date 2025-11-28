#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class MyRect : public QGraphicsRectItem {
public:
    MyRect(const QRectF& rect) : QGraphicsRectItem(rect) {
        setFlags(QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsFocusable);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        // поднять объект на передний план
        setZValue(1);
        QGraphicsRectItem::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        setZValue(0); // вернуть обратно (или оставить 1, если хотите всегда сверху)
        QGraphicsRectItem::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (isSelected()) {
            QGraphicsRectItem::mouseMoveEvent(event);
        }
        else {
            event->ignore();
        }
    }
};
