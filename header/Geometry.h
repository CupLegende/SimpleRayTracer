#pragma once
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

struct Ray {
	Vector3d origin;
	Vector3d direction;
};


struct Light {
	Ray r;
	Vector3d diffuse;
	Vector3d spec;
};


struct ShadeData {
	Vector3d material;
	Vector3d normal;
	Vector3d hitPoint;
	float t = INT_MAX;
	float ambientCoeff = 0.05;
	float diffuseCoeff = 0.1;
	float specCoeff = 0.1;
	float specAlpha = 0.5;
};


class Geometry {
public:
	double maxCoord [3] = { DBL_MAX ,DBL_MAX,DBL_MAX };
	double minCoord[3] = { DBL_MIN , DBL_MIN ,DBL_MIN };
	Vector3d material;
	float ambientCoeff = 0.9;
	float diffuseCoeff = 1.2;
	float specCoeff = 0.2;
	float specAlpha = 4.5;
	Geometry(){}
	virtual bool hit(Ray &ray, ShadeData *data){
		return false;
	}
	
};

class Plane : public Geometry{
public:
	Vector3d point;
	Vector3d normal;
	Vector3d material;
	//Vector3d is pointer!
	Plane(Vector3d, Vector3d, Vector3d = Vector3d(1.0, 1.0, 1.0));

	bool hit(Ray &ray, ShadeData *data);
};

class Sphere : public Geometry {
public:
	Vector3d center;
	int radius;
	Vector3d material;

	Sphere(Vector3d, int, Vector3d = Vector3d(1.0, 1.0, 1.0));

	bool hit(Ray &ray, ShadeData *data);
};

class Triangle : public Geometry {
public:
	Vector3d a;
	Vector3d b;
	Vector3d c;
	Vector3d material;
	Vector3d normal;
	
	Triangle(Vector3d, Vector3d, Vector3d, Vector3d = Vector3d(1.0, 1.0, 1.0));

	bool hit(Ray &ray, ShadeData *data);
};