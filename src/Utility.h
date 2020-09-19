#pragma once

template <class T>
struct Vector2 {
    Vector2();
    Vector2(T X, T Y);
    template <class U>
    explicit Vector2(const Vector2<U> &vector);
    T x;
    T y;
};

#include "Utility.inl"

typedef Vector2<int> Vector2i;