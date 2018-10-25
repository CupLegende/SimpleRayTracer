#pragma once
#include <Eigen/Dense>
#include <list>
#include "Geometry.h"
#include "GUI.h"
#include <list>
#include "KDtree.h"
#include "Shader.h"
#include "Graphics.h"

using namespace Eigen;
using namespace std;




struct ViewPlane {
	int hres = 800;
	int vres = 800;
	double pixelSize = 1.0;
	int num_samples = 1;
	double gamma = 1.0;
	double inv_gamma = 1.0;
	bool shou_out_of_gamus = true;
};

class World {
public:
	KDtree tree;
	ViewPlane *vp;
	//DrawPanel * paintArea;
	//MyApp app;
	Vector3d backGround; 
	vector <Geometry*> objects;
	Shader shader;
	vector<Light*>lights;
	Graphics gr;
	World(ViewPlane*, Vector3d& Am);
	~World();
	void addLights(Light* lt);

	void deleteLights(int lt);

	void addObjects(Geometry*);

	void deleteObject(int);

	void orthographicRender(double zCoord, bool multiJitter);

	void perspectiveRender(Vector3d eyePos, Vector3d up, Vector3d look, double dist, bool multiJitter);

	Vector3d hit_objects(Ray& r);

	void buildTree();

	void display(int row, int col, Vector3d color);

	void show();

	Vector3d clamp_to_color(Vector3d c);

	Vector3d max_to_one(Vector3d c);
};