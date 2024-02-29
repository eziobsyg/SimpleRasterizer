#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include <cmath>
#include <iostream>
#include "matrix44.h"
#include "vec3.h"
#include "vec4.h"
#include "triangle.h"

#define M_PI 3.14159265358979323846
class AffineTransform {
private:
	Matrix44 transform;
	Matrix44 rotTransform;

public:
	AffineTransform() : transform(Matrix44(true)), rotTransform(Matrix44(true)) {}

	void scale(float sx, float sy, float sz);

	void rotateX(float angle);

	void rotateY(float angle);

	void rotateZ(float angle);

	void translate(float tx, float ty, float tz);

	void applyPoint(point4& point) const;
	void applyVec(vec4& vec) const;

	void applyToTriangle(Triangle& triangle) const;

	Matrix44 getTransform() const;
};

#endif