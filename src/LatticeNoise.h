#pragma once
#include <Eigen/Dense>
#include "RandomSample.h"

using namespace Eigen;
const int tableSize = 256;
const int tableMask = tableSize - 1;

#define PERM(x) permutationTable[(x)&tableMask]
#define INDEX(ix,iy,iz) PERM((ix) + PERM((iy)+PERM(iz)) )
class LatticeNoise
{
public:
	float fsMin;  						
	float fsMax;
	float fbmMin;
	float fbmMax;
	int numOctaves;
	float gain;
	float lacu;

	LatticeNoise(int numOfOCtave, float fsMin = 0, float fsMax = 1.0, float gain = 0.75, float lacu = 4);
	~LatticeNoise();
	virtual float valueNoise(const Vector3d& p) = 0;
	static const unsigned char permutationTable[tableSize];
	float valueTable[tableSize];
	float valueFractalSum(const Vector3d& p);
	float valueFBm(const Vector3d& p);
	
};

class LinearNoise :public LatticeNoise {
public:
	LinearNoise(int numOc, float min, float max);
	~LinearNoise();
	float valueNoise(const Vector3d& p);
	
};

template<class T> T
lerp(const float f, const T& a, const T& b) {
	return (a + f * (b - a));
}

template<class T> T
four_knot_spline(const float x, const T knots[]) {
	T c3 = -0.5 * knots[0] + 1.5 * knots[1] - 1.5 * knots[2] + 0.5 * knots[3];
	T c2 = knots[0] - 2.5 * knots[1] + 2.0 * knots[2] - 0.5 * knots[3];
	T c1 = 0.5 * (-knots[0] + knots[2]);
	T c0 = knots[1];

	return (T((c3*x + c2)*x + c1)*x + c0);
}

