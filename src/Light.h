#pragma once
#include <Eigen/Dense>
#include "Ray.h"
#include "Material.h"
#include "Geometry.h"


using namespace Eigen;
struct ShadeData;
//class Material;
//class Geometry;
//struct ShadeData;
class Light
{
public:
	Light();
	~Light();

	virtual Vector3d
		get_direction(ShadeData* sr) {
		return Vector3d(0.0, 0.0, 0.0);
	}
	virtual Vector3d
		L(ShadeData* sr) {
		return Vector3d(0.0, 0.0, 0.0);
	}
	virtual float
		pdf(ShadeData* sr) {
		return 0.0;
	}
	virtual float
		G(ShadeData* sr) {
		return 1.0;
	}
	virtual bool
		inShadow(Ray& ray, ShadeData* sr) {
		return false;
	}
};

class Ambient:public Light
{
public:
	Ambient();
	~Ambient();

	Vector3d
		get_direction(ShadeData* sr);
	Vector3d
		L(ShadeData* sr);
	//float
		//pdf(ShadeData* sr);
	//float
		//G(ShadeData* sr);
	//bool
		//inShadow(Ray& ray,ShadeData* sr);
	float ls;
	Vector3d color;
};

class PointLight :public Light
{
public:
	PointLight();
	~PointLight();
	PointLight(Vector3d color, Vector3d position, float ls);
	

	Vector3d
		get_direction(ShadeData* sr);
	Vector3d
		L(ShadeData* sr);
	//float
		//pdf(ShadeData* sr);
	//float
		//G(ShadeData* sr);
	bool
		inShadow(Ray& ray, ShadeData* sr);
	float ls = 1.0;
	Vector3d color;
	Vector3d position;
};



class AreaLight :public Light 
{
public:
	AreaLight();
	~AreaLight();

	void
		setObject(Geometry* obj_ptr);
	Vector3d
		get_direction(ShadeData* sr);
	Vector3d
		L(ShadeData* sr);
	float
		pdf(ShadeData* sr);
	float
		G(ShadeData* sr);
	bool
		inShadow(Ray& ray, ShadeData* sr);
	Geometry * object;
	Material * material;
	Vector3d  samplePoint;
	Vector3d  normal;
	Vector3d  wi;
};



