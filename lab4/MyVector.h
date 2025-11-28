#pragma once
#include "Shape.h"
#include <iostream>

class Vector : public Shape
{
private:
	double x, y;	// Координаты вектора на плоскости
	//static int Count;
public:
	//========== Три конструктора
	Vector (double c1, double c2);
	Vector ();							// Default
	Vector(const Vector& v);			// Конструктор копирования
	//~Vector();
	
	//====== Переопределение операций =====//
	Vector& operator= (const Vector& v);	// Присвоение
	Vector operator+ (const Vector& v);
	double operator! ();
	double operator! () const;
	bool operator> (const Vector& v);
	bool operator< (const Vector& v);
	bool operator== (const Vector& v);
	bool operator== (const Vector& v) const;
	Vector operator* (double d);
	double operator* (const Vector& v); 
	friend Vector operator* (double d, const Vector& v);
	friend std::ostream& operator<<(std::ostream& os, const Vector& v);

	void Out() override;
	void Move(Vector& v) override;
	double Area() override;

	double GetX() const;
	double GetY() const;
	//static void PrintCount();
};

Vector operator* (double d, const Vector& v);
std::ostream& operator<<(std::ostream& os, const Vector& v);