#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>
#include <iostream>
#include "vec3.h"
#include <vector>
using namespace std;

class Triangle {
public:
	point3 p0, p1, p2;
	vec3 n0, n1, n2;

	Triangle(const point3& p0, const point3& p1, const point3& p2) : p0(p0), p1(p1), p2(p2) {}
	bool isPointInsideXY(const point3& p);
	float computeAreaXY(const point3& a, const point3& b, const point3& c);
	vec3 computeLambda(const point3& pixelCenter);
};

vector<Triangle> convertToTriangles(const vector<float>& posBuf);
void updateNormals(vector<Triangle> &Triangles, const vector<float>& norBuf);
#endif