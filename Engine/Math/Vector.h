#pragma once

class Vector2 {
public:
	Vector2() {
		this->x = x;
		this->y = y;
	}
	
	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	// BASIC
	inline Vector2 operator = (const Vector2 other) {
		x = other.x;
		y = other.y;
		return *this;
	}
	inline bool operator == (const Vector2 other) {
		if (x == other.x && y == other.y)
			return true;
		else
			return false;
	}
	inline bool operator != (const Vector2 &other) {
		return !(*this == other);
	}

	// MATH
	inline Vector2 operator - () {
		return Vector2{-x, -y};
	}
	inline Vector2 operator += (const Vector2 other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	inline Vector2 operator -= (const Vector2 other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	inline Vector2 operator *= (const Vector2 other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}
	inline Vector2 operator /= (const Vector2 other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	inline Vector2 operator *= (const float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	inline Vector2 operator /= (const float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	static float DistanceBetween(const Vector2 &v1, const Vector2 &v2)
	{
		const float dX = v2.x - v1.x;
		const float dY = v2.y - v1.y;
		return sqrt((dX * dX) + (dY * dY));
	}

	float DistanceFrom(const Vector2 &v) const
	{
		const float dX = v.x - x;
		const float dY = v.y - y;
		return sqrt((dX * dX) + (dY * dY));
	}


	// Need a const implementation
	static const Vector2 Zero() {
		return { 0, 0 };
	}
	static const Vector2 One() {
		return { 1, 1 };
	}
	static const Vector2 Up() {
		return { 0, 1 };
	}
	static const Vector2 Down() {
		return { 0, -1 };
	}
	static const Vector2 Left() {
		return { -1, 0 };
	}
	static const Vector2 Right() {
		return { 1, 0 };
	}


public:
	float x;
	float y;
};

inline Vector2 operator + (const Vector2 v1, const Vector2 v2) {
	return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2 operator - (const Vector2 v1, const Vector2 v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}
inline Vector2 operator * (const Vector2 v1, const float scalar) {
	return Vector2{ v1.x * scalar, v1.y * scalar };
}
inline Vector2 operator / (const Vector2 v1, const float scalar) {
	return Vector2{ v1.x / scalar, v1.y / scalar };
}