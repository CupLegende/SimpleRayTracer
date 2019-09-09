#include <Eigen/dense>
#include "World.h"
//#include "GUI.h"
#include "ObjReader.h"
#include "Geometry.h"
#include <stdio.h>
#include "Graphics.h"
#include <iostream>
#include "Light.h"
#include "Material.h"
#include "RandomSample.h"
#include "Tracer.h"
#include "Texture.h"
#include "LatticeNoise.h"
#include <ctime>

using namespace Eigen;
using namespace std;
int rayCount = 0;
int main() {

	srand(time(NULL));
	ViewPlane vp;

	Ambient * am = new Ambient();
	am->color = Vector3d(0.3, 0.2, 0.5);
	am->ls = 0.5;
	World *wd = new World(&vp, am);
	Vector3d c1 = Vector3d(0.5, 0.7, 0.2);
	ConstantColor* col1 = new ConstantColor(c1);
	Vector3d c2 = Vector3d(1.0, 1.0, 1.0);
	ConstantColor* col2 = new ConstantColor(c2);

	PlaneChecker * checker = new PlaneChecker(30.0, 5.0);
	Phong * matSp1 = new Phong((float)0.9, (float)0.7, col1, (float)0.6, c2, (float)7.5);
	Sphere* temp = new Sphere(Vector3d(-200, -100, -20), 100, matSp1);

	Vector3d c3 = Vector3d(0.4, 0.8, 0.9);
	ConstantColor* col3 = new ConstantColor(c3);

	

	LinearNoise * noi = new LinearNoise(6, -0.1, 1.1);
	FSTexture* fsTemp2 = new FSTexture(c3, noi, -0.1, 1.1);

	Phong * matSp2 = new Phong((float)0.9, (float)0.7, fsTemp2, (float)0.4, c2, (float)7.5);
	//Phong * matSp2 = new Phong((float)0.9, (float)0.7, col3, (float)0.4, c2, (float)7.5);
	Sphere* temp2 = new Sphere(Vector3d(-350, -200, -40), 130, matSp2);


	Vector3d c4 = Vector3d(1.0, 0.0, 0.0);
	
	FSTexture* fs = new FSTexture(c4, noi, -0.1, 1.1);

	ConstantColor* col4 = new ConstantColor(c4);
	Phong * matSp3 = new Phong((float)0.9, (float)0.7, col4, (float)0.5, c2, (float)7.5);

	Sphere* temp31 = new Sphere(Vector3d(275, 100, -40), 160, matSp3);
	Cube* temp32 = new Cube(Vector3d(200, 0, -40), 100, matSp1);
	Intersection* temp3 = new Intersection(temp31, temp32);

	Phong * matSp4 = new Phong((float)0.9, (float)0.7, fs, (float)0.5, c2, (float)7.5);
	Sphere* temp4 = new Sphere(Vector3d(0, -100, -40), 100, matSp4);

	Emissive * matLight = new Emissive(Vector3d(1.0, 1.0, 1.0), 0.7);
	Rectangle * li = new Rectangle(Vector3d(0, 150, 100), Vector3d(50, 0, 0), Vector3d(0, 0, 50), Vector3d(0.0, -1.0, 0.0), matLight);

	li->sampler = new Sampler();
	li->castShadow = false;
	Vector3d c5 = Vector3d(0.6, 0.5, 0.5);

	Phong * matG = new Phong((float)0.9, (float)0.9, checker, (float)0.1, c2, (float)3);
	Vector3d GroundNorm = Vector3d(0.0, 1.0, 0.2);
	GroundNorm = GroundNorm / GroundNorm.norm();
	Plane* ground = new Plane(Vector3d(-150, -330, -40), GroundNorm, matG);

	AreaLight* light1 = new AreaLight();
	PointLight * light2 = new PointLight(Vector3d(1.0, 1.0, 1.0), Vector3d(0.0, 300.0, 200.0), 3.0);

	light1->setObject(li);
	light1->material = li->material;
	wd->addObjects(temp);
	wd->addObjects(temp2);
	wd->addObjects(temp3);
	wd->addObjects(temp4);
	wd->addObjects(li);
	wd->addObjects(ground);
	wd->addLights(light1);
	wd->buildTree();

	//wd->orthographicRender(400, false);
	wd->traceArea = true;
	wd->perspectiveRender(Vector3d(0, 0, 300), Vector3d(0, 1, 0), Vector3d(0, 0, 0), 200, true);
	wd->show();
}


/*
soft shadow code

	ViewPlane vp;

	Ambient * am = new Ambient();
	am->color = Vector3d(0.3, 0.2, 0.5);
	am->ls = 0.5;
	World *wd = new World(&vp, am);
	Vector3d c1 = Vector3d(0.5, 0.7, 0.2);
	Vector3d c2 = Vector3d(1.0, 1.0, 1.0);
	Phong * matSp1 = new Phong((float)0.9, (float)0.7, c1, (float)0.6, c2, (float)7.5);
	Sphere* temp = new Sphere(Vector3d(0, -100, -20), 100, matSp1);

	Vector3d c3 = Vector3d(0.4, 0.8, 0.9);
	Phong * matSp2 = new Phong((float)0.9, (float)0.7, c3, (float)0.4, c2, (float)7.5);
	Sphere* temp2 = new Sphere(Vector3d(-150, -200, -40), 130, matSp2);

	Emissive * matLight = new Emissive(Vector3d(1.0, 1.0, 1.0), 0.7);
	Rectangle * li = new Rectangle(Vector3d(0, 150, 100), Vector3d(50, 0, 0), Vector3d(0, 0, 50), Vector3d(0.0, -1.0, 0.0), matLight);

	li->sampler = new Sampler();
	li->castShadow = false;
	Vector3d c4 = Vector3d(0.6, 0.5, 0.5);
	Phong * matG = new Phong((float)0.9, (float)0.9, c4, (float)0.1, c2, (float)3);
	Vector3d GroundNorm = Vector3d(0.0, 1.0, 0.2);
	GroundNorm = GroundNorm / GroundNorm.norm();
	Plane* ground = new Plane(Vector3d(-150, -330, -40),GroundNorm, matG);

	AreaLight* light1 = new AreaLight();
	PointLight * light2 = new PointLight(Vector3d(1.0, 1.0, 1.0), Vector3d(0.0, 300.0, 200.0), 3.0);

	light1->setObject(li);
	light1->material = li->material;
	wd->addObjects(temp);
	wd->addObjects(temp2);

	wd->addObjects(li);
	wd->addObjects(ground);
	wd->addLights(light1);
	wd->buildTree();

	//wd->orthographicRender(400, false);

	wd->perspectiveRender(Vector3d(0, 0, 300), Vector3d(0, 1, 0), Vector3d(0, 0, 0), 200, false);
	wd->show();





*/

/*
reflective code

	ViewPlane vp;

	Ambient * am = new Ambient();
	am->color = Vector3d(0.3, 0.2, 0.5);
	am->ls = 0.5;
	World *wd = new World(&vp, am);
	Vector3d c1 = Vector3d(0.2, 0.5, 0.9);
	Vector3d c2 = Vector3d(1.0, 1.0, 1.0);

	Transparent * matSp1 = new Transparent((float)0.1, (float)0.1, c1, (float)0.3, c2, (float)2000, (float)0.1, c2, (float)1.5, (float)0.9);
	Sphere* temp = new Sphere(Vector3d(0, -100, 150), 100, matSp1);

	Vector3d c4= Vector3d(0.8, 0.3, 0.9);
	Phong * matSp3 = new Phong((float)0.9, (float)0.7, c4, (float)0.3, c2, (float)7.5);
	Sphere* temp3 = new Sphere(Vector3d(-150, 0, 200), 50, matSp3);


	Vector3d c3 = Vector3d(0.75, 0.75, 0.0);

	Reflective * matSp2 = new Reflective((float)0.1, (float)0.1, c3, (float)0.75, c2, (float)100, (float)0.75, c2);

	Sphere* temp2 = new Sphere(Vector3d(-150, -200, -80), 130, matSp2);



	Emissive * matLight = new Emissive(Vector3d(1.0, 1.0, 1.0), 0.7);
	Rectangle * li = new Rectangle(Vector3d(0, 150, 100), Vector3d(50, 0, 0), Vector3d(0, 0, 50), Vector3d(0.0, -1.0, 0.0), matLight);


//	Reflective * matSp2 = new Reflective((float)0.1, (float)0.1, c3, (float)0.3, c2, (float)100, (float)0.75, c2);
	Sphere* temp4 = new Sphere(Vector3d(-30, 20, 50), 80, matSp2);



	li->sampler = new Sampler();
	li->castShadow = false;
	Vector3d c5 = Vector3d(0.2, 0.9, 0.2);
	Phong * matG = new Phong((float)0.9, (float)0.9, c5, (float)0.1, c2, (float)3);

	Vector3d GroundNorm = Vector3d(0.0, 1.0, -0.4);
	GroundNorm = GroundNorm / GroundNorm.norm();
	Plane* ground = new Plane(Vector3d(-150, -350, -40),GroundNorm, matG);

	AreaLight* light1 = new AreaLight();
	PointLight * light2 = new PointLight(Vector3d(1.0, 1.0, 1.0), Vector3d(0.0, 100.0, 400.0), 3.0);

	light1->setObject(li);
	light1->material = li->material;
	wd->addObjects(temp);
	wd->addObjects(temp2);
	wd->addObjects(temp3);
	wd->addObjects(temp4);
	//wd->addObjects(li);
	wd->addObjects(ground);
	wd->addLights(light2);
	wd->buildTree();

	//wd->orthographicRender(400, false);

	wd->perspectiveRender(Vector3d(0, 0, 400), Vector3d(0, 1, 0), Vector3d(0, 0, 0), 200, false);
	wd->show();

*/

/* Transparent code


	ViewPlane vp;

	Ambient * am = new Ambient();
	am->color = Vector3d(0.3, 0.2, 0.5);
	am->ls = 0.5;
	World *wd = new World(&vp, am);
	Vector3d c1 = Vector3d(0.2, 0.5, 0.9);
	Vector3d c2 = Vector3d(1.0, 1.0, 1.0);

	Transparent * matSp1 = new Transparent((float)0.1, (float)0.1, c1, (float)0.3, c2, (float)2000, (float)0.75, c2, (float)1.0, (float)1.0);
	Sphere* temp = new Sphere(Vector3d(-50, -100, 300), 100, matSp1);

	Vector3d c4= Vector3d(0.8, 0.3, 0.9);
	Phong * matSp3 = new Phong((float)0.9, (float)0.7, c4, (float)0.3, c2, (float)7.5);
	Sphere* temp3 = new Sphere(Vector3d(-150, 0, 200), 50, matSp3);


	Vector3d c3 = Vector3d(0.95, 0.25, 0.0);

	Phong * matSp2 = new Phong((float)0.9, (float)0.5, c3, (float)0.2, c2, (float)10);

	Sphere* temp2 = new Sphere(Vector3d(-150, -200, -80), 130, matSp2);



	Emissive * matLight = new Emissive(Vector3d(1.0, 1.0, 1.0), 0.7);
	Rectangle * li = new Rectangle(Vector3d(0, 150, 100), Vector3d(50, 0, 0), Vector3d(0, 0, 50), Vector3d(0.0, -1.0, 0.0), matLight);


//	Reflective * matSp2 = new Reflective((float)0.1, (float)0.1, c3, (float)0.3, c2, (float)100, (float)0.75, c2);
	Sphere* temp4 = new Sphere(Vector3d(-30, 20, 50), 80, matSp2);



	li->sampler = new Sampler();
	li->castShadow = false;
	Vector3d c5 = Vector3d(0.2, 0.9, 0.2);
	Phong * matG = new Phong((float)0.9, (float)0.9, c5, (float)0.1, c2, (float)3);

	Vector3d GroundNorm = Vector3d(0.0, 1.0, -0.4);
	GroundNorm = GroundNorm / GroundNorm.norm();
	Plane* ground = new Plane(Vector3d(-150, -350, -40),GroundNorm, matG);

	AreaLight* light1 = new AreaLight();
	PointLight * light2 = new PointLight(Vector3d(1.0, 1.0, 1.0), Vector3d(0.0, 100.0, 400.0), 3.0);

	light1->setObject(li);
	light1->material = li->material;
	wd->addObjects(temp);
	wd->addObjects(temp2);
	wd->addObjects(temp3);
	wd->addObjects(temp4);
	//wd->addObjects(li);
	wd->addObjects(ground);
	wd->addLights(light2);
	wd->buildTree();

	//wd->orthographicRender(400, false);

	wd->perspectiveRender(Vector3d(0, 0, 400), Vector3d(0, 1, 0), Vector3d(0, 0, 0), 200, false);
	wd->show();

*/

