#include "Texture.h"
#include <Eigen/Dense>
#include "ShadeData.h"
#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace Eigen;
Texture::Texture()
{

}

/*Vector3d Texture::getColor(ShadeData* sd)
{
	return Vector3d(0.0, 0.0, 0.0);
}
*/
Texture::~Texture()
{
}

ConstantColor::ConstantColor(Vector3d c)
{
	color = c;
}



Vector3d ConstantColor::getColor(const ShadeData* sd)
{
	return color;
}

Checker3D::Checker3D(float siz, Vector3d c1, Vector3d c2) {
	size = siz;
	color1 = c1;
	color2 = c2;
}

Vector3d Checker3D::getColor(const ShadeData * sd) {
	float eps = -0.000187453738;
	float x = sd->hitPoint[0] + eps;
	float y = sd->hitPoint[1] + eps;
	float z = sd->hitPoint[2] + eps;

	if (((int)floor(x / size) + (int)floor(y / size) + (int)floor(z / size)) % 2 == 0) {
		return color1;
	}
	else {
		return color2;
	}
}
PlaneChecker::PlaneChecker(float size, float wid, Vector3d cl, Vector3d c1, Vector3d c2): Checker3D(size, c1, c2) {
	outlineWidth = wid;
	outlineColor = cl;
}

Vector3d PlaneChecker::getColor(const ShadeData * sd) {
	float x = sd->hitPoint[0];
	float z = sd->hitPoint[2];
	int ix = floor(x / size);
	int iz = floor(z / size);
	float fx = x / size - ix;
	float fz = z / size - iz;
	float width = 0.5 * outlineWidth / size;
	bool inOutline = (fx < width || fx > 1.0 - width) || (fz < width || fz > 1.0 - width);
	if ((ix + iz) % 2 == 0) {
		if (!inOutline)
			return color1;
	}
	else {
		if (!inOutline)
			return color2;
	}

	return outlineColor;

}

FSTexture::FSTexture(Vector3d col, LatticeNoise* noi, float mi, float ma) {
	min = mi;
	max = ma;
	color = col;
	noise = noi;
};

Vector3d FSTexture::getColor(const ShadeData* sd) {
	float value = noise->valueFBm(sd->hitPoint);
	//cout << value << "\n";
	value = (min + (max - min) * value);
	//cout << value << "\n";
	return value * color;
};

