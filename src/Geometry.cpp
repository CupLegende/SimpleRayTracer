#pragma once
#include <Eigen/Dense>
#include <math.h>
#include "Geometry.h"
#include "Material.h"
#include "RandomSample.h"
using namespace Eigen;
using namespace std;

Intersection::Intersection(Geometry* g1, Geometry* g2) {
	geo1 = g1;
	geo2 = g2;
	minCoord[0] = max(geo1->minCoord[0], geo2->minCoord[0]);
	maxCoord[0] = min(geo1->maxCoord[0], geo2->maxCoord[0]);
	minCoord[1] = max(geo1->minCoord[1], geo2->minCoord[1]);
	maxCoord[1] = min(geo1->maxCoord[1], geo2->maxCoord[1]);
	minCoord[2] = max(geo1->minCoord[2], geo2->minCoord[2]);
	maxCoord[2] = min(geo1->minCoord[2], geo2->minCoord[2]);

}

Vector3d Intersection::getNormal(Vector3d point) {
	//not really useful
	return Vector3d(1.0, 0.0, 0.0);
}

bool Intersection::hit(Ray &ray, ShadeData *data) {
	ShadeData sd1;
	ShadeData sd2;
	bool hit1 = geo1->hit(ray, &sd1);
	bool hit2 = geo2->hit(ray, &sd2);
	if (!hit1 || !hit2) {
			return false;
	}
	if (sd1.t > sd2.t && sd1.t < sd2.maxt)
	{
		*data = sd1;
		return true;

	}
	if (sd2.t > sd1.t && sd2.t < sd1.maxt)
	{
		*data = sd2;
		return true;

	}
	return false;

}



Vector3d Rectangle::sample() {
	Vector2d samp = sampler->sampleSquare();
	
	return point + w * samp[0] + h * samp[1];
}

float Rectangle::pdf(ShadeData* data) {
	return inverseArea;
}


Rectangle::Rectangle(Vector3d p, Vector3d wi, Vector3d hi, Vector3d n, Material* mate) {
	//castShadow = mate->castShadow;
	normal = n;
	material = mate;
	point = p;
	w = wi;
	h = hi;
	area = wi.norm() * hi.norm();
	inverseArea = 1 / area;
	normal = normal / normal.norm();

	maxCoord[0] = max(p[0], p[0] + wi[0] + hi[0]);
	minCoord[0] = min(p[0], wi[0] + p[0] + hi[0]);

	maxCoord[1] = max(p[1], wi[1] + p[1] + hi[1]);
	minCoord[1] = min(p[1], wi[1] + p[1] + hi[1]);

	maxCoord[2] = max(p[2], wi[2] + p[2] + hi[2]);
	minCoord[2] = min(p[2], wi[2] + p[2] + hi[2]);

	wlenS = wi.norm() * wi.norm();
	hlenS = hi.norm() * hi.norm();
}

Vector3d Rectangle::getNormal(Vector3d point) {
	return normal;
}

bool Rectangle::hit(Ray &ray, ShadeData *sd) {
	double t = (point - ray.origin).dot(normal) / (ray.direction.dot(normal));
	if (t <= 0)
		return false;
	Vector3d p = ray.origin + t * ray.direction;
	Vector3d d = p - point;
	double ddota = d.dot(w);
	if (ddota < 0.0 || ddota > wlenS)
		return false;
	double ddotb = d.dot(h);
	if (ddotb < 0.0 || ddotb > hlenS)
		return false;
	
	sd->t = t;
	sd->normal = normal / normal.norm();
	sd->hitPoint = ray.origin + t * ray.direction;
	sd->material = material;
	return true;

}

Vector3d Plane::getNormal(Vector3d point) {
	return normal;
}

Plane::Plane(Vector3d p, Vector3d n, Material* m) {
	//castShadow = m->castShadow;
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
			sd->material = material;
			return true;
		}
	}
	else {
		return false;
	}
}


Cube::Cube(Vector3d c, int r, Material* mat) {
	center = c;
	length = r;
	material = mat;
	minCoord[0] = c[0] - r;
	maxCoord[0] = c[0] + r;
	minCoord[1] = c[1] - r;
	maxCoord[1] = c[1] + r;
	minCoord[2] = c[2] - r;
	maxCoord[2] = c[2] + r;
}

Vector3d Cube::getNormal(Vector3d point) {
	Vector3d temp = (point - center).cwiseAbs();
	Vector3d tempAbs = (point - center).cwiseAbs();
	if (tempAbs[0] > tempAbs[1] && tempAbs[0] > tempAbs[2]) {
		if (temp[0] > center[0])
			return Vector3d(1.0, 0.0, 0.0);
		return Vector3d(-1.0, 0.0, 0.0);
	}
	else if(tempAbs[1] > tempAbs[0] && tempAbs[1] > tempAbs[2]){
		if (temp[1] > center[1])
			return Vector3d(0.0, 1.0, 0.0);
		return Vector3d(0.0, -1.0, 0.0);
	}
	else {
		if (temp[2] > center[2])
			return Vector3d(0.0, 0.0, 1.0);
		return Vector3d(0.0, 0.0, -1.0);
	}
}

bool Cube::hit(Ray &ray, ShadeData *sd) {
	float t1 = (minCoord[0] - ray.origin[0]) / ray.direction[0];
	float t2 = (maxCoord[0] - ray.origin[0]) / ray.direction[0];

	float t3 = (minCoord[1] - ray.origin[1]) / ray.direction[1];
	float t4 = (maxCoord[1] - ray.origin[1]) / ray.direction[1];

	float t5 = (minCoord[2] - ray.origin[2]) / ray.direction[2];
	float t6 = (maxCoord[2] - ray.origin[2]) / ray.direction[2];


	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
	//float t;
	if (tmax < 0)
	{
		//t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		//t = tmax;
		return false;
	}

	sd->t = tmin;
	sd->hitPoint = ray.origin + tmin * ray.direction;
	sd->normal = this->getNormal(sd->hitPoint);
	sd->material = material;
	sd->maxt = tmax;
	return true;
}

Vector3d Sphere::getNormal(Vector3d point) {
	Vector3d temp = point - center;
	return temp / temp.norm();
}



Sphere::Sphere(Vector3d c, int r, Material* m) {
	//castShadow = m->castShadow;
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
				sd->material = material;
				sd->maxt = max(t1, t2);
				return true;
			}
		}
		return false;
	}
}

Vector3d Triangle::getNormal(Vector3d point) {
	return normal;
}

Triangle::Triangle(Vector3d at, Vector3d bt, Vector3d ct, Material* m) {
	//castShadow = m->castShadow;
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