/*Last mp

srand(time(NULL));
	ViewPlane vp;

	Ambient * am = new Ambient();
	am->color = Vector3d(0.3, 0.2, 0.5);
	am->ls = 0.5;
	World *wd = new World(&vp, am);
	Vector3d c1 = Vector3d(0.5, 0.7, 0.2);
	ConstantColor* col1 = new ConstantColor(c1);
	Vector3d c2 = Vector3d(1.0, 1.0, 1.0);
	ConstantColor* col2 = new ConstantColor(c2);

	PlaneChecker * checker = new PlaneChecker(30.0, 5.0);
	Phong * matSp1 = new Phong((float)0.9, (float)0.7, col1, (float)0.6, c2, (float)7.5);
	Sphere* temp = new Sphere(Vector3d(0, -100, -20), 100, matSp1);

	Vector3d c3 = Vector3d(0.4, 0.8, 0.9);
	ConstantColor* col3 = new ConstantColor(c3);
	Phong * matSp2 = new Phong((float)0.9, (float)0.7, col3, (float)0.4, c2, (float)7.5);
	Sphere* temp2 = new Sphere(Vector3d(-150, -200, -40), 130, matSp2);


	Vector3d c4 = Vector3d(0.75, 0.75, 0.9);
	LinearNoise * noi = new LinearNoise(6, -0.1, 1.1);
	FSTexture* col4 = new FSTexture(c4, noi, -0.1, 1.1);
	Phong * matSp3 = new Phong((float)0.9, (float)0.7, col4, (float)0.5, c2, (float)7.5);

	Sphere* temp31 = new Sphere(Vector3d(175, -100, -40), 160, matSp3);
	Cube* temp32 = new Cube(Vector3d(100, -200, -40), 100, matSp3);
	Intersection* temp3 = new Intersection(temp31, temp32);

	Emissive * matLight = new Emissive(Vector3d(1.0, 1.0, 1.0), 0.7);
	Rectangle * li = new Rectangle(Vector3d(0, 150, 100), Vector3d(50, 0, 0), Vector3d(0, 0, 50), Vector3d(0.0, -1.0, 0.0), matLight);

	li->sampler = new Sampler();
	li->castShadow = false;
	Vector3d c5 = Vector3d(0.6, 0.5, 0.5);

	Phong * matG = new Phong((float)0.9, (float)0.9, checker, (float)0.1, c2, (float)3);
	Vector3d GroundNorm = Vector3d(0.0, 1.0, 0.2);
	GroundNorm = GroundNorm / GroundNorm.norm();
	Plane* ground = new Plane(Vector3d(-150, -330, -40), GroundNorm, matG);

	AreaLight* light1 = new AreaLight();
	PointLight * light2 = new PointLight(Vector3d(1.0, 1.0, 1.0), Vector3d(0.0, 300.0, 200.0), 3.0);

	light1->setObject(li);
	light1->material = li->material;
	//wd->addObjects(temp);
	//wd->addObjects(temp2);
	wd->addObjects(temp3);

	wd->addObjects(li);
	wd->addObjects(ground);
	wd->addLights(light1);
	wd->buildTree();

	//wd->orthographicRender(400, false);
	wd->traceArea = true;
	wd->perspectiveRender(Vector3d(0, 0, 300), Vector3d(0, 1, 0), Vector3d(0, 0, 0), 200, false);
	wd->show();



*/

