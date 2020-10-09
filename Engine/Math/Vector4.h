#pragma once

class Vector4 {
public:
	Vector4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	Vector4(const Vector3 input, const float i_w = 0) {
		this->x = input.x;
		this->y = input.y;
		this->z = input.z;
		this->w = i_w;
	}

	Vector4(const Vector2 input, const float i_z = 0, const float i_w = 0) {
		this->x = input.x;
		this->y = input.y;
		this->z = i_z;
		this->w = i_w;
	}
	
	Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	// BASIC
	inline Vector4 operator = (const Vector4 other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}
	inline bool operator == (const Vector4 other) const {
		if (x == other.x && y == other.y && z == other.z && w == other.w)
			return true;
		else
			return false;
	}
	inline bool operator != (const Vector4 &other) const {
		return !(*this == other);
	}

	// MATH
	inline Vector4 getInverse() const
	{
		return Vector4{ -x, -y, -z, -w };
	}
	inline float getMagnitude() const
	{
		return sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}
	inline Vector4 getNormalized() const
	{
		const float mag = getMagnitude();
		return Vector4{x / mag, y / mag, z / mag, w / mag};
	}
	inline Vector4 operator - () const {
		return Vector4{-x, -y, -z, -w};
	}
	inline Vector4 operator += (const Vector4 other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	inline Vector4 operator -= (const Vector4 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}
	inline Vector4 operator *= (const Vector4 other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}
	inline Vector4 operator /= (const Vector4 other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	inline Vector4 operator *= (const float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}
	inline Vector4 operator /= (const float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}


	// Need a const implementation
	static const Vector4 Zero() {
		return { 0, 0, 0, 0 };
	}
	static const Vector4 One() {
		return { 1, 1, 1, 1 };
	}

	inline float Get(const int i) const
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return -9999;
		}
	}

	inline void Set(const int i, const float input)
	{
		switch (i)
		{
		case 0:
			x = input;
			return;
		case 1:
			y = input;
			return;
		case 2:
			z = input;
			return;
		case 3:
			w = input;
			return;
		default:
			return;
		}
	}


public:
	float x;
	float y;
	float z;
	float w;
};

inline Vector4 operator + (const Vector4 v1, const Vector4 v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}
inline Vector4 operator - (const Vector4 v1, const Vector4 v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}
inline Vector4 operator * (const Vector4 v1, const float scalar) {
	return Vector4{ v1.x * scalar, v1.y * scalar, v1.z * scalar, v1.w * scalar };
}
inline Vector4 operator / (const Vector4 v1, const float scalar) {
	return Vector4{ v1.x / scalar, v1.y / scalar, v1.z / scalar, v1.w / scalar };
}