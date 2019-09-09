#pragma once
#include <Eigen/Dense>
#include "RandomSample.h"
#include "Material.h"
using namespace Eigen;
using namespace std;


/* 
struct Light {
	Ray r;
	Vector3d diffuse;
	Vector3d spec;
};
*/


class Geometry {
public:
	double maxCoord [3] = { DBL_MAX ,DBL_MAX,DBL_MAX };
	double minCoord[3] = { DBL_MIN , DBL_MIN ,DBL_MIN };
	Material* material;
	bool castShadow = true;
	Geometry(){}
	virtual Vector3d getNormal(Vector3d point) {
		return Vector3d(1.0,1.0,1.0);
	}
	virtual bool hit(Ray &ray, ShadeData *data){
		return false;
	}
	virtual Vector3d sample() {
		return Vector3d(0.0, 0.0, 0.0);
	}
	virtual float pdf(ShadeData* data) {
		return 0.0;
	}
};

class Plane : public Geometry{
public:
	Vector3d point;
	Vector3d normal;
	Material* material;
	//Vector3d is pointer!
	Plane(Vector3d, Vector3d, Material* mat);
	Vector3d getNormal(Vector3d point);
	bool hit(Ray &ray, ShadeData *data);
};

class Sphere : public Geometry {
public:
	Vector3d center;
	int radius;
	Material* material;
	Vector3d getNormal(Vector3d point);
	Sphere(Vector3d, int, Material* mat);

	bool hit(Ray &ray, ShadeData *data);
};



class Cube : public Geometry {
public:
	Vector3d center;
	int length;
	//float extremeVal[6];
	Material* material;
	Vector3d getNormal(Vector3d point);
	Cube(Vector3d, int, Material* mat);

	bool hit(Ray &ray, ShadeData *data);
};
/*
TBD
class Union : public Geometry {
public:


};
*/
/*
TBD
class Difference : public Geometry {
public:

Plane(Vector3d, Vector3d, Material* mat);
	Vector3d getNormal(Vector3d point);
	bool hit(Ray &ray, ShadeData *data);
};
*/

class Intersection : public Geometry {
public:
	//simplest case
	Geometry* geo1;
	Geometry* geo2;
	Intersection(Geometry*, Geometry*);
	Vector3d getNormal(Vector3d point);
	bool hit(Ray &ray, ShadeData *data);

};

class Triangle : public Geometry {
public:
	Vector3d a;
	Vector3d b;
	Vector3d c;
	Material* material;
	Vector3d normal;
	Vector3d getNormal(Vector3d point);
	Triangle(Vector3d, Vector3d, Vector3d, Material* mat);

	bool hit(Ray &ray, ShadeData *data);
};

class Rectangle : public Geometry {
public:
	Sampler * sampler;
	Vector3d point;
	float area;
	float inverseArea;
	Vector3d h;
	Vector3d w;
	Material* material;
	Vector3d normal;
	float wlenS;
	float hlenS;
	Vector3d getNormal(Vector3d point);
	Rectangle(Vector3d p, Vector3d w, Vector3d h, Vector3d n, Material* mat );
	Vector3d sample();
	bool hit(Ray &ray, ShadeData *data);
	float pdf(ShadeData* data);
};