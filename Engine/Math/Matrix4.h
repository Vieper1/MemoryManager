#pragma once

#include "Vector4.h"

class Matrix4 {
public:
	Matrix4() : mat{0}
	{}

	Matrix4(const float i_mat[4][4])
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				mat[i][j] = i_mat[i][j];
	}


	
	// OPERATIONS - WITH SCALAR
	Matrix4 operator + (const float i_scalar)
	{
		Matrix4 newMat;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				newMat.mat[i][j] = mat[i][j] + i_scalar;
		return newMat;
	}

	Matrix4 operator - (const float i_scalar)
	{
		Matrix4 newMat;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				newMat.mat[i][j] = mat[i][j] - i_scalar;
		return newMat;
	}
	
	Matrix4 operator * (const float i_scalar)
	{
		Matrix4 newMat;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				newMat.mat[i][j] = mat[i][j] * i_scalar;
		return newMat;
	}

	Matrix4 operator / (const float i_scalar)
	{
		Matrix4 newMat;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				newMat.mat[i][j] = mat[i][j] / i_scalar;
		return newMat;
	}



	// OPERATIONS - SELF
	Matrix4 transpose() const
	{
		float newMat[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				newMat[i][j] = mat[j][i];
			}
		}
	}
	


	// CONSTANTS
	static const Matrix4 Zero()
	{
		const Matrix4 zero;
		return zero;
	}
	static const Matrix4 Unit()
	{
		Matrix4 temp;
		temp.mat[0][0] = 1;
		temp.mat[1][1] = 1;
		temp.mat[2][2] = 1;
		temp.mat[3][3] = 1;
		return temp;
	}



	// TRANSFORMS
	static const Matrix4 Translation(const Vector4 i_translationVector)
	{
		Matrix4 temp = Matrix4::Unit();
		temp.mat[0][3] = i_translationVector.x;
		temp.mat[1][3] = i_translationVector.y;
		temp.mat[2][3] = i_translationVector.z;
		return temp;
	}

	static const Matrix4 Scale(const Vector4 i_scaleVector)
	{
		Matrix4 temp;
		temp.mat[0][0] = i_scaleVector.x;
		temp.mat[1][1] = i_scaleVector.y;
		temp.mat[2][2] = i_scaleVector.z;
		temp.mat[3][3] = 1;
		return temp;
	}

	static const Matrix4 ZRotation(const float i_zRotation)
	{
		Matrix4 temp = Matrix4::Unit();
		const float ToDeg = 1;
		temp.mat[0][0] = cosf(i_zRotation * ToDeg);
		temp.mat[0][1] = -sinf(i_zRotation * ToDeg);
		temp.mat[1][0] = sinf(i_zRotation * ToDeg);
		temp.mat[1][1] = cosf(i_zRotation * ToDeg);
		return temp;
	}

	
//private:
	float mat[4][4];





public:
	Matrix4 inverse() const
	{
		float m[16];
		{
			int count = 0;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					m[count++] = mat[i][j];
		}

		
		float inv[16];
		int i = 0;

		inv[0] = m[5] * m[10] * m[15] -
			m[5] * m[11] * m[14] -
			m[9] * m[6] * m[15] +
			m[9] * m[7] * m[14] +
			m[13] * m[6] * m[11] -
			m[13] * m[7] * m[10];

		inv[4] = -m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		inv[8] = m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		inv[12] = -m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		inv[5] = m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		inv[9] = -m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		inv[13] = m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		inv[2] = m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		inv[6] = -m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		inv[10] = m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		inv[14] = -m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		det = 1.f / det;
		Matrix4 output;
		{
			int count = 0;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					output.mat[i][j] = inv[count++] * det;
		}
		return output;
	}
};


// OPERATIONS - WITH OTHER MATRIX
inline Matrix4 operator * (const Matrix4 my, const Matrix4 other)
{
	Matrix4 newMat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
			{
				newMat.mat[i][j] += my.mat[i][k] * other.mat[k][j];
			}
	return newMat;
}

inline Matrix4 operator + (const Matrix4 my, const Matrix4 other)
{
	Matrix4 newMat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			newMat.mat[i][j] = my.mat[i][j] + other.mat[i][j];
	return newMat;
}

inline Matrix4 operator - (const Matrix4 my, const Matrix4 other)
{
	Matrix4 newMat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			newMat.mat[i][j] = my.mat[i][j] - other.mat[i][j];
	return newMat;
}


// OPERATIONS - WITH VECTORS
inline Vector4 operator * (const Matrix4 my, const Vector4 other)
{
	Vector4 newVec;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newVec.Set(i, newVec.Get(i) + (my.mat[i][j] * other.Get(j)));
	return newVec;
}