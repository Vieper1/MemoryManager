#pragma once

#include "Vector3.h"


class Matrix3 {
public:
	Matrix3() : mat{0}
	{}

	Matrix3(const float i_mat[3][3])
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				mat[i][j] = i_mat[i][j];
	}



	Vector3 row(const int i)
	{
		return Vector3{ mat[i][0], mat[i][1], mat[i][2] };
	}

	Vector3 col(const int i)
	{
		return Vector3{ mat[0][i], mat[1][i], mat[2][i] };
	}
	
	// OPERATIONS - WITH SCALAR
	Matrix3 operator + (const float i_scalar)
	{
		Matrix3 newMat;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				newMat.mat[i][j] = mat[i][j] + i_scalar;
		return newMat;
	}

	Matrix3 operator - (const float i_scalar)
	{
		Matrix3 newMat;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				newMat.mat[i][j] = mat[i][j] - i_scalar;
		return newMat;
	}
	
	Matrix3 operator * (const float i_scalar)
	{
		Matrix3 newMat;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				newMat.mat[i][j] = mat[i][j] * i_scalar;
		return newMat;
	}

	Matrix3 operator / (const float i_scalar)
	{
		Matrix3 newMat;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				newMat.mat[i][j] = mat[i][j] / i_scalar;
		return newMat;
	}

	
	// OPERATIONS - WITH OTHER MATRIX
	Matrix3 operator * (const Matrix3 other)
	{
		Matrix3 newMat;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				for (int k = 0; k < 3; ++k)
				{
					newMat.mat[i][j] += mat[i][k] * other.mat[k][j];
				}
		return newMat;
	}

	Matrix3 operator + (const Matrix3 other)
	{
		Matrix3 newMat;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				newMat.mat[i][j] = mat[i][j] + other.mat[i][j];
		return newMat;
	}

	Matrix3 operator - (const Matrix3 other)
	{
		Matrix3 newMat;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				newMat.mat[i][j] = mat[i][j] - other.mat[i][j];
		return newMat;
	}



	// OPERATIONS - SELF
	Matrix3 transpose() const
	{
		float newMat[3][3];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				newMat[i][j] = mat[j][i];
			}
		}
	}

	Matrix3 inverse() const
	{
		
	}



	// CONSTANTS
	static const Matrix3 Zero()
	{
		const Matrix3 zero;
		return zero;
	}
	static const Matrix3 Unit()
	{
		Matrix3 temp;
		temp.mat[0][0] = 1;
		temp.mat[1][1] = 1;
		temp.mat[2][2] = 1;
		temp.mat[3][3] = 1;
		return temp;
	}



	// TRANSFORMS
	static const Matrix3 Translation(const Vector3 i_translationVector)
	{
		Matrix3 temp = Matrix3::Unit();
		temp.mat[3][0] = i_translationVector.x;
		temp.mat[3][1] = i_translationVector.y;
		temp.mat[3][2] = i_translationVector.z;
		return temp;
	}

	static const Matrix3 Scale(const Vector3 i_scaleVector)
	{
		Matrix3 temp;
		temp.mat[0][0] = i_scaleVector.x;
		temp.mat[1][1] = i_scaleVector.y;
		temp.mat[2][2] = i_scaleVector.z;
		temp.mat[3][3] = 1;
		return temp;
	}

	static const Matrix3 ZRotation(const float i_zRotation)
	{
		Matrix3 temp = Matrix3::Unit();
		temp.mat[0][0] = cos(i_zRotation);
		temp.mat[0][1] = -sin(i_zRotation);
		temp.mat[1][0] = sin(i_zRotation);
		temp.mat[1][1] = cos(i_zRotation);
		return temp;
	}

	
private:
	float mat[3][3];
};
