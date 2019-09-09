#include "Lambertian.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Texture.h"
#define invPI 0.31830988618


Lambertian::Lambertian()
{
	kd = 0.0;
		cd = new ConstantColor(Vector3d(0.0,0.0,0.0));
}

Lambertian::Lambertian(float k, Texture* t)
{
	kd = k;
	cd = t;
}


Lambertian::~Lambertian()
{
}

Vector3d Lambertian::rho(const ShadeData* sd, const Vector3d&) {
	return kd * cd->getColor(sd);
}
Vector3d
Lambertian::f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi) {
	return (kd/M_PI) * cd->getColor(sr);
}


Vector3d
Lambertian::sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf) {
	Vector3d w = sr->normal;
	Vector3d v = Vector3d(0.0034, 1, 0.0071).cross(w);
	v = v/v.norm();
	Vector3d u = v.cross(w);

	Vector3d sp = sampler->sampleHemi();
	wi = sp[0] * u + sp[1] * v + sp[2] * w;
	wi = wi/wi.norm();

	pdf = sr->normal.dot(wi) * invPI;

	return (kd * cd->getColor(sr) * invPI);

}

Glossy::Glossy()
{
	exp = 1.0;
	ks = 0.0;
	cs = Vector3d(1.0, 1.0, 1.0);
	sampler = NULL;
}

Glossy::Glossy(float k, Vector3d& c, float ex)
{
	ks = k;
	cs = c;
	exp = ex;
	sampler = NULL;
}

void
Glossy::setSampler(int sp, const float exp) {
	sampler = new Sampler(sp);
	sampler->generateHemi(exp);
}



Glossy::~Glossy()
{
	if (sampler != NULL) {
		delete sampler;
		sampler = NULL;
}
}

Vector3d Glossy::rho(const ShadeData*, const Vector3d&) {
	return Vector3d(0.0, 0.0, 0.0);
}
Vector3d
Glossy::f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi) {
	Vector3d 	L = Vector3d(0.0,0.0,0.0);
	float 		ndotwi = sr->normal.dot(wi);
	Vector3d 	r = (-wi + 2.0 * sr->normal * ndotwi);
	float 		rdotwo = r.dot(wo);

	if (rdotwo > 0.0)
		L = ks * cs * pow(rdotwo, exp);

	return L;
}


Vector3d
Glossy::sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf) {
	float ndotwo = sr->normal.dot(wo);
	Vector3d r = -wo + 2.0* sr->normal * ndotwo;
	Vector3d w = r;
	Vector3d u = Vector3d(0.00424, 1, 0.00764).cross(w);
	u = u / u.norm();
	Vector3d v = u.cross(w);
	Vector3d sp = sampler->sampleHemi();
	wi = sp[0] * u + sp[1] * v + sp[2] * w;
	if (sr->normal.dot(wi) < 0.0) {
		wi = -sp[0] * u - sp[1] * v + sp[2] * w;

	}
	float phong_lobe = pow(r.dot(wi), exp);
	pdf = phong_lobe * (sr->normal.dot(wi));
	return (ks * cs * phong_lobe);

}










PerfectSpecular::PerfectSpecular():BRDF()
{
	kr = 0.0;
	cr = Vector3d(1.0, 1.0, 1.0);
	sampler = NULL;
}

PerfectSpecular::PerfectSpecular(float k, Vector3d &c) {
	kr = k;
	cr = c;
	sampler = NULL;

}

PerfectSpecular::~PerfectSpecular()
{
}

Vector3d PerfectSpecular::rho(const ShadeData*, const Vector3d&) {
	return Vector3d(0.0, 0.0, 0.0);
}
Vector3d
PerfectSpecular::f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi) {
	return Vector3d(0.0, 0.0, 0.0);
}


Vector3d
PerfectSpecular::sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf) {
	float ndotwo = sr->normal.dot(wo);
	wi = -wo + 2.0 * sr->normal * ndotwo;
	pdf = fabs(sr->normal.dot(wi));
	return (kr * cr / (sr->normal.dot(wi)));

}

Vector3d
PerfectSpecular::sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi) {
	float ndotwo = sr->normal.dot(wo);
	wi = -wo + 2.0 * sr->normal * ndotwo;
	return (kr * cr / (sr->normal.dot(wi)));
}