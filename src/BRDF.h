#pragma once

#include "RandomSample.h"
#include <Eigen/Dense>
#include "ShadeData.h"

using namespace Eigen;
class BRDF
{
public:
	BRDF();
	~BRDF();
	Sampler* sampler;
	void setSampler(Sampler * sPtr);
	virtual Vector3d rho(const ShadeData*, const Vector3d&);
	virtual Vector3d
		f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi);

	virtual Vector3d
		sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi);

	virtual Vector3d
		sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf);

};

