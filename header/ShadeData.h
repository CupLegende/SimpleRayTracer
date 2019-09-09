#pragma once
#include <Eigen/Dense>
#include "Ray.h"
class Material;
class World;

using namespace Eigen;




struct ShadeData {
	Ray ray;
	Material * material= NULL;
	Vector3d normal;
	Vector3d hitPoint;
	int depth;
	bool hitObject;
	World* w;
	float t = FLT_MAX;
	bool castShadow;
	float maxt = FLT_MAX;
};