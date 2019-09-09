#include "Tracer.h"
#include "World.h"

extern int rayCount;
Tracer::Tracer()
{
}


Tracer::~Tracer()
{
}


Vector3d
Tracer::traceRay(Ray& ray)  {
	ShadeData *sr = world->hit_objects(ray);

	if (sr != nullptr) {
		if (sr->hitObject) {
			sr->ray = ray;
			Vector3d out = (sr->material->shade(sr));
			delete sr;
			return out;
		}
		delete sr;
	}
	return (world->backGround);
}

Vector3d
Tracer::traceRay(Ray& ray, int depth) {
	rayCount += 1;
	if (depth > 3) {
		return world->backGround;
	}
	rayCount += 1;
	ShadeData *sr = world->hit_objects(ray);

	if (sr != nullptr) {
		if (sr->hitObject) {
			sr->ray = ray;
			sr->depth = depth;
			Vector3d out = (sr->material->shade(sr));
			delete sr;
			return out;
		}
		delete sr;
	}
	return (world->backGround);
}

Vector3d
Tracer::traceRayArea(Ray& ray, int depth) {
	if (depth > 3) {
		return Vector3d(0.0, 0.0, 0.0);
	}
	rayCount += 1;
	ShadeData *sr = world->hit_objects(ray);

	if (sr != nullptr) {
		if (sr->hitObject) {
			sr->ray = ray;
			sr->depth = depth;
			
			Vector3d out = (sr->material->areaLightShade(sr));
			delete sr;
			return out;
		}
		delete sr;
	}
	return (world->backGround);
}

Vector3d
Tracer::traceRayArea(Ray& ray) {
	ShadeData *sr = world->hit_objects(ray);
	rayCount += 1;
	if (sr != nullptr) {
		if (sr->hitObject) {
			sr->ray = ray;
			Vector3d out = (sr->material->areaLightShade(sr));
			delete sr;
			return out;
		}
		delete sr;
	}
	return (world->backGround);
}

bool
Tracer::traceShadowRay(Ray& ray, float d) {
	return world->hit_objectsShallow(ray,d);

}