#include "BRDF.h"
#include "RandomSample.h"
#include <Eigen/Dense>


using namespace Eigen;

BRDF::BRDF()
{
}

Vector3d BRDF::rho(const ShadeData*, const Vector3d&) {
	return Vector3d(0.0, 0.0, 0.0);
}
Vector3d
BRDF::f(const ShadeData* sr, const Vector3d& wo, const Vector3d& wi){
	return Vector3d(0.0, 0.0, 0.0);
}

Vector3d
BRDF::sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi){
	return Vector3d(0.0, 0.0, 0.0);
}

Vector3d
BRDF::sampleF(const ShadeData* sr, const Vector3d& wo, Vector3d& wi, float& pdf){
	return Vector3d(0.0, 0.0, 0.0);

}


void
BRDF::setSampler(Sampler* sPtr) {
	sampler = sPtr;
	sampler->generateHemi(1); 
}

BRDF::~BRDF()
{
	if (sampler) {
		delete sampler;
		sampler = NULL;
	}
}
