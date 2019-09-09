#pragma once

#include <Eigen/Dense>
#include "Ray.h"

using namespace Eigen;
class World;

class Tracer
{
	public:
	
	Tracer();
	~Tracer();
	Vector3d traceRay(Ray& ray);
	Vector3d traceRay(Ray& ray, int depth);
	Vector3d traceRayArea(Ray& ray, int depth);
	Vector3d traceRayArea(Ray& ray);
	bool traceShadowRay(Ray& ray, float d);
	World* world;
};

