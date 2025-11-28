#include "Point.h"
#include <iostream>

Point::Point() : x_(0.0), y_(0.0) {}

Point::Point(double x, double y) : x_(x), y_(y) {}

double Point::getX() const {
    return x_;
}

double Point::getY() const {
    return y_;
}

void Point::setX(double x) {
    x_ = x;
}

void Point::setY(double y) {
    y_ = y;
}

void Point::print() const {
    std::cout << "Point(" << x_ << ", " << y_ << ")" << std::endl;
}

bool Point::operator ==(const Point& other) const {
    if (x_ == other.x_ && y_ == other.y_)
        return true;
    else
        return false;
}

Point& Point::operator =(const Point& other) {
    x_ = other.x_;
    y_ = other.y_;
    return *this;
}

Point& Point::operator +=(const float& n) {
    x_ += n;
    y_ += n;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "Point(" << point.x_ << ", " << point.y_ << ")";
    return os;
}

bool Point::operator<(const Point& other) const {
    if (x_ == other.x_)
        return y_ < other.y_;
    return x_ < other.x_;
}