#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "triangle.h"
#include "affine_transform.h"

class BoundingBox {
private:
	float xmin, xmax, ymin, ymax, zmin, zmax;

public:
	BoundingBox() : xmin(std::numeric_limits<float>::max()), xmax(std::numeric_limits<float>::lowest()),
		ymin(std::numeric_limits<float>::max()), ymax(std::numeric_limits<float>::lowest()),
		zmin(std::numeric_limits<float>::max()), zmax(std::numeric_limits<float>::lowest()) {}

	float getXMin() { return xmin; }
	float getXMax() { return xmax; }
	float getYMin() { return ymin; }
	float getYMax() { return ymax; }
	float getZMin() { return zmin; }
	float getZMax() { return zmax; }

	void updateBounds(float x, float y, float z);

	void updateBoundsbyTriangle(const Triangle& t);
};

vector<BoundingBox> convertTrianglesToBoundingBoxes(vector<Triangle>& Triangles);
BoundingBox convertObjToViewBox(vector<Triangle>& Triangles);
#endif