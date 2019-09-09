#pragma once
#include <Eigen/Dense>
//#include "Geometry.h"
#include "Lambertian.h"
//#include "World.h"			
#include "ShadeData.h"
#include "Transmitter.h"
#include "Texture.h"

class World;
class Material {
public:
	bool castShadow = true;
	Material() {};
	~Material() {};
	virtual Vector3d shade(ShadeData*) {
		return Vector3d(0.0,0.0,0.0);
	}
	virtual Vector3d areaLightShade(ShadeData* sr) {
		return Vector3d(0.0, 0.0, 0.0);
	}
	virtual Vector3d getLe(ShadeData*) {
		return Vector3d(0.0, 0.0, 0.0);
	}
};

class Emissive : public Material
{
public:
	bool castShadow = false;
	Emissive(Vector3d c, float ls);
	~Emissive();
	float ls = 1.0;
	Vector3d ce = Vector3d(1.0, 1.0, 1.0);
	Vector3d getLe(ShadeData* sr);
	Vector3d shade(ShadeData* sr);
	Vector3d areaLightShade(ShadeData* sr);
	//Lambertian*		ambient_brdf;
	//Lambertian*		diffuse_brdf;
};

class Matte : public Material {
public:
	Matte();
	~Matte();
	Lambertian*		ambient_brdf;
	Lambertian*		diffuse_brdf;
	Vector3d shade(ShadeData* sr);
	Vector3d areaLightShade(ShadeData* sr);
};

class Phong : public Material {
public:
	Phong();
	Phong(float ka, float kd, Texture* cd, float ks, Vector3d& cs, float exp);
	~Phong();
	Lambertian*		ambient_brdf;
	Lambertian*		diffuse_brdf;
	Glossy*         specular_brdf;
	Vector3d shade(ShadeData* sr);
	Vector3d areaLightShade(ShadeData* sr);
};

class GlossyReflector : public Phong {
public:
	GlossyReflector();
	GlossyReflector(float ka, float kd, Texture* cd, float ks, Vector3d& cs, float exp, float kr, Vector3d& cr);
	~GlossyReflector();
	Glossy*         glossy_reflective_brdf;
	Vector3d areaLightShade(ShadeData* sr);
	void setSampler(int num, float exp);
};

class Reflective : public Phong {
public:
	Reflective();
	Reflective(float ka, float kd, Texture* cd, float ks, Vector3d& cs, float exp, float kr, Vector3d& cr);
	~Reflective();
	Vector3d shade(ShadeData* sr);
	//void setSampler(int num, float exp);
	PerfectSpecular* reflective_brdf;
};

class Transparent : public Phong {
public:
	Transparent();
	Transparent(float ka, float kd, Texture* cd, float ks, Vector3d& cs, float exp, float kr, Vector3d& cr, float i, float kt);
	~Transparent();
	Vector3d shade(ShadeData* sr);
	//void setSampler(int num, float exp);
	PerfectSpecular* reflective_brdf;
	PerfectTransmitter* transparent_brdf;
};



