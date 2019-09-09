#include <Eigen/Dense>
#include <list>
#include "Geometry.h"
#include <list>
#include "World.h"
#include "Geometry.h"
#include <Eigen/Dense>
#include "RandomSample.h"
#include <cmath>
#include "KDtree.h"
#include "Shader.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ShadeData.h"
#include "Tracer.h"
#include <omp.h>

using namespace Eigen;
using namespace std;

void World::addObjects(Geometry* geo) {
	objects.push_back(geo);
};

void World::deleteObject(int index) {
	objects.erase(objects.begin()+index);
};

World::~World() {
	for (auto & i : objects) {
		delete i;
	}
}

World::World(ViewPlane * v, Ambient* Ambient){
	vp = v;
	ambient = Ambient;
	//shader = (Shader(Ambient));
	tracer = new Tracer();
	tracer->world = this;
	//paintArea = 
}

void World::addLights(Light * l) {
	lights.push_back(l);
}

void World::deleteLights(int index) {
	lights.erase(lights.begin() + index);
}

void World::orthographicRender(double zCoord, bool multiJitter) {
	Ray ra = Ray{ Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, -1.0) };
	int hres = vp->hres;
	int vres = vp->vres;
	float s = vp->pixelSize;
	if (multiJitter == false) {

		for (int r = 0; r < vres; r++) {
			cout << r << "\n";
			for (int c = 0; c < hres; c++) {
				ra.origin = Vector3d(s*(c - hres / 2 + 0.5), s*(r - vres / 2 + 0.5), zCoord);
				Vector3d pixel_color;
				if(traceArea == true)
					pixel_color = tracer->traceRayArea(ra);
				else
					pixel_color = tracer->traceRay(ra);
				display(r, c, pixel_color);
			}
		}
	}
	else {
		Sampler rs = Sampler();
		Vector2d temp = rs.sampleSquare();
		for (int r = 0; r < vres; r++) {
			for (int c = 0; c < hres; c++) {
				Vector3d resultColor = Vector3d(0.0, 0.0, 0.0);
				for (int si = 0; si < rs.num_samples; si++) {
					Vector2d temp = rs.sampleSquare();
					ra.origin = Vector3d(s*(c - hres / 2 + temp[0]), s*(r - vres / 2  + temp[1]), zCoord);
					Vector3d pixel_color;
					if (traceArea == true)
						pixel_color = tracer->traceRayArea(ra);
					else
						pixel_color = tracer->traceRay(ra);
					resultColor += pixel_color;
				}
				display(r, c, resultColor / rs.num_samples);
			}
		}
	}

}

void World::perspectiveRender(Vector3d eyePos, Vector3d up, Vector3d look, double dist, bool multiJitter) {
	
	Vector3d w = (eyePos - look);
	w = w / w.norm();
	if (up[1] == w[1]) {
		up = -1.0 * up;
	}
	Vector3d u = up.cross(w);
	u = u/ u.norm();
	Vector3d v = w.cross(u);
	int hres = vp->hres;
	int vres = vp->vres;
	float s = vp->pixelSize;
	if (multiJitter == false) {
		int r, c;
		
		//for (r = vres-1; r >= 0; r--) {
		for (r = 0; r < vres; r++) {
			cout << r << "\n";
			//cout << r << "\n";
			//#pragma omp parallel for private(c) schedule(dynamic)
			for (c = 0; c < hres; c++) {
				double xv = s * (c - hres / 2 + 0.5);
				double yv = s * (r - vres / 2 + 0.5);
				Vector3d dir = xv * u + yv * v - dist * w;
				Ray ra = Ray{ Vector3d(0.0,0.0,0.0), Vector3d(0.0,0.0,1.0) };
				ra.direction = dir / dir.norm();
				ra.origin = eyePos;

				Vector3d pixel_color;
				if (traceArea == true)
					pixel_color = tracer->traceRayArea(ra, 0);
				else
					pixel_color = tracer->traceRay(ra, 0);

				display(r, c, pixel_color);
			}
		}
	}

	else {
		Sampler rs = Sampler();

		for (int r = 0; r < vres; r++) {
			for (int c = 0; c < hres; c++) {
				Vector3d resultColor = Vector3d(0.0, 0.0, 0.0);

				for (int si = 0; si < rs.num_samples; si++) {
					Vector2d temp = rs.sampleSquare();
					double xv = s * (c - hres / 2 + temp[0]);
					double yv = s * (r - vres / 2 + temp[1]);
					Vector3d dir = xv * u + yv * v - dist * w;
					Ray ra = Ray{ Vector3d(0.0,0.0,0.0), Vector3d(0.0,0.0,-1.0) };
					ra.origin = eyePos;
					ra.direction = dir / dir.norm();

					Vector3d pixel_color;
					if (traceArea == true)
						pixel_color = tracer->traceRayArea(ra);
					else
						pixel_color = tracer->traceRay(ra);

					resultColor += pixel_color;
				}
				
				display(r, c, resultColor / rs.num_samples);
			}
		}

	}
}


