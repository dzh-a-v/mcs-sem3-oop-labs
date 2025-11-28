#pragma once

class Vector;

class Shape
{
private:
	static int Count;
	static int nextIndex;

public:
	static Shape* shapes[1000];

	Shape();
	virtual ~Shape();

	static void PrintCount();
	static int GetCount();
	static void incCount();
	static void decCount();

	virtual void Move(Vector& v) = 0;
	virtual void Out() = 0;
	virtual double Area() = 0;
};