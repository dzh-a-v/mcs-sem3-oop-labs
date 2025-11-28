#pragma once
#include <iostream>

class Point {
private:
    double x_;
    double y_;

public:
    Point();
    Point(double x, double y);
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    void print() const;

    bool operator ==(const Point& other) const;
    Point& operator =(const Point& other);
	Point& operator +=(const float& n);
	friend std::ostream& operator<<(std::ostream& os, const Point& point);
	bool operator<(const Point& other) const;
};