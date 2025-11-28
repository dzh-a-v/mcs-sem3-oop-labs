#include "Shape.h"
#include <iostream>
using namespace std;

int Shape::Count = 0;

int Shape::nextIndex = 0;

Shape* Shape::shapes[1000] = {};  // = {} initializes nullptr


void Shape::PrintCount()
{
    cout << "\n Now there are " << Count << " shapes" << endl;
}

int Shape::GetCount() {
    return Count;
}

Shape::Shape() {
    if (Count < 1000) {
        shapes[nextIndex] = this;
        nextIndex++;
    }
    Count++;
}

Shape::~Shape() {
    for (int i = 0; i < nextIndex; ++i) {
        if (shapes[i] == this) {
            for (int j = i; j < nextIndex - 1; ++j) {
                shapes[j] = shapes[j + 1];
            }
            shapes[--nextIndex] = nullptr;
            break;
        }
    }
    Count--;
}


void Shape::incCount() { Count++; }

void Shape::decCount() { Count--; }