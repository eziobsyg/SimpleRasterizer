#include <vector>
#include "Triangle.h"

bool Triangle::isPointInsideXY(const point3& p) {
	const float nepsilon = -0.0001f;
	const float pepsilon = 0.0001f;

	vec3 v01 = p1 - p0;
	vec3 v12 = p2 - p1;
	vec3 v20 = p0 - p2;
	vec3 v0p = p - p0;
	vec3 v1p = p - p1;
	vec3 v2p = p - p2;

	vec3 cross1 = cross(v01, v0p);
	vec3 cross2 = cross(v12, v1p);
	vec3 cross3 = cross(v20, v2p);

	if ((cross1[2] > nepsilon&& cross2[2] > nepsilon&& cross3[2] > nepsilon) || (cross1[2] < pepsilon && cross2[2] < pepsilon && cross3[2] < pepsilon)) {
		return true;
	}
	else {
		return false;
	}
}

float Triangle::computeAreaXY(const point3& a, const point3& b, const point3& c) {
	vec3 ab = vec3(b[0] - a[0], b[1] - a[1], 0);
	vec3 ac = vec3(c[0] - a[0], c[1] - a[1], 0);
	vec3 result = cross(ab, ac);
	return 0.5f * fabs(result[2]);
}

vec3 Triangle::computeLambda(const point3& pixelCenter) {
	float areaABC = computeAreaXY(this->p0, this->p1, this->p2);
	float areaPBC = computeAreaXY(pixelCenter, this->p1, this->p2);
	float areaPCA = computeAreaXY(pixelCenter, this->p2, this->p0);
	float lambdaA = areaPBC / areaABC;
	float lambdaB = areaPCA / areaABC;
	float lambdaC = 1.0f - lambdaA - lambdaB; 
	vec3 lambda = vec3(lambdaA, lambdaB, lambdaC);
	return lambda;
}

vector<Triangle> convertToTriangles(const vector<float>& posBuf) {
	vector<Triangle> triangles;

	if (posBuf.size() % 9 != 0) {
		throw invalid_argument("posBuf size is not a multiple of 9.");
	}

	for (size_t i = 0; i < posBuf.size(); i += 9) {
		point3 p0(posBuf[i], posBuf[i + 1], posBuf[i + 2]);
		point3 p1(posBuf[i + 3], posBuf[i + 4], posBuf[i + 5]);
		point3 p2(posBuf[i + 6], posBuf[i + 7], posBuf[i + 8]);

		triangles.push_back(Triangle(p0, p1, p2));
	}

	return triangles;
}

void updateNormals(vector<Triangle>& Triangles, const vector<float>& norBuf) {
	if (norBuf.size() % 9 != 0) {
		throw invalid_argument("norBuf size is not a multiple of 9.");
	}

	for (size_t i = 0; i < norBuf.size(); i += 9) {
		int index = i / 9;
		Triangles[index].n0 = vec3(norBuf[i], norBuf[i + 1], norBuf[i + 2]);
		Triangles[index].n1 = vec3(norBuf[i + 3], norBuf[i + 4], norBuf[i + 5]);
		Triangles[index].n2 = vec3(norBuf[i + 6], norBuf[i + 7], norBuf[i + 8]);
	}

}

