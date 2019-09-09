#include <Eigen/Dense>
#include "Material.h"
//#include "Geometry.h"
//#include "Lambertian.h"
//#include "ShadeData.h"
#include "World.h"
#include <iostream>
#define SHADOW_EPSILON 0.005

	Emissive::Emissive(Vector3d c, float l)
	{
		ce = c;
		ls = l;

	}

	Emissive::~Emissive()
	{

	}

	Vector3d Emissive::getLe(ShadeData* sr) {
		return ce * ls;
	}

	Vector3d Emissive::shade(ShadeData* sr)
	{
		Vector3d normal = -sr->normal;
		if (normal.dot(sr->ray.direction) > 0.0) {
			return ce ;
		}
		else {
			return sr->w->backGround;
		}
	}

	Vector3d Emissive::areaLightShade(ShadeData* sr)
	{
		Vector3d normal = -sr->normal;
		if (normal.dot(sr->ray.direction) > 0.0) {
			return ce;
		}
		else {
			return sr->w->backGround;
		}
	}


	Matte::Matte(void)
		: Material(),
		ambient_brdf(new Lambertian),
		diffuse_brdf(new Lambertian)
	{}

	Matte::~Matte(void) {

		if (ambient_brdf) {
			delete ambient_brdf;
			ambient_brdf = NULL;
		}

		if (diffuse_brdf) {
			delete diffuse_brdf;
			diffuse_brdf = NULL;
		}
	}

	Vector3d
		Matte::shade(ShadeData* sr) {
		Vector3d 	wo = -sr->ray.direction;
		Vector3d 	out = ambient_brdf->rho(sr, wo).cwiseProduct(sr->w->ambient->L(sr));
		int 		num_lights = sr->w->lights.size();

		for (int j = 0; j < num_lights; j++) {
			Vector3d wi = sr->w->lights[j]->get_direction(sr);
			float ndotwi = sr->normal.dot(wi);

			if (ndotwi > 0.0)
			{
			//	L += diffuse_brdf->f(sr, wo, wi) * sr->w->lights[j]->L(sr) * ndotwi;

				Ray shadowRay = { sr->hitPoint + (SHADOW_EPSILON * wi), wi };
				bool bInShadow = sr->w->lights[j]->inShadow(shadowRay, sr);

				if (!bInShadow)
					out += diffuse_brdf->f(sr, wo, wi).cwiseProduct(sr->w->lights[j]->L(sr)) * ndotwi;
			}
		}

		return out;
	}

	Vector3d Matte::areaLightShade(ShadeData* sr)
	{
		Vector3d wo = -sr->ray.direction;

		// Add ambient light
		Vector3d out = ambient_brdf->rho(sr, wo).cwiseProduct(sr->w->ambient->L(sr));

		int num_lights = sr->w->lights.size();

		for (int j = 0; j < num_lights; j++) {
			Vector3d wi = sr->w->lights[j]->get_direction(sr);
			float ndotwi = sr->normal.dot(wi);

			if (ndotwi > 0.0)
			{
				//	L += diffuse_brdf->f(sr, wo, wi) * sr->w->lights[j]->L(sr) * ndotwi;

				Ray shadowRay = { sr->hitPoint + (SHADOW_EPSILON * wi), wi };
				bool bInShadow = sr->w->lights[j]->inShadow(shadowRay, sr);

				if (!bInShadow)
					out += diffuse_brdf->f(sr, wo, wi).cwiseProduct(sr->w->lights[j]->L(sr)) * sr->w->lights[j]->G(sr) * ndotwi/ sr->w->lights[j]->pdf(sr);
			}
		}

		return out;
	}

	Phong::Phong() {


	}

	Phong::Phong(float ka, float kd, Texture* cd, float ks, Vector3d& cs, float exp) {
		ambient_brdf = new Lambertian(ka, cd);
		diffuse_brdf = new Lambertian(kd, cd);
		specular_brdf = new Glossy(ks, cs, exp);
	}

	Phong::~Phong() {


	}

	Vector3d
		Phong::shade(ShadeData* sr) {
		Vector3d 	wo = -sr->ray.direction;
		Vector3d 	out = ambient_brdf->rho(sr, wo).cwiseProduct(sr->w->ambient->L(sr));
		int 		num_lights = sr->w->lights.size();

		for (int j = 0; j < num_lights; j++) {
			Vector3d wi = sr->w->lights[j]->get_direction(sr);
			float ndotwi = sr->normal.dot(wi);

			if (ndotwi > 0.0)
			{
				//	L += diffuse_brdf->f(sr, wo, wi) * sr->w->lights[j]->L(sr) * ndotwi;

				Ray shadowRay = { sr->hitPoint + (SHADOW_EPSILON * wi), wi };
				bool bInShadow = sr->w->lights[j]->inShadow(shadowRay, sr);

				if (!bInShadow) {
					
					Vector3d tempC = diffuse_brdf->f(sr, wo, wi);
					Vector3d second = specular_brdf->f(sr, wo, wi);
					out += (tempC + second).cwiseProduct(sr->w->lights[j]->L(sr)) * ndotwi;
				}
			}
		}

		return out;
	}

	Vector3d Phong::areaLightShade(ShadeData* sr)
	{
		int mont = 1;

		Vector3d wo = -sr->ray.direction;

		// Add ambient light
		Vector3d out = ambient_brdf->rho(sr, wo).cwiseProduct(sr->w->ambient->L(sr));

		int num_lights = sr->w->lights.size();

		for (int j = 0; j < num_lights; j++) {
			Vector3d outTemp = Vector3d(0.0, 0.0, 0.0);
			for (int z = 0; z < mont; z++) {
				
				Vector3d wi = sr->w->lights[j]->get_direction(sr);
				float ndotwi = sr->normal.dot(wi);
				//cout << wi << "\n";
				//cout << sr->normal << "\n";
				//cout << "\n";
				if (ndotwi > 0.0)
				{
					//cout << "hi2" << "\n";
					//	L += diffuse_brdf->f(sr, wo, wi) * sr->w->lights[j]->L(sr) * ndotwi;

					Ray shadowRay = { sr->hitPoint + (SHADOW_EPSILON * wi), wi };
					bool bInShadow = sr->w->lights[j]->inShadow(shadowRay, sr);
					//cout << shadowRay.direction << "\n";
					//cout << "\n";
					if (!bInShadow) {
						
						Vector3d tempC = diffuse_brdf->f(sr, wo, wi) + specular_brdf->f(sr, wo, wi);

						outTemp += tempC.cwiseProduct(sr->w->lights[j]->L(sr)) * sr->w->lights[j]->G(sr) * ndotwi / sr->w->lights[j]->pdf(sr);
					}
					//else {
						//cout << "testSD" << "\n";
					//}
				}
				

			}
			outTemp = outTemp / mont;
			out += outTemp;
		}

		return out;
	}

	GlossyReflector::GlossyReflector():Phong() {

	}

	GlossyReflector::GlossyReflector(float ka, float kd, Texture* cd, float ks, Vector3d& cs, float exp, float kr, Vector3d& cr): Phong(ka,kd,cd,ks,cs,exp) {
		glossy_reflective_brdf = new Glossy(kr, cr, exp);
	}
	GlossyReflector::~GlossyReflector() {

	}
	void
	GlossyReflector::setSampler(int num, float exp) {
		glossy_reflective_brdf->setSampler(num, exp);
	}

	Vector3d GlossyReflector::areaLightShade(ShadeData* sr)
	{
		Vector3d L = Phong::areaLightShade(sr);
		Vector3d wo = -sr->ray.direction;
		Vector3d wi = Vector3d(0.0, 0.0, 0.0);
		float pdf = 0;
		Vector3d fr = glossy_reflective_brdf->sampleF(sr, wo, wi, pdf);
		Ray reflected = Ray{ sr->hitPoint, wi };
		L += fr.cwiseProduct(sr->w->tracer->traceRay(reflected, sr->depth + 1))*(sr->normal.dot(wi))/pdf;
		return L;

	}

	Reflective::Reflective() :Phong() {

	}

	Reflective::Reflective(float ka, float kd, Texture* cd, float ks, Vector3d& cs,  float exp, float kr, Vector3d& cr): Phong(ka, kd, cd, ks, cs, exp) {
		reflective_brdf = new PerfectSpecular(kr, cr);
	}
	Reflective::~Reflective() {

	}

	Vector3d Reflective::shade(ShadeData* sr)
	{
		Vector3d L = Phong::shade(sr);
		Vector3d wo = -sr->ray.direction;
		Vector3d wi = Vector3d(0.0, 0.0, 0.0);
		Vector3d fr = reflective_brdf->sampleF(sr, wo, wi);
		//cout << wi[0] << "\n";
		Ray reflected = Ray{ sr->hitPoint + (SHADOW_EPSILON*wi), wi };
		L += fr.cwiseProduct(sr->w->tracer->traceRay(reflected, sr->depth + 1))*(sr->normal.dot(wi));
		return L;


	}



	Transparent::Transparent() :Phong() {

	}

	Transparent::Transparent(float ka, float kd, Texture * cd, float ks, Vector3d& cs, float exp, float kr, Vector3d& cr, float i, float kt) : Phong(ka, kd, cd, ks, cs, exp) {
		reflective_brdf = new PerfectSpecular(kr, cr);
		transparent_brdf = new PerfectTransmitter(i, kt);
	}
	Transparent::~Transparent() {

	}

	Vector3d Transparent::shade(ShadeData* sr)
	{
		Vector3d L = Phong::shade(sr);
		Vector3d wo = -sr->ray.direction;
		Vector3d wi = Vector3d(0.0, 0.0, 0.0);
		Vector3d fr = reflective_brdf->sampleF(sr, wo, wi);

		Ray reflected = Ray{ sr->hitPoint + (SHADOW_EPSILON*wi), wi };
		if (transparent_brdf->tir(sr))
			L += sr->w->tracer->traceRay(reflected , sr->depth + 1);
		else {
			Vector3d wt = Vector3d(0.0, 0.0, 0.0);
			Vector3d ft = transparent_brdf->sampleF(sr, wo, wt);
			Ray transmitted = Ray{ sr->hitPoint +(SHADOW_EPSILON*wt), wt };
			L += fr.cwiseProduct(sr->w->tracer->traceRay(reflected, sr->depth + 1)) * fabs(sr->normal.dot(wi));

			L += fr.cwiseProduct(sr->w->tracer->traceRay(transmitted, sr->depth + 1))*fabs(sr->normal.dot(wt));
			
		}
		return L;
	}
