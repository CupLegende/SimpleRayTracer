#include <Eigen/dense>
#include "World.h"
#include "GUI.h"
#include "ObjReader.h"
#include "Geometry.h"
#include <stdio.h>
#include "Graphics.h"
#include <iostream>
using namespace Eigen;
using namespace std;

int main() {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(9554);
	//_CrtSetBreakAlloc(9553);
	//_CrtSetBreakAlloc(9552);
	//cout << "what";
	vector<Vector3d*> vertices;
	vector<Triangle*> triangles;
	srand(time(NULL));
	bool tryRead = ObjReader::read("A:\\Sophomore\\CS419\\dragon.obj", vertices, triangles);
	//cout << "what";

	

	ViewPlane vp;

	Vector3d am(0.3, 0.3, 0.6);
	
	
	World *wd = new World(&vp, am);
	
	/* 
	for (auto & tri : triangles) {
		
		wd->addObjects(tri);
	}
	for (auto & i : vertices) {
		delete i;
	}
	*/
	

	//Sphere* temp = new Sphere(Vector3d(0, 0, -20), 100, Vector3d(0.5, 0.7, 0.2));
	//Sphere* temp2 = new Sphere(Vector3d(-150, -200, -40), 130, Vector3d(0.4, 0.8, 0.9));

	
	//wd->addObjects(temp);
	//wd->addObjects(temp2);

	/* 
	for (auto & i : triangles) {
		world.addObjects(i);
	}
	for (auto & i : vertices) {
		delete i;
	}
	*/
	/* 
	Triangle * tri = new Triangle(Vector3d(50, -20, 20), Vector3d(-10, 130, 30), Vector3d(-90, 130, 40),
		Vector3d((double)rand() / (2 * RAND_MAX), (double)rand() / (2 * RAND_MAX), (double)rand() / (2 * RAND_MAX))
		);
	Triangle * tri2 = new Triangle(Vector3d(40, 40, 50), Vector3d(10, -25, -10), Vector3d(-70, -5, -70),
		Vector3d((double)rand() / (2 * RAND_MAX), (double)rand() / (2 * RAND_MAX), (double)rand() / (2 * RAND_MAX))
		);
		*/
	//wd->addObjects(tri);
	//wd->addObjects(tri2);
	
	
	for (int i = 0; i < 100; i++) {
	Sphere* temp = new Sphere(Vector3d(rand() % 500 -250, rand() % 500 - 250, rand() % 500 - 250), 15, Vector3d((double)rand()/(2*RAND_MAX), (double)rand() / (2*RAND_MAX), (double)rand() / (2*RAND_MAX)));
	//Sphere* temp2 = new Sphere(Vector3d(-150, -200, -40), 130, Vector3d(0.4, 0.8, 0.9));
	wd->addObjects(temp);
	}
	

	Ray r = {
		Vector3d(0.0,0.0, 300.0),
		Vector3d(0.0,0.0, -1.0)
	};

		Light light1 = { r,
		Vector3d(0.7, 0.1, 0.2),
		Vector3d(1.0,1.0,1.0)
	};

		


		Ray r2 = {
		Vector3d(0.0,300.0, 50.0),
		Vector3d(0.0,-1.0, 0.0)
		};

		Light light2 = { r2,
		Vector3d(0.2, 0.9, 0.2),
		Vector3d(1.0,1.0,1.0)
		};

		Ray r3 = {
		Vector3d(-300,-300, 50.0),
		(Vector3d(1.0,1.0, 0.0)/Vector3d(1.0,1.0, 0.0).norm())
		};

		Light light3 = { r3,
		Vector3d(0.3, 0.3, 0.9),
		Vector3d(1.0,1.0,1.0)
		};


		Ray r4 = {
		Vector3d(0.0, 0.0, 0.0),
		Vector3d(0.0,-1.0, 0.0)
		};

		Light light4 = { r4,
		Vector3d(0.3, 0.3, 0.8),
		Vector3d(1.0,1.0,1.0)
		};

		//cout << "hi";
		wd->buildTree();
		wd->addLights(&light1);
		wd->addLights(&light2);
		wd->addLights(&light3);
		wd->addLights(&light4);
		wd->orthographicRender(400, false);
		//wd->perspectiveRender(Vector3d(0, -60, 60), Vector3d(0, 0, -1), Vector3d(0, 0, 0), 50, false);
		wd->show();


	return 0;
}