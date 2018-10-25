#pragma once
#include <Eigen/Dense>
#include <math.h>
#include "Geometry.h"

using namespace Eigen;
using namespace std;

Plane::Plane(Vector3d p, Vector3d n, Vector3d m) {
	point = p;
	normal = n / n.norm();
	material = m;
	if (n[0] == 0 && n[1] == 0) {
		maxCoord[2] = p[2];
		minCoord[2] = p[2];
	}
	if (n[2] == 0 && n[1] == 0) {
		maxCoord[0] = p[0];
		minCoord[0] = p[0];
	}
	if (n[0] == 0 && n[2] == 0) {
		maxCoord[1] = p[1];
		minCoord[1] = p[1];
	}
}

bool Plane::hit(Ray &r, ShadeData* sd) {
	double temp = normal.dot(r.direction);
	if (temp == 0.0) {
		return false;
	}
	double t = ((point - r.origin).dot(normal)) / temp;
	if (t >= 0) {
		if (t < sd->t) {
			sd->t = t;
			sd->normal = normal;
			sd->hitPoint = r.origin + t * r.direction;
			sd->ambientCoeff = ambientCoeff;
			sd->diffuseCoeff = diffuseCoeff;
			sd->specCoeff = specCoeff;
			sd->specAlpha = specAlpha;
			sd->material = material;
			return true;
		}
	}
	else {
		return false;
	}
}



Sphere::Sphere(Vector3d c, int r, Vector3d m) {
	center = c;
	radius = r;

	material = m;
	maxCoord[0] = c[0] + radius;
	minCoord[0] = c[0] - radius;
	maxCoord[1] = c[1] + radius;
	minCoord[1] = c[1] - radius;
	maxCoord[2] = c[2] + radius;
	minCoord[2] = c[2] - radius;
}

bool Sphere::hit(Ray &r, ShadeData * sd) {
	
	Vector3d temp = r.origin - center;
	float a = r.direction.dot(r.direction);
	float b = 2 * r.direction.dot(temp);
	float c = temp.dot(temp) - radius * radius;
	float delta = b * b - 4 * a*c;

	if(delta < 0)
		return false;
	else {
		float t1 = (sqrt(delta) - b) / (2 * a);
		float t2 = (-sqrt(delta) - b) / (2 * a);
		if (t1 > 0 || t2 > 0) {
			float tfin;
			if (t1 < 0) {
				tfin = t2;
			}
			else if (t2 < 0) {
				tfin = t1;
			}
			else {
				tfin = min(t1, t2);
			}
			if (tfin < sd->t) {
				sd->t = tfin;
				Vector3d temp2 = r.origin - center;
				sd->normal = temp2 / temp2.norm();
				sd->hitPoint = r.origin + tfin * r.direction;
				sd->ambientCoeff = ambientCoeff;
				sd->diffuseCoeff = diffuseCoeff;
				sd->material = material;
				sd->specCoeff = specCoeff;
				sd->specAlpha = specAlpha;
				return true;
			}
		}
		return false;
	}
}

Triangle::Triangle(Vector3d at, Vector3d bt, Vector3d ct, Vector3d m) {
	a = at;
	b = bt;
	c = ct;
	material = m;
	normal = (b - a).cross((c - a));
	normal = normal / normal.norm();
	maxCoord[0] = max(max(at[0], bt[0]), ct[0]);
	minCoord[0] = min(min(at[0], bt[0]), ct[0]);
	maxCoord[1] = max(max(at[1], bt[1]), ct[1]);
	minCoord[1] = min(min(at[1], bt[1]), ct[1]);
	maxCoord[2] = max(max(at[2], bt[2]), ct[2]);
	minCoord[2] = min(min(at[2], bt[2]), ct[2]);
}

bool Triangle::hit(Ray & r, ShadeData * sd) {
	Vector3d edge1 = this->b - this->a;
	Vector3d edge2 = this->c - this->a;
	Vector3d h = (r.direction).cross(edge2);
	float a = edge1.dot(h);
	if (a > -0.0000005 && a < 0.00000005) {
		return false;
	}
	float f = 1.0 / a;
	Vector3d s = r.origin - this->a;
	float u = f * (s.dot(h));
	if (u<0.0 || u>1.0) {
		return false;
	}
	Vector3d q = s.cross(edge1);
	float v = f * r.direction.dot(q);
	if (v<0.0 || u + v >1.0) {
		return false;
	}
	float t = f * edge2.dot(q);
	if (t > 0.00000005) {
		sd->t = t;
		sd->normal = normal / normal.norm();
		sd->hitPoint = r.origin + t * r.direction;
		sd->ambientCoeff = ambientCoeff;
		sd->diffuseCoeff = diffuseCoeff;
		sd->specCoeff = specCoeff;
		sd->specAlpha = specAlpha;
		sd->material = material;
		return true;
	}
	return false;
		/* 
	float temp = normal.dot(r.direction);
	if (temp == 0) {
		return false;
	}
	float t = (a - r.origin).dot(normal) / temp;
	if (t < 0) {
		return false;
	}
	else if(t<sd->t){
		Vector3d p = r.origin + t * r.direction;
		double n2 = normal.dot(normal);
		double bar1 = normal.dot((b - a).cross(p - a)) / n2;
		double bar2 = normal.dot((c - b).cross(p - b)) / n2;
		double bar3 = 1 - bar1 - bar2;
		if (bar1 > 0 || bar2 > 0 || bar3 > 0) {
			sd->t = t;
			sd->normal = normal / normal.norm();
			sd->hitPoint = r.origin + t * r.direction;
			sd->ambientCoeff = ambientCoeff;
			sd->diffuseCoeff = diffuseCoeff;
			sd->specCoeff = specCoeff;
			sd->specAlpha = specAlpha;
			sd->material = material;
			return true;
		}
		return false;
	}
	*/
}



