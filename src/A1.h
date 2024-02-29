#ifndef A1_H
#define A1_H

#include <iostream>
#include <string>
#include <algorithm> 

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"
#include "triangle.h"
#include "bounding_box.h"
#include "matrix44.h"
#include "affine_transform.h"


// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

#endif