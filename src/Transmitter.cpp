#include "Transmitter.h"



PerfectTransmitter::PerfectTransmitter()
{

}

PerfectTransmitter::PerfectTransmitter(float i, float k)
{
	kt = k;
	ior = i;
}

PerfectTransmitter::~PerfectTransmitter()
{
}

bool PerfectTransmitter::tir(ShadeData* sd)
{
	Vector3d wo = -sd->ray.direction;
	float cos_thetai = sd->normal.dot(wo);
	float eta = ior;
	if (cos_thetai < 0.0)
		eta = 1.0 / eta;
	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);

}

Vector3d PerfectTransmitter::rho(const ShadeData*, const Vector3d&) {
	return Vector3d(0.0, 0.0, 0.0);
}
Vector3d
PerfectTransmitter::f(ShadeData* sr, const Vector3d& wo, const Vector3d& wi) {
	return Vector3d(0.0, 0.0, 0.0);
}

Vector3d PerfectTransmitter::sampleF( ShadeData * sr, const Vector3d& wo,  Vector3d&wt) {
	Vector3d n = sr->normal;
	float cos_thetai = n.dot(wo);
	float eta = ior;
	if (cos_thetai < 0.0) {
		cos_thetai = -cos_thetai;
		n = -n;
		eta = 1.0 / eta;
	}
	float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	
	wt = -wo/ eta - (cos_theta2 - cos_thetai / eta) * n;
	return (kt / (eta*eta) * Vector3d(1.0, 1.0, 1.0) / fabs(sr->normal.dot(wt)));
}
