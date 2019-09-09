#include "Light.h"
//#include "ShadeData.h"
#include <iostream>
#include <Eigen/Dense>
#include "World.h"
#include "Geometry.h"

using namespace Eigen;
Light::Light()
{
}


Light::~Light()
{
}

Ambient::Ambient()
	: Light(){
	ls = 1.0;
	color = Vector3d(1.0, 1.0, 1.0);
}

Vector3d
Ambient::get_direction(ShadeData* s) {
	return (Vector3d(0.0,0.0,0.0));
}

Vector3d Ambient::L(ShadeData* sr) {
	return (ls * color);
}

AreaLight::AreaLight() {

}

Ambient::~Ambient() {

}

void AreaLight::setObject(Geometry* obj_ptr) {
	object = obj_ptr;
//	material = obj_ptr->material;
//	cout << obj_ptr << "\n";
//	cout << obj_ptr->material << "\n";
}

Vector3d AreaLight::get_direction(ShadeData* sr) {
	samplePoint = object->sample();    // used in the G function
	//cout << samplePoint << "\n";
	normal = object->getNormal(samplePoint);
	wi = samplePoint - sr->hitPoint;  		// used in the G function
	wi = wi / wi.norm();

	return wi;
}



Vector3d
AreaLight::L(ShadeData* sr) {
	float ndotd = -normal.dot(wi);

	if (ndotd > 0.0)
		return (material->getLe(sr));
	else
		return Vector3d(0.0,0.0,0.0);
}

bool
AreaLight::inShadow(Ray& ray, ShadeData* sr) {
//	float t;
	//int num_objects = sr->w->objects.size();
	float ts = (samplePoint - ray.origin)[0]/ray.direction[0];
	//cout << ts << "\n";
	return sr->w->tracer->traceShadowRay(ray, ts);
}

float
AreaLight::pdf(ShadeData* sr)  {
	return object->pdf(sr);
}

float
AreaLight::G(ShadeData* sr) {
	float ndotd = (-normal).dot(wi);
	float d2 = (samplePoint - sr->hitPoint).norm()*2;

	return (ndotd / d2);
}

PointLight::PointLight(){

}

PointLight::~PointLight() {

}

PointLight::PointLight(Vector3d col, Vector3d pos, float l) {
	color = col;
	position = pos;
	ls = l;
}

bool
PointLight::inShadow(Ray& ray, ShadeData* sr) {
//	float t;
	int num_objects = sr->w->objects.size();
	float ts = (position - ray.origin)[0] / ray.direction[0];
	return sr->w->tracer->traceShadowRay(ray, ts);
}

Vector3d PointLight::get_direction(ShadeData* sr) {
	Vector3d wi = position - sr->hitPoint;  		// used in the G function
	wi = wi / wi.norm();

	return wi;
}

Vector3d PointLight::L(ShadeData* sr) {
	return color*ls;
}