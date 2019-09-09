#pragma once
#include <stdlib.h>
#include <string>
#include <Eigen/dense>
#include <vector>
#include "Geometry.h"
using namespace std;
using namespace Eigen;
class ObjReader
{
public:
	

	ObjReader();

	~ObjReader();

	static bool read(const char* path, vector<Vector3d*> &vertices, vector<Triangle*> &triangles);
};

