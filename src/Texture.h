#pragma once
#include <Eigen/Dense>
#include "ShadeData.h"
#include "LatticeNoise.h"
//#include <CImg.h>

using namespace Eigen;
//using namespace cimg_library;
class Texture
{
public:
	Texture();
	~Texture();
	virtual Vector3d getColor(const ShadeData* sd)=0;
};

class ConstantColor : public Texture {
public:
	Vector3d color;
	ConstantColor(Vector3d cl);
	Vector3d getColor(const ShadeData* sd);
};

/*TBD
class ImageTexture : public Texture {
public:
	int hres;
	int vres;
	CImg<unsigned char>* img;
	Mapping * mapping;
	Vector3d getColor(ShadeData* sd);
};
*/

class Checker3D : public Texture {
public:
	float size;
	Vector3d color1;
	Vector3d color2;
	Checker3D(float size, Vector3d=Vector3d(0.0,0.0,0.0), Vector3d = Vector3d(1.0, 1.0, 1.0));
	Vector3d getColor(const ShadeData* sd);
};

class FSTexture : public Texture {
public:

	
	Vector3d color;
	LatticeNoise* noise;
	float min, max;
	FSTexture( Vector3d color, LatticeNoise* noise, float = 0, float = 1.0);
	Vector3d getColor(const ShadeData* sd);
};

class PlaneChecker : public Checker3D {
public:
	Vector3d outlineColor;
	float outlineWidth;
	PlaneChecker(float size, float width = 0.0, Vector3d = Vector3d(0.5, 0.5, 0.5) , Vector3d = Vector3d(0.0, 0.0, 0.0), Vector3d = Vector3d(1.0, 1.0, 1.0));
	Vector3d getColor(const ShadeData* sd);
};
/* 
class RampFBmTexture: public Texture{
public:
	Vector3d color;


}
*/