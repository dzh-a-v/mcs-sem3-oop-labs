#pragma once

#include <string>
#include <iostream>
#include <type_traits>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include "Point.h"
#include "MyVector.h"
#include "Rect.h"


template <class T>
void pr(const T& v, std::string s)
{
    std::cout << "\n\n\t" << s << "  # Sequence:\n";

    int i = 0;
    for (const auto& x : v)
        std::cout << "\n" << ++i << ". " << x;
    std::cout << "\n";
}

template <typename T1, typename T2, typename... Args>
void pr(const std::map<T1, T2, Args...>& m, std::string s)
{
	std::cout << "\n\n\t" << s << "  # Sequence:\n";
	int i = 0;
	for (const auto& p : m)
		std::cout << "\n" << ++i << ". " << p.first << " -> " << p.second;
	std::cout << "\n";
}

template <typename T1, typename T2, typename... Args>
void pr(const std::multimap<T1, T2, Args...>& m, std::string s)
{
	std::cout << "\n\n\t" << s << "  # Sequence:\n";
	int i = 0;
	for (const auto& p : m)
		std::cout << "\n" << ++i << ". " << p.first << " -> " << p.second;
	std::cout << "\n";
}


template<typename T>
struct is_stack : std::false_type {}; // for general case
template<typename T>
struct is_stack<std::stack<T>> : std::true_type {}; // specialization for std::stack 
													// (inherited from std::true_type)
template<typename T>
struct is_queue : std::false_type {};
template<typename T, typename Container>
struct is_queue<std::queue<T, Container>> : std::true_type {};

template<typename T>
struct is_priority_queue : std::false_type {};
template<typename T, typename Container, typename Compare>
struct is_priority_queue<std::priority_queue<T, Container, Compare>> : std::true_type {};


template<typename Container>
void printContainer(Container c) {
	if constexpr (is_stack<decltype(c)>::value) {
		while (!c.empty()) {
			std:: cout << c.top() << " ";
			c.pop();
		}
	}
	else if constexpr (is_queue<decltype(c)>::value) {
		while (!c.empty()) {
			std::cout << c.front() << " ";
			c.pop();
		}
	}
	else if constexpr (is_priority_queue<decltype(c)>::value) {
		while (!c.empty()) {
			std::cout << c.top() << " ";
			c.pop();
		}
	}
	else {
		std::cout << "Unsupported container type." << std::endl;
		return;
	}
	std::cout << std::endl;
}

struct CompareString {
	bool operator()(const char* a, const char* b) const {
		return std::strcmp(a, b) > 0; // lexicographically biggest on top
	}
};

template <typename T>
void printElement(const T& elem) {
	std::cout << elem << " ";
}

template <typename T>
void addToElement(T& elem, float value) {
	elem += value;
}

bool Pred1_1(const Point& elem) {
	float n = 4.0;
	float m = 7.9;
	return (elem.getX() >= -n && elem.getX() <= m && elem.getY() >= -n && elem.getY() <= m);
}

struct CompareRectCenters {
	bool operator()(const Rect& r1, const Rect& r2) const {
		Vector center1 = r1.getCenter();
		Vector center2 = r2.getCenter();
		return center1 < center2; // Rect with the farthest center from origin has higher priority
	}
};

void lowerCase(const std::string& input, std::string& output) {
	output.resize(input.size());
	std::transform(input.begin(), input.end(), 
		output.begin(),
		[](unsigned char c) { return std::tolower(c); });
}