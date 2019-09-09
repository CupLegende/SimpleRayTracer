#pragma once
#include "BRDF.h"
#include "ShadeData.h"
#include <Eigen/Dense>
using namespace Eigen;
class PerfectTransmitter:public BRDF
{
public:
	float ior;
	float kt;
	PerfectTransmitter();
	PerfectTransmitter(float i, float kt);
	~PerfectTransmitter();
	virtual bool tir(ShadeData* sd);
	virtual Vector3d rho(const ShadeData*, const Vector3d&);
	virtual Vector3d
		f( ShadeData* sr, const Vector3d& wo, const Vector3d& wi);
	virtual Vector3d
		sampleF( ShadeData* sr, const Vector3d& wo,  Vector3d& wi);

};