bool World::hit_objectsShallow(Ray& r, float d) {
	/*
	 	ShadeData * sd = 
		new ShadeData;
	int localTmin = d;
	bool hitObject = false;

	for (auto & i : objects) {
		bool flag = i->hit(r, sd);
		//cout << i->castShadow << "\n";
		if (flag && sd->t < localTmin && i->castShadow) {
			hitObject = true;
			localTmin = sd->t;
		}
	}
	//cout  << "\n";
	if (hitObject) {
		
			return true;
		
		
	}
	else {
		return false;
	}
	*/
	
	//r.origin = r.origin + 0.0000001 * r.direction;
	//cout << "fk1" << "\n";
	ShadeData*sdT = tree.traverseShallow(r, d);
	//Maybe buggy to do this
	//cout << d << "\n";
	//cout << sdT->t << "\n\n";
	bool cs = sdT->castShadow && sdT->hitObject;
	delete sdT;
	return cs;
	//cout << "fk2" << "\n";



	/* 
	bool cash = true;
	
	bool newTemp= tree.traverseShallow(r, d, cash);
	//cout << cash << "\n";
	return newTemp && cash;
	*/
	
}

ShadeData* World::hit_objects(Ray& r) {
	
	Vector3d * color = &Vector3d();
	
	/*
	ShadeData * sd = new ShadeData;
	int localTmin = INT_MAX;
	bool hitObject = false;
	 
	for (auto & i : objects) {
		bool flag = i->hit(r, sd);
		if (flag && sd->t < localTmin) {
			hitObject = true;
			localTmin = sd->t;
		}
	}
	if (hitObject) {
		
	}
	else {
		sd = nullptr;
	}
	*/

	ShadeData * sd = tree.traverse(r);
	if (sd != nullptr)
	{
		sd->ray = r;
		sd->w = this;
	}
	//if (sd != nullptr) {
	
		//color = shader.shade(r,sd, &tree, &lights);
		//may want to do depth
	//}
	/*
	for (auto & i: objects) {
		bool flag = i.hit(r, sd);
		if (flag && sd.t < tmin) {
			hitObject = true;
			tmin = sd.t;
			material = i.material;
			Vector3d hitPoint = r.origin + sd.t*r.direction;
			normal = sd.normal;
		}
	}
	*/
	//else {
	//	color = &backGround;
	//}

	//delete sd;
	//return *color;
	return sd;
};

void World::display(int row, int col, Vector3d color) {
	Vector3d finalColor;
	if (vp->shou_out_of_gamus) {
		finalColor = clamp_to_color(color);
	}
	else {
		finalColor = max_to_one(color);
	}
	if (vp->gamma != 1.0) {
		finalColor[0] = std::pow(finalColor[0], vp->inv_gamma);
		finalColor[1] = std::pow(finalColor[1], vp->inv_gamma);
		finalColor[2] = std::pow(finalColor[2], vp->inv_gamma);
			//Eigen::pow(finalColor, vp->inv_gamma);

	}
	int x = col;
	int y = vp->vres - row - 1;
	
	
	RenderPixel temp = RenderPixel{ x,y, (int)(finalColor[0] * 255), (int)(finalColor[1] * 255), (int)(finalColor[2] * 255) };
	//paintArea->addPixel(*temp);
	gr.add(&temp);
}

void World::show() {
	gr.draw();
	//app = new wxApp();
	//paintArea->Show(true);
	//app->MainLoop();
}

Vector3d World::clamp_to_color(Vector3d c) {
	/*double maxVal = c.maxCoeff();
	if (maxVal > 1.0) {
		c = Vector3d(1.0, 0.0, 0.0);
	}*/
	c[0] = min(1.0, c[0]);
	c[1] = min(1.0, c[1]);
	c[2] = min(1.0, c[2]);
	return c;

};

Vector3d World::max_to_one(Vector3d c) {
	double maxVal = c.maxCoeff();
	if (maxVal > 1.0) {
		return c / maxVal;
	}
	return c;
}

void World::buildTree() {
	tree = KDtree(objects);
}