#include "rasterizer.h"

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};

bool checkExtension(const string& fileName, const string& extension) {
	size_t pos = fileName.rfind(extension);
	return pos != string::npos && pos == fileName.length() - extension.length();
}

float cap(float num, float min, float max) {
	if (num > max) {
		return max;
	}
	else if (num < min) {
		return min;
	}
	else {
		return num;
	}
}

void drawBoundingBoxes(shared_ptr<Image> img, vector<BoundingBox>& BoundingBoxes) {
	int width = img->getWidth();
	int height = img->getHeight();
	point3 lowerleft(-width / 2.0f, -height / 2.0f, 0.0f);
	for (size_t index = 0; index < BoundingBoxes.size(); index++) {
		int leftpixel = static_cast<int>(floor(BoundingBoxes[index].getXMin() - lowerleft[0]));
		int rightpixel = static_cast<int>(floor(BoundingBoxes[index].getXMax() - lowerleft[0]));
		int upperpixel = static_cast<int>(floor(BoundingBoxes[index].getYMax() - lowerleft[1]));
		int lowerpixel = static_cast<int>(floor(BoundingBoxes[index].getYMin() - lowerleft[1]));

		for (size_t y = lowerpixel; y <= upperpixel; y++) {
			if (y == height) {
				break;
			}
			for (size_t x = leftpixel; x <= rightpixel; x++) {
				if (x == width) {
					break;
				}
				unsigned char r = static_cast<unsigned char>(RANDOM_COLORS[index % 7][0] * 256);
				unsigned char g = static_cast<unsigned char>(RANDOM_COLORS[index % 7][1] * 256);
				unsigned char b = static_cast<unsigned char>(RANDOM_COLORS[index % 7][2] * 256);
				img->setPixel(x, y, r, g, b);
			}
		}
	}
}

void drawTriangles(shared_ptr<Image> img, vector<Triangle>& Triangles, vector<BoundingBox>& BoundingBoxes) {
	int width = img->getWidth();
	int height = img->getHeight();
	point3 lowerleft(-width / 2.0, -height / 2.0, 0);
	for (size_t index = 0; index < BoundingBoxes.size(); index++) {
		int leftpixel = static_cast<int>(floor(BoundingBoxes[index].getXMin() - lowerleft[0]));
		int rightpixel = static_cast<int>(floor(BoundingBoxes[index].getXMax() - lowerleft[0]));
		int upperpixel = static_cast<int>(floor(BoundingBoxes[index].getYMax() - lowerleft[1]));
		int lowerpixel = static_cast<int>(floor(BoundingBoxes[index].getYMin() - lowerleft[1]));

		for (size_t y = lowerpixel; y <= upperpixel; y++) {
			if (y == height) {
				break;
			}
			for (size_t x = leftpixel; x <= rightpixel; x++) {
				if (x == width) {
					break;
				}

				point3 pixelCenter(lowerleft[0] + x + 0.5, lowerleft[1] + y + 0.5, 0);
				if (Triangles[index].isPointInsideXY(pixelCenter)) {
					unsigned char r = static_cast<unsigned char>(RANDOM_COLORS[index % 7][0] * 256);
					unsigned char g = static_cast<unsigned char>(RANDOM_COLORS[index % 7][1] * 256);
					unsigned char b = static_cast<unsigned char>(RANDOM_COLORS[index % 7][2] * 256);
					img->setPixel(x, y, r, g, b);
				}
			}
		}
	}
}

void interpolateColors(shared_ptr<Image> img, vector<Triangle>& Triangles, vector<BoundingBox>& BoundingBoxes) {
	int width = img->getWidth();
	int height = img->getHeight();
	point3 lowerleft(-width / 2.0, -height / 2.0, 0);
	for (size_t index = 0; index < BoundingBoxes.size(); index++) {
		int leftpixel = static_cast<int>(floor(BoundingBoxes[index].getXMin() - lowerleft[0]));
		int rightpixel = static_cast<int>(floor(BoundingBoxes[index].getXMax() - lowerleft[0]));
		int upperpixel = static_cast<int>(floor(BoundingBoxes[index].getYMax() - lowerleft[1]));
		int lowerpixel = static_cast<int>(floor(BoundingBoxes[index].getYMin() - lowerleft[1]));

		for (size_t y = lowerpixel; y <= upperpixel; y++) {
			if (y == height) {
				break;
			}
			for (size_t x = leftpixel; x <= rightpixel; x++) {
				if (x == width) {
					break;
				}

				point3 pixelCenter(lowerleft[0] + x + 0.5, lowerleft[1] + y + 0.5, 0);
				if (Triangles[index].isPointInsideXY(pixelCenter)) {
					vec3 lambda = Triangles[index].computeLambda(pixelCenter);
					vector<vec3> colors;
					for (size_t k = 0; k < 3; k++) {
						float r = RANDOM_COLORS[(index * 3 + k) % 7][0];
						float g = RANDOM_COLORS[(index * 3 + k) % 7][1];
						float b = RANDOM_COLORS[(index * 3 + k) % 7][2];
						colors.push_back(vec3(r, g, b));
					}
					vec3 color = lambda[0] * colors[0] + lambda[1] * colors[1] + lambda[2] * colors[2];
					float r = color[0] * 256.0f;
					float g = color[1] * 256.0f;
					float b = color[2] * 256.0f;
					r = cap(r, 0.0f, 255.9999f);
					g = cap(g, 0.0f, 255.9999f);
					b = cap(b, 0.0f, 255.9999f);

					unsigned char ucr = static_cast<unsigned char>(r);
					unsigned char ucg = static_cast<unsigned char>(g);
					unsigned char ucb = static_cast<unsigned char>(b);
					img->setPixel(x, y, ucr, ucg, b);
				}
			}
		}
	}
}

void verticalColor(shared_ptr<Image> img, vector<Triangle>& Triangles, vector<BoundingBox>& BoundingBoxes, BoundingBox& viewBox) {
	int width = img->getWidth();
	int height = img->getHeight();
	point3 lowerleft(-width / 2.0, -height / 2.0, 0);
	for (size_t index = 0; index < BoundingBoxes.size(); index++) {
		int leftpixel = static_cast<int>(floor(BoundingBoxes[index].getXMin() - lowerleft[0]));
		int rightpixel = static_cast<int>(floor(BoundingBoxes[index].getXMax() - lowerleft[0]));
		int upperpixel = static_cast<int>(floor(BoundingBoxes[index].getYMax() - lowerleft[1]));
		int lowerpixel = static_cast<int>(floor(BoundingBoxes[index].getYMin() - lowerleft[1]));

		for (size_t y = lowerpixel; y <= upperpixel; y++) {
			if (y == height) {
				break;
			}
			for (size_t x = leftpixel; x <= rightpixel; x++) {
				if (x == width) {
					break;
				}

				point3 pixelCenter(lowerleft[0] + x + 0.5, lowerleft[1] + y + 0.5, 0);
				if (Triangles[index].isPointInsideXY(pixelCenter)) {
					float lambda = (pixelCenter[1] - viewBox.getYMin()) / (viewBox.getYMax() - viewBox.getYMin());
					vec3 blue = vec3(0, 0, 255);
					vec3 red = vec3(255, 0, 0);
					vec3 color = lambda * red + (1 - lambda) * blue;
					color[0] = cap(color[0], 0.0f, 255.9999f);
					color[1] = cap(color[1], 0.0f, 255.9999f);
					color[2] = cap(color[2], 0.0f, 255.9999f);
					unsigned char r = static_cast<unsigned char>(color[0]);
					unsigned char g = static_cast<unsigned char>(color[1]);
					unsigned char b = static_cast<unsigned char>(color[2]);
					img->setPixel(x, y, r, g, b);
				}
			}
		}
	}
}


void zBuffering(shared_ptr<Image> img, vector<Triangle>& Triangles, vector<BoundingBox>& BoundingBoxes, BoundingBox& viewBox, vector<vector<float>>& zBuffer) {
	int width = img->getWidth();
	int height = img->getHeight();
	point3 lowerleft(-width / 2.0, -height / 2.0, 0);
	for (size_t index = 0; index < BoundingBoxes.size(); index++) {
		int leftpixel = static_cast<int>(floor(BoundingBoxes[index].getXMin() - lowerleft[0]));
		int rightpixel = static_cast<int>(floor(BoundingBoxes[index].getXMax() - lowerleft[0]));
		int upperpixel = static_cast<int>(floor(BoundingBoxes[index].getYMax() - lowerleft[1]));
		int lowerpixel = static_cast<int>(floor(BoundingBoxes[index].getYMin() - lowerleft[1]));

		for (size_t y = lowerpixel; y <= upperpixel; y++) {
			if (y == height) {
				break;
			}
			for (size_t x = leftpixel; x <= rightpixel; x++) {
				if (x == width) {
					break;
				}

				point3 pixelCenter(lowerleft[0] + x + 0.5, lowerleft[1] + y + 0.5, 0);
				if (Triangles[index].isPointInsideXY(pixelCenter)) {
					vec3 lambda = Triangles[index].computeLambda(pixelCenter);
					pixelCenter[2] = lambda[0] * Triangles[index].p0[2] + lambda[1] * Triangles[index].p1[2] + lambda[2] * Triangles[index].p2[2];
					if (pixelCenter[2] > zBuffer[y][x]) {
						zBuffer[y][x] = pixelCenter[2];
						float lambdaf = (pixelCenter[2] - viewBox.getZMin()) / (viewBox.getZMax() - viewBox.getZMin());
						float red = lambdaf * 256.0f;
						red = cap(red, 0.0f, 255.9999f);
						unsigned char r = static_cast<unsigned char>(red);
						unsigned char g = 0;
						unsigned char b = 0;
						img->setPixel(x, y, r, g, b);
					}
				}
			}
		}
	}
}

void normalColoring(shared_ptr<Image> img, vector<Triangle>& Triangles, vector<BoundingBox>& BoundingBoxes, vector<vector<float>>& zBuffer) {
	int width = img->getWidth();
	int height = img->getHeight();
	point3 lowerleft(-width / 2.0, -height / 2.0, 0);
	for (size_t index = 0; index < BoundingBoxes.size(); index++) {
		int leftpixel = static_cast<int>(floor(BoundingBoxes[index].getXMin() - lowerleft[0]));
		int rightpixel = static_cast<int>(floor(BoundingBoxes[index].getXMax() - lowerleft[0]));
		int upperpixel = static_cast<int>(floor(BoundingBoxes[index].getYMax() - lowerleft[1]));
		int lowerpixel = static_cast<int>(floor(BoundingBoxes[index].getYMin() - lowerleft[1]));

		for (size_t y = lowerpixel; y <= upperpixel; y++) {
			if (y == height) {
				break;
			}
			for (size_t x = leftpixel; x <= rightpixel; x++) {
				if (x == width) {
					break;
				}
				point3 pixelCenter(lowerleft[0] + x + 0.5, lowerleft[1] + y + 0.5, 0);
				if (Triangles[index].isPointInsideXY(pixelCenter)) {
					vec3 lambda = Triangles[index].computeLambda(pixelCenter);
					pixelCenter[2] = lambda[0] * Triangles[index].p0[2] + lambda[1] * Triangles[index].p1[2] + lambda[2] * Triangles[index].p2[2];
					if (pixelCenter[2] > zBuffer[y][x]) {
						vec3 normal = lambda[0] * Triangles[index].n0 + lambda[1] * Triangles[index].n1 + lambda[2] * Triangles[index].n2;
						zBuffer[y][x] = pixelCenter[2];
						float red = 255.0f * (normal[0] * 0.5f + 0.5f);
						float green = 255.0f * (normal[1] * 0.5f + 0.5f);
						float blue = 255.0f * (normal[2] * 0.5f + 0.5f);
						red = cap(red, 0.0f, 255.9999f);
						green = cap(green, 0.0f, 255.9999f);
						blue = cap(blue, 0.0f, 255.9999f);
						unsigned char r = static_cast<unsigned char>(red);
						unsigned char g = static_cast<unsigned char>(green);
						unsigned char b = static_cast<unsigned char>(blue);
						img->setPixel(x, y, r, g, b);
					}
				}
			}
		}
	}
}

void simpleLighting(shared_ptr<Image> img, vector<Triangle>& Triangles, vector<BoundingBox>& BoundingBoxes, vector<vector<float>>& zBuffer, vec3& light) {
	int width = img->getWidth();
	int height = img->getHeight();
	point3 lowerleft(-width / 2.0, -height / 2.0, 0);
	for (size_t index = 0; index < BoundingBoxes.size(); index++) {
		int leftpixel = static_cast<int>(floor(BoundingBoxes[index].getXMin() - lowerleft[0]));
		int rightpixel = static_cast<int>(floor(BoundingBoxes[index].getXMax() - lowerleft[0]));
		int upperpixel = static_cast<int>(floor(BoundingBoxes[index].getYMax() - lowerleft[1]));
		int lowerpixel = static_cast<int>(floor(BoundingBoxes[index].getYMin() - lowerleft[1]));

		for (size_t y = lowerpixel; y <= upperpixel; y++) {
			if (y == height) {
				break;
			}
			for (size_t x = leftpixel; x <= rightpixel; x++) {
				if (x == width) {
					break;
				}
				point3 pixelCenter(lowerleft[0] + x + 0.5, lowerleft[1] + y + 0.5, 0);
				if (Triangles[index].isPointInsideXY(pixelCenter)) {
					vec3 lambda = Triangles[index].computeLambda(pixelCenter);
					pixelCenter[2] = lambda[0] * Triangles[index].p0[2] + lambda[1] * Triangles[index].p1[2] + lambda[2] * Triangles[index].p2[2];
					if (pixelCenter[2] > zBuffer[y][x]) {
						vec3 normal = lambda[0] * Triangles[index].n0 + lambda[1] * Triangles[index].n1 + lambda[2] * Triangles[index].n2;
						zBuffer[y][x] = pixelCenter[2];
						float c = max(dot(light, normal), 0.0f);
						float red = 256.0f * c;
						float green = 256.0f * c;
						float blue = 256.0f * c;
						red = cap(red, 0.0f, 255.9999f);
						green = cap(green, 0.0f, 255.9999f);
						blue = cap(blue, 0.0f, 255.9999f);
						unsigned char r = static_cast<unsigned char>(red);
						unsigned char g = static_cast<unsigned char>(green);
						unsigned char b = static_cast<unsigned char>(blue);
						img->setPixel(x, y, r, g, b);
					}
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	if (argc < 6) {
		cout << "Error: Missing arguments." << endl;
		cout << "Usage: A1 meshfile outputFileName width height taskNum" << endl;
		return 1; // return an error code
	}

	string meshName(argv[1]);
	string outputFileName(argv[2]);
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	int taskNum = atoi(argv[5]);

	//Check if the input is valid
	if (!checkExtension(meshName, ".obj")) {
		cout << "Error: meshfile must end with .obj" << endl;
		return 1;
	}

	if (!checkExtension(outputFileName, ".png")) {
		cout << "Error: outputFileName must end with .png" << endl;
		return 1;
	}

	if (width <= 0 || height <= 0) {
		cout << "Error: Width and height must be positive integers." << endl;
		return 1;
	}

	if (taskNum < 1 || taskNum > 8) {
		cout << "Error: Task number must be an integer between 1 and 8." << endl;
		return 1;
	}

	// Load geometry
	vector<float> posBuf; // list of vertex positions
	vector<float> norBuf; // list of vertex normals
	vector<float> texBuf; // list of vertex texture coords
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
					if(!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
					}
					if(!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;

	// Get triangles and view bounding boxes
	vector<Triangle> Triangles;
	vector<BoundingBox> BoundingBoxes;
	BoundingBox viewBox;
	Triangles = convertToTriangles(posBuf);
	if (!norBuf.empty()) {
		updateNormals(Triangles, norBuf);
	}
	if (taskNum == 8) {
		AffineTransform preAffineTransform = AffineTransform();
		preAffineTransform.rotateY(45);
		for (size_t i = 0; i < Triangles.size(); i++) {
			preAffineTransform.applyToTriangle(Triangles[i]);
		}
	}
	viewBox = convertObjToViewBox(Triangles);

	// Compute scale and tranlation factors
	vec3 viewCenter = vec3((viewBox.getXMax() + viewBox.getXMin()) / 2, 
		(viewBox.getYMax() + viewBox.getYMin()) / 2, 
		(viewBox.getZMax() + viewBox.getZMin()) / 2);
	float viewBoxWidth = viewBox.getXMax() - viewBox.getXMin();
	float viewBoxHeight = viewBox.getYMax() - viewBox.getYMin();
	float scaleRate;
	if (viewBoxWidth / viewBoxHeight > width / height) {
		scaleRate = width / viewBoxWidth;
	}
	else {
		scaleRate = height / viewBoxHeight;
	}

	//viewing transformation and get bounding boxes
	AffineTransform affineTransform = AffineTransform();
	affineTransform.translate(-viewCenter[0], -viewCenter[1], -viewCenter[2]);
	affineTransform.scale(scaleRate, scaleRate, scaleRate);
	for (size_t i = 0; i < Triangles.size(); i++) {
		affineTransform.applyToTriangle(Triangles[i]);
	}
	BoundingBoxes = convertTrianglesToBoundingBoxes(Triangles);
	BoundingBox newViewBox;
	newViewBox = convertObjToViewBox(Triangles);
	vector<vector<float>> zBuffer(height, vector<float>(width, numeric_limits<float>::lowest()));

	auto image = make_shared<Image>(width, height);

	if (taskNum == 1) {
		drawBoundingBoxes(image, BoundingBoxes);
	}
	else if (taskNum == 2) {
		drawTriangles(image, Triangles, BoundingBoxes);
	}
	else if (taskNum == 3) {
		interpolateColors(image, Triangles, BoundingBoxes);
	}
	else if (taskNum == 4) {
		verticalColor(image, Triangles, BoundingBoxes, newViewBox);
	}
	else if (taskNum == 5) {
		zBuffering(image, Triangles, BoundingBoxes, newViewBox, zBuffer);
	}
	else if (taskNum == 6) {
		normalColoring(image, Triangles, BoundingBoxes, zBuffer);
	}
	else if (taskNum == 7 || taskNum == 8) {
		vec3 light = vec3(1, 1, 1) / sqrt(3.0f);
		simpleLighting(image, Triangles, BoundingBoxes, zBuffer, light);
	}
	// Write image to file
	image->writeToFile(outputFileName);
	return 0;
}
