#include "Rect.h"
#include <iostream>
using namespace std;

Rect::Rect(double left, double top, double right, double bottom)
	: ptLT(left, top), ptRB(right, bottom) {}

Rect::Rect() : ptLT(0, 0), ptRB(0, 0) {}

Rect::Rect(const Rect& other)
{
	*this = other;
}

Rect::Rect(Vector& v1, Vector& v2) : ptLT(v1), ptRB(v2) { }

void Rect::Inflate(double d)
{
	ptLT = ptLT + Vector(-d, d);
	ptRB = ptRB + Vector(d, -d);
}

void Rect::Inflate(double dHorz, double dVert)
{
	ptLT = ptLT + Vector(-dHorz, dVert);
	ptRB = ptRB + Vector(dHorz, -dVert);
}

void Rect::Inflate(double dLeft, double dTop, double dRight, double dBottom)
{
	ptLT = ptLT + Vector(-dLeft, dTop);
	ptRB = ptRB + Vector(dRight, -dBottom);
}

void Rect::Out()
{
	cout << "\nRect (" << ptLT.GetX() << ","
		<< ptLT.GetY() << ","
		<< ptRB.GetX() << ","
		<< ptRB.GetY() << ")\n";
}

void Rect::Move(Vector& v)
{
	ptLT = ptLT + v;
	ptRB = ptRB + v;
}

double Rect::Area() {
	return (ptLT.GetY() + ptRB.GetY()) * (ptLT.GetX() + ptRB.GetX());
}

Vector Rect::getCenter() const
{
	return Vector((ptLT.GetX() + ptRB.GetX()) / 2, (ptLT.GetY() + ptRB.GetY()) / 2);
}

Rect& Rect::operator= (const Rect& r)
{
	if (this == &r)
		return *this;
	ptLT = r.ptLT;
	ptRB = r.ptRB;
	return *this;
}

std::ostream& operator<< (std::ostream& out, const Rect& r)
{
	out << "Rect(" << r.ptLT << ", " << r.ptRB << ")";
	return out;
}