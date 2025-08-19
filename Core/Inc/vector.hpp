#pragma once

template<class T>
constexpr T abs(T x) { return x < 0 ? -x : x; }

template<class T>
class Vector2
{
public:
	T x;
	T y;

	constexpr inline Vector2<T> operator+() const { return { x, y }; }
	constexpr inline Vector2<T> operator-() const { return { (T)-x, (T)-y }; }

	constexpr Vector2<T> operator+(const Vector2<T> add) const { return { (T)(x + add.x), (T)(y + add.y) }; }
	constexpr Vector2<T> operator-(const Vector2<T> sub) const { return { (T)(x - sub.x), (T)(y - sub.y) }; }
	constexpr T operator*(const Vector2<T> dotProduct) const { return (T)(x * dotProduct.x) + (T)(y * dotProduct.y); }

	constexpr Vector2<T>& operator+=(const Vector2<T> add) { x += add.x; y += add.y; return *this; }
	constexpr Vector2<T>& operator-=(const Vector2<T> sub) { x -= sub.x; y -= sub.y; return *this; }

	constexpr bool operator==(const Vector2<T> equal) const { return (x == equal.x) && (y == equal.y); }

	constexpr Vector2<T> operator*(const T k) const { return { (T)(x * k), (T)(y * k) }; }
	constexpr Vector2<T> operator/(const T k) const { return { (T)(x / k), (T)(y / k) }; }

	constexpr void swap(Vector2<T>& swap) { auto t = swap; swap = *this; *this = t; } 

	template<class Y>
	constexpr operator Vector2<Y>() { return { (Y)x, (Y)y }; }
};

using Vector2uc = Vector2<unsigned char>;
using Vector2f = Vector2<float>;
