#pragma once

#include "Vector.h"

class Vector3 {
public:
	Vector3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector3(const Vector2 input)
	{
		this->x = input.x;
		this->y = input.y;
		this->z = 0;
	}
	
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// BASIC
	inline Vector3 operator = (const Vector3 other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	inline bool operator == (const Vector3 other) const {
		if (x == other.x && y == other.y && z == other.z)
			return true;
		else
			return false;
	}
	inline bool operator != (const Vector3 &other) const {
		return !(*this == other);
	}

	// MATH
	inline Vector3 getInverse() const
	{
		return Vector3{ -x, -y, -z };
	}
	inline float getMagnitude() const
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}
	inline Vector3 getNormalized() const
	{
		const float mag = getMagnitude();
		return Vector3{x / mag, y / mag, z / mag};
	}
	inline Vector3 operator - () const {
		return Vector3{-x, -y, -z};
	}
	inline Vector3 operator += (const Vector3 other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	inline Vector3 operator -= (const Vector3 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	inline Vector3 operator *= (const Vector3 other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	inline Vector3 operator /= (const Vector3 other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	inline Vector3 operator *= (const float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	inline Vector3 operator /= (const float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}


	// Need a const implementation
	static const Vector3 Zero() {
		return { 0, 0, 0 };
	}
	static const Vector3 One() {
		return { 1, 1, 1 };
	}


public:
	float x;
	float y;
	float z;
};

inline Vector3 operator + (const Vector3 v1, const Vector3 v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
inline Vector3 operator - (const Vector3 v1, const Vector3 v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
inline Vector3 operator * (const Vector3 v1, const float scalar) {
	return Vector3{ v1.x * scalar, v1.y * scalar, v1.z * scalar };
}
inline Vector3 operator / (const Vector3 v1, const float scalar) {
	return Vector3{ v1.x / scalar, v1.y / scalar, v1.z / scalar };
}

inline float dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vector3 cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}