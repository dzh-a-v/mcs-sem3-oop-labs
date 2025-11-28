#pragma once
#include <iostream>

class MyString
{
private:
	char* m_pStr;		// Элемент данных класса (адрес строки)
public:
    MyString ();
	MyString (const char* s);	// Объявление конструктора
	MyString(const MyString& other); // Конструктор копирования
    ~MyString();		// Объявление деструктора

	MyString& operator= (const MyString& str); // Операция присвоения
	friend std::ostream& operator<<(std::ostream& os, const MyString& str);
	bool operator<(const MyString& other) const;

	void Copy (const char* s);
	char* GetString();	// Объявление метода (accessor)
	int GetLength();	// Объявление метода (длина строки)
};