/*Cornell Box

srand(time(NULL));
	ViewPlane vp;

	Ambient * am = new Ambient();
	am->color = Vector3d(0.3, 0.2, 0.5);
	am->ls = 0.5;
	World *wd = new World(&vp, am);
	Vector3d c1 = Vector3d(1.0, 1.0, 1.0);
	Vector3d c2 = Vector3d(0.0, 1.0, 0.0);
	Vector3d c3 = Vector3d(1.0, 0.0, 0.0);
	Vector3d c4 = Vector3d(0.0, 0.0, 1.0);
	ConstantColor* col1 = new ConstantColor(c1);
	ConstantColor* col2 = new ConstantColor(c2);
	ConstantColor* col3 = new ConstantColor(c3);
	ConstantColor* col4 = new ConstantColor(c4);
	PlaneChecker * checker = new PlaneChecker(30.0, 5.0);
	Phong * flo = new Phong((float)0.9, (float)0.7, checker, (float)0.2, c1, (float)7.5);
	Phong * cei = new Phong((float)0.9, (float)0.7, col1, (float)0.2, c1, (float)7.5);
	Phong * gre = new Phong((float)0.9, (float)0.7, col2, (float)0.2, c1, (float)7.5);
	Phong * red = new Phong((float)0.9, (float)0.7, col3, (float)0.2, c1, (float)7.5);
	Phong * blu = new Phong((float)0.9, (float)0.7, col4, (float)0.2, c1, (float)7.5);
	Rectangle * floor = new Rectangle(Vector3d(0, 0, 0), Vector3d(552, 0, 0), Vector3d(0, 0, -559.2), Vector3d(0.0, 1.0, 0.0), flo);

	Rectangle * ceiling = new Rectangle(Vector3d(0, 548.8, 0), Vector3d(556, 0, 0), Vector3d(0, 0, -559.2), Vector3d(0.0, -1.0, 0.0), cei);

	Rectangle * back = new Rectangle(Vector3d(0, 0, -559.2), Vector3d(556, 0, 0), Vector3d(0, 548.8, 0), Vector3d(0.0, 0.0, 1.0), cei);
	Rectangle * left = new Rectangle(Vector3d(0, 0, 0), Vector3d(0, 548.8, 0), Vector3d(0, 0, -559.2), Vector3d(1.0, 0.0, 0.0), gre);
	Rectangle * right = new Rectangle(Vector3d(552, 0, 0), Vector3d(0, 548.8, 0), Vector3d(0, 0, -559.2), Vector3d(-1.0, 0.0, 0.0), red);

	Emissive * matLight = new Emissive(Vector3d(1.0, 1.0, 1.0), 0.7);
	Rectangle * li = new Rectangle(Vector3d(213, 544.8, -227.0), Vector3d(130, 0, 0), Vector3d(0, 0, -105), Vector3d(0.0, -1.0, 0.0), matLight);
	li->sampler = new Sampler();
	li->castShadow = false;
	AreaLight* light1 = new AreaLight();
	light1->setObject(li);
	light1->material = li->material;
	//cout << li->castShadow << "\n";
	Sphere* temp2 = new Sphere(Vector3d(250, 130, -350), 130, blu);

	PointLight * light2 = new PointLight(Vector3d(1.0, 1.0, 1.0), Vector3d(0.0, 300.0, 200.0), 3.0);


	wd->addObjects(floor);
	wd->addObjects(ceiling);
	wd->addObjects(back);
	//wd->addObjects(left);
	//wd->addObjects(right);
	wd->addObjects(temp2);

	wd->addObjects(li);
	//wd->addObjects(ground);
	wd->addLights(light1);
	wd->buildTree();

	//wd->orthographicRender(400, false);
	wd->traceArea = true;
	wd->perspectiveRender(Vector3d(278, 273, 800), Vector3d(0, 1, 0), Vector3d(278, 273, 0), 800, false);
	wd->show();


*/