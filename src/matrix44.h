#ifndef MATRIX44_H
#define MATRIX44_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Matrix44 {
public:
	float m[4][4];
	Matrix44(bool isIdentity) {
		if (isIdentity) {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
		else {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m[i][j] = 0.0f;
		}
	}

	Matrix44(float mat[4][4]) {
		memcpy(m, mat, 16 * sizeof(float));
	}

	float* operator[](int i) {
		return m[i];
	}

	const float* operator[](int i) const {
		return m[i];
	}
};

inline Matrix44 operator+(const Matrix44& A, const Matrix44& B) {
	Matrix44 matrix(false);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix[i][j] = A[i][j] + B[i][j];
	return matrix;
}

inline Matrix44 operator-(const Matrix44& A, const Matrix44& B) {
	Matrix44 matrix(false);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix[i][j] = A[i][j] - B[i][j];
	return matrix;
}

inline Matrix44 operator*(const Matrix44& A, const Matrix44& B) {
	Matrix44 matrix(false);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				matrix[i][j] += A[i][k] * B[k][j];
			}
		}		
	}
	return matrix;
}
#endif