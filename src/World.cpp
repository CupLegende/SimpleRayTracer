#include <Eigen/Dense>
#include <list>
#include "Geometry.h"
#include "GUI.h"
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

World::World(ViewPlane * v, Vector3d &Ambient){
	vp = v;
	shader = (Shader(Ambient));
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
	int s = vp->pixelSize;
	if (multiJitter == false) {
		for (int r = 0; r < vres; r++) {
			for (int c = 0; c < hres; c++) {
				ra.origin = Vector3d(s*(c - hres / 2 + 0.5), s*(r - vres / 2 + 0.5), zCoord);
				Vector3d pixel_color = hit_objects(ra);
				display(r, c, pixel_color);
			}
		}
	}
	else {
		Sampler rs = Sampler();
		ArrayXf line = ArrayXf::LinSpaced(rs.numEachRow+2, 0.0, 1.0);
		for (int r = 0; r < vres; r++) {
			for (int c = 0; c < hres; c++) {
				Vector3d resultColor = Vector3d(0.0, 0.0, 0.0);
				for (int rr = 1; rr < rs.numEachRow + 1; rr++) {
					for (int cc = 1; cc < rs.numEachRow + 1; cc++) {
						double* temp = rs.getSample();
						ra.origin = Vector3d(s*(c - hres / 2 + line[cc] + temp[0]), s*(r - vres / 2 + line[rr] + temp[1]), zCoord);
						Vector3d pixel_color = hit_objects(ra);
						resultColor += pixel_color;
					}

				}
				display(r, c, resultColor / rs.numOfSample);
			}
		}
	}

}

void World::perspectiveRender(Vector3d eyePos, Vector3d up, Vector3d look, double dist, bool multiJitter) {
	Ray ra = Ray{ Vector3d(0.0,0.0,0.0), Vector3d(0.0,0.0,-1.0) };
	Vector3d w = eyePos - look;
	w = w / w.norm();
	if (up[1] == w[1]) {
		up = -1.0 * up;
	}
	Vector3d u = up.cross(w);
	u = u/ u.norm();
	Vector3d v = w.cross(u);
	int hres = vp->hres;
	int vres = vp->vres;
	int s = vp->pixelSize;
	if (multiJitter == false) {
		for (int r = 0; r < vres; r++) {
			for (int c = 0; c < hres; c++) {
				double xv = s * (c - hres / 2 + 0.5);
				double yv = s * (r - vres / 2 + 0.5);
				Vector3d dir = xv * u + yv * v - dist * w;
				ra.direction = dir / dir.norm();
				ra.origin = eyePos;
				Vector3d pixel_color = hit_objects(ra);
				display(r, c, pixel_color);
			}
		}
	}

	else {
		Sampler rs = Sampler();
		ArrayXf line = ArrayXf::LinSpaced(rs.numEachRow + 2, 0.0, 1.0);
		for (int r = 0; r < vres; r++) {
			for (int c = 0; c < hres; c++) {
				Vector3d resultColor = Vector3d(0.0, 0.0, 0.0);
				for (int rr = 1; rr < rs.numEachRow + 1; rr++) {
					for (int cc = 1; cc < rs.numEachRow + 1; cc++) {
						double* temp = rs.getSample();
						double xv = s * (c - hres / 2 + line[cc] + temp[0]);
						double yv = s * (r - vres / 2 + line[rr] + temp[1]);
						Vector3d dir = xv * u + yv * v - dist * w;
						ra.origin = eyePos;
						ra.direction = dir / dir.norm();
						Vector3d pixel_color = hit_objects(ra);
						resultColor += pixel_color;
					}

				}
				display(r, c, resultColor / rs.numOfSample);
			}
		}

	}
}

Vector3d World::hit_objects(Ray& r) {
	//bool hitObject = false;
	Vector3d * color = &Vector3d();
	

	//ShadeData * sd = new ShadeData;
	//int localTmin = INT_MAX;
	//bool hitObject = false;
	/* 
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
	if (sd != nullptr) {
	
		color = shader.shade(r,sd, &tree, &lights);
		//may want to do depth
	}
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
	else {
		color = &backGround;
	}

	//delete sd;
	return *color;
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
	
	cout << y << "\n";
	RenderPixel* temp = new RenderPixel{ x,y, (int)(finalColor[0] * 255), (int)(finalColor[1] * 255), (int)(finalColor[2] * 255) };
	//paintArea->addPixel(*temp);
	gr.add(temp);
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