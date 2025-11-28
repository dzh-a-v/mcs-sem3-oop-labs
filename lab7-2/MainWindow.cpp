#include "MainWindow.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QToolBar>
#include <QAction>
#include <algorithm>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    auto* view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setDragMode(QGraphicsView::NoDrag);

    setCentralWidget(view);

    auto* toolbar = addToolBar("Shapes");
    QAction* rectAct = toolbar->addAction("Rectangle");
    QAction* ellipseAct = toolbar->addAction("Ellipse");
    QAction* triAct = toolbar->addAction("Triangle");
    QAction* delAct = toolbar->addAction("Delete");

    connect(rectAct, &QAction::triggered, this, &MainWindow::addRectangle);
    connect(ellipseAct, &QAction::triggered, this, &MainWindow::addEllipse);
    connect(triAct, &QAction::triggered, this, &MainWindow::addTriangle);
    connect(delAct, &QAction::triggered, this, &MainWindow::deleteSelected);

    // Поднимаем выделенную фигуру наверх
    connect(scene, &QGraphicsScene::selectionChanged, [this]() {
        for (auto item : scene->selectedItems()) {
            // ZValue = текущий максимум + 1
            qreal maxZ = 0;
            for (auto it : scene->items()) {
                if (it != item)
                    maxZ = std::max(maxZ, it->zValue());
            }
            item->setZValue(maxZ + 1);
        }
        });
}

void MainWindow::addRectangle()
{
    auto* rect = scene->addRect(50, 50, 100, 60, QPen(Qt::black), QBrush(Qt::yellow));
    rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    rect->setZValue(0);
}

void MainWindow::addEllipse()
{
    auto* ellipse = scene->addEllipse(200, 50, 80, 80, QPen(Qt::black), QBrush(Qt::green));
    ellipse->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    ellipse->setZValue(0);
}

void MainWindow::addTriangle()
{
    QPolygonF triangle;
    triangle << QPointF(0, 0) << QPointF(50, 100) << QPointF(-50, 100);
    auto* poly = scene->addPolygon(triangle, QPen(Qt::black), QBrush(Qt::cyan));
    poly->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    poly->setPos(350, 50);
    poly->setZValue(0);
}

void MainWindow::deleteSelected()
{
    auto selected = scene->selectedItems();
    for (auto item : selected) {
        scene->removeItem(item);
        delete item;
    }
}
