#include "stdafx.h"
#include "MyString.h"


void MyString::Copy (const char* s)
{
	delete [] m_pStr;
	// Динамически выделяем требуемое количество памяти.
	int len = strlen(s) + 1;
	m_pStr = new char[len];
	// + 1, так как нулевой байт тоже нужно скопировать
	// Если память выделена, копируем строку-аргумент в строку-член класса
	if (m_pStr)
		strcpy_s (m_pStr, len, s);
}

MyString::MyString() // Default-конструктор
{
	m_pStr = 0;
	Copy ("");
}

// Определение конструктора.
MyString::MyString (const char* s)
{
	m_pStr = 0;
	Copy(s);
}

MyString::MyString(const MyString& other) // Конструктор копирования
{
	m_pStr = 0;
	Copy(other.m_pStr);
}

// Определение деструктора.
MyString::~MyString()
{
	// Освобождение памяти, занятой в конструкторе для строки-члена класса
	delete[] m_pStr;
}

MyString& MyString::operator= (const MyString& str) // Операция присвоения
{
	if (this == &str)
		return *this;
	Copy(str.m_pStr);
	return *this;
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
	if (str.m_pStr)
		os << str.m_pStr;
	return os;
}

bool MyString::operator< (const MyString& other) const {
	return strcmp(m_pStr, other.m_pStr) < 0;
}

// Метод класса
char* MyString::GetString()
{
	return m_pStr;
}

int MyString::GetLength()
{
	return strlen(m_pStr) + 1;
}
