#pragma once
#include "Shape.h"
#include "MyVector.h"
#include <iostream>


class Rect : public Shape {
private:
	Vector ptLT; // Left Top point
	Vector ptRB; // Right Bottom point
public:
	Rect();
	Rect(double left, double top, double right, double bottom);
	Rect(const Rect& other);
	Rect(Vector& v1, Vector& v2);

	void Inflate(double d = 1);
	void Inflate(double dHorz, double dVert);
	void Inflate(double dLeft, double dTop, double dRight, double dBottom);
	void Out() override;
	void Move(Vector& v) override;
	double Area() override;
	Vector getCenter() const;

	Rect& operator= (const Rect& r);
	friend std::ostream& operator<< (std::ostream& out, const Rect& r);
};