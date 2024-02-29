#include <vector>
#include "bounding_box.h"

void BoundingBox::updateBounds(float x, float y, float z) {
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	if (z < zmin) zmin = z;
	if (z > zmax) zmax = z;
}

void BoundingBox::updateBoundsbyTriangle(const Triangle& t) {
	updateBounds(t.p0[0], t.p0[1], t.p0[2]);
	updateBounds(t.p1[0], t.p1[1], t.p1[2]);
	updateBounds(t.p2[0], t.p2[1], t.p2[2]);
}

vector<BoundingBox> convertTrianglesToBoundingBoxes(vector<Triangle>& Triangles) {
	vector<BoundingBox> BoundingBoxes;

	for (size_t i = 0; i < Triangles.size(); i++) {
		BoundingBox boundingBox = BoundingBox();
		boundingBox.updateBoundsbyTriangle(Triangles[i]);
		BoundingBoxes.push_back(boundingBox);
	}

	return BoundingBoxes;
}

BoundingBox convertObjToViewBox(vector<Triangle>& Triangles) {
	BoundingBox viewBox;
	for (size_t i = 0; i < Triangles.size(); i++) {
		viewBox.updateBoundsbyTriangle(Triangles[i]);
	}
	return viewBox;
}