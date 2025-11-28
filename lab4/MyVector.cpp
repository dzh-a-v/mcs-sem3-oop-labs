#include "stdafx.h"
#include "MyVector.h"


Vector::Vector (double c1, double c2)
{
	//Shape::incCount();
	x=c1;   y=c2;
}

Vector::Vector ()
{
	//Shape::incCount();
	x = y = 0.;
}

Vector::Vector(const Vector& v) // Конструктор копирования
{
	//Shape::incCount();
	*this = v; // Using assignment operator
}

//Vector::~Vector() { Shape::decCount(); }

void Vector::Out()
{
	cout << "\nVector:  x = " << x << ",  y = " << y;
}

void Vector::Move(Vector& v) {
	*this = *this + v;
}

double Vector::Area() {
	return 0.0;
}

//====== Переопределение операций =====//
Vector& Vector::operator= (const Vector& v)	// Присвоение
{
	if (this == &v)
		return *this;
	x = v.x;
	y = v.y;
	return *this;
}

Vector Vector::operator+ (const Vector& v)	// Сложение
{
	Vector t;
	t.x = x + v.x;
	t.y = y + v.y;
	return t;
}

double Vector::operator! ()  // Взятие модуля
{
	return sqrt(x*x + y*y);
}

double Vector::operator! () const // Взятие модуля (const версия)
{
	return sqrt(x*x + y*y);
}

bool Vector::operator> (const Vector& v) // Сравнение
{
	return ( (!(*this)) > (!v) );
}

bool Vector::operator< (const Vector& v) // Сравнение
{
	return ( (!(*this)) < (!v) );
}

bool Vector::operator== (const Vector& v)
{
	return (fabs(x - v.x) < DBL_EPSILON) && (fabs(y - v.y) < DBL_EPSILON);
}

bool Vector::operator== (const Vector& v) const
{
	return (fabs(x - v.x) < DBL_EPSILON) && (fabs(y - v.y) < DBL_EPSILON);
}

Vector Vector::operator* (double d)
{
	return Vector(d * x, d * y);
}

double Vector::operator* (const Vector& v)
{
	return (x * v.x + y * v.y);
}

Vector operator* (double d, const Vector& v)
{
	return Vector(d * v.x, d * v.y);
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

double Vector::GetX() const { return x; }

double Vector::GetY() const { return y; }