#pragma once
#include "BRDF.h"
#include "RandomSample.h"
#include "ShadeData.h"
#include "Texture.h"
class Lambertian :
	public BRDF
{

public:
	virtual Vector3d rho(const ShadeData*, const Vector3d&);
	virtual Vector3d
		f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi);
	Lambertian(float k, Texture* c);
	
	virtual Vector3d
		sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf);
	float kd;
	Texture* cd;
	Lambertian();
	~Lambertian();
};

class Glossy:
	public BRDF
{

public:
	virtual Vector3d rho(const ShadeData*, const Vector3d&);
	virtual Vector3d
		f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi);


	virtual Vector3d
		sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf);
	float ks;
	Vector3d cs;
	float exp;
	Sampler* sampler;

	void setSampler(int sp, const float exp);
	Glossy(float k, Vector3d &c, float ex);
	Glossy();
	~Glossy();
};

class PerfectSpecular : public BRDF {
public:
	PerfectSpecular();
	~PerfectSpecular();
	PerfectSpecular(float k, Vector3d &c);
	virtual Vector3d rho(const ShadeData*, const Vector3d&);
	virtual Vector3d
		f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi);
	virtual Vector3d
		sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi);
	virtual Vector3d
		sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf);
	float kr;
	Vector3d cr;
};
