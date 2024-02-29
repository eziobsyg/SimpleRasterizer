#include "affine_transform.h"
void AffineTransform::scale(float sx, float sy, float sz) {
	Matrix44 scaleMat(true);
	scaleMat[0][0] = sx;
	scaleMat[1][1] = sy;
	scaleMat[2][2] = sz;
	transform = scaleMat * transform;
}

void AffineTransform::rotateX(float angle) {
	Matrix44 rotMat(true);
	float rad = angle * M_PI / 180.0f; // angle to radian
	rotMat[1][1] = cos(rad);
	rotMat[1][2] = -sin(rad);
	rotMat[2][1] = sin(rad);
	rotMat[2][2] = cos(rad);
	transform = rotMat * transform;
	rotTransform = rotMat * rotTransform;
}

void AffineTransform::rotateY(float angle) {
	Matrix44 rotMat(true);
	float rad = angle * M_PI / 180.0f; // angle to radian
	rotMat[0][0] = cos(rad);
	rotMat[0][2] = sin(rad);
	rotMat[2][0] = -sin(rad);
	rotMat[2][2] = cos(rad);
	transform = rotMat * transform;
	rotTransform = rotMat * rotTransform;
}

void AffineTransform::rotateZ(float angle) {
	Matrix44 rotMat(true);
	float rad = angle * M_PI / 180.0f; // angle to radian
	rotMat[0][0] = cos(rad);
	rotMat[0][1] = -sin(rad);
	rotMat[1][0] = sin(rad);
	rotMat[1][1] = cos(rad);
	transform = rotMat * transform;
	rotTransform = rotMat * rotTransform;
}

void AffineTransform::translate(float tx, float ty, float tz) {
	Matrix44 transMat(true);
	transMat[0][3] = tx;
	transMat[1][3] = ty;
	transMat[2][3] = tz;
	transform = transMat * transform;
}

void AffineTransform::applyPoint(point4& point) const {
	point = transform * point;
}

void AffineTransform::applyVec(vec4& vec) const {
	vec = rotTransform * vec;
}

void AffineTransform::applyToTriangle(Triangle& triangle) const {
	point4 p0 = point4(triangle.p0);
	point4 p1 = point4(triangle.p1);
	point4 p2 = point4(triangle.p2);
	vec4 n0 = vec4(triangle.n0);
	vec4 n1 = vec4(triangle.n1);
	vec4 n2 = vec4(triangle.n2);
	applyPoint(p0);
	applyPoint(p1);
	applyPoint(p2);
	applyVec(n0);
	applyVec(n1);
	applyVec(n2);
	triangle.p0 = point3(p0[0], p0[1], p0[2]);
	triangle.p1 = point3(p1[0], p1[1], p1[2]);
	triangle.p2 = point3(p2[0], p2[1], p2[2]);
	triangle.n0 = vec3(n0[0], n0[1], n0[2]);
	triangle.n1 = vec3(n1[0], n1[1], n1[2]);
	triangle.n2 = vec3(n2[0], n2[1], n2[2]);
}

Matrix44 AffineTransform::getTransform() const {
	return transform;
}