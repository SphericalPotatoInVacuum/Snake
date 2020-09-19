#include "Utility.h"
#include <iostream>

template <class T>
inline Vector2<T>::Vector2() : x(0), y(0) {}

template <class T>
inline Vector2<T>::Vector2(T X, T Y) : x(X), y(Y) {}

template <typename T>
template <typename U>
inline Vector2<T>::Vector2(const Vector2<U> &v)
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

template <typename T>
inline Vector2<T> operator-(const Vector2<T> &v) {
    return Vector2<T>(-v.x, -v.y);
}

template <typename T>
inline bool operator==(const Vector2<T> &left, const Vector2<T> &right) {
    return left.x == right.x && left.y == right.y;
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T> &left, const Vector2<T> &right) {
    return Vector2<T>(left.x - right.x, left.y - right.y);
}
