#pragma once
#include <Eigen/dense>
#include "KDtree.h"
using namespace Eigen;




class Shader
{
public:
	Vector3d ambientLight;
	double shadowEpsilon = 0.05;
	Shader(Vector3d& ambientLight);
	Shader();
	Vector3d* shade(Ray& r, ShadeData* sd, KDtree * tree,
		vector<Light*>* Light);

	~Shader();
};

