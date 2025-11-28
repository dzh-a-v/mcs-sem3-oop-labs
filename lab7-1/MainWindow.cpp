#include "MainWindow.h"
#include <QToolBar> // for creating toolbar
#include <QAction> // for toolbar actions (buttons)

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) // call parent QMainWindow constructor with parent widget
{
    setFixedSize(800, 600); // user cannot resize

    widget = new ShapeWidget(this); // create the widget canvas where shapes will be drawn
    
    setCentralWidget(widget); // it becomes main here and occupies the whole area

    auto* toolbar = new QToolBar("Shapes", this);
    addToolBar(Qt::TopToolBarArea, toolbar); // put it on the top

    // Add buttons to the toolbar
    QAction* rectAct = toolbar->addAction("Rectangle"); // button to add a rectangle
    QAction* ellipseAct = toolbar->addAction("Ellipse"); // button to add an ellipse
    QAction* triAct = toolbar->addAction("Triangle"); // button to add a triangle
    QAction* delAct = toolbar->addAction("Delete"); // button to delete the active shape

    // Connect the "Rectangle" button click to a lambda function
    // This lambda creates a new Rectangle and adds it to the ShapeWidget
    connect(rectAct, &QAction::triggered, this, [this]() {
        widget->addShape(new Rectangle(QRect(50, 50, 100, 60), Qt::yellow));
        });

    //same
    connect(ellipseAct, &QAction::triggered, this, [this]() {
        widget->addShape(new Ellipse(QRect(200, 50, 80, 80), Qt::green));
        });

    //sam
    connect(triAct, &QAction::triggered, this, [this]() {
        widget->addShape(new Triangle(QRect(350, 50, 100, 100), Qt::cyan));
        });

    //sa
    connect(delAct, &QAction::triggered, this, [this]() {
        widget->removeActiveShape();
        });
    //s
    //
    ///
    //
}
