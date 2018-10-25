#include "ObjReader.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <Eigen/dense>
#include "Geometry.h"

using namespace Eigen;
using namespace std;
ObjReader::ObjReader()
{
}

bool ObjReader::read(const char* path, vector<Vector3d*> &vertices, vector<Triangle*> &triangles) {
	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			double tempCoord[3];
			
			fscanf(file, "%lf %lf %lf\n", &tempCoord[0], &tempCoord[1], &tempCoord[2]);
			Vector3d * vertex = new Vector3d(tempCoord[0], tempCoord[1], tempCoord[2]);
			vertices.push_back(vertex);
		}

		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3];
			int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
			//cout << vertices.size();

			

			Triangle * temp = new Triangle( (*vertices.at(vertexIndex[0]-1)) * 300, (*vertices.at(vertexIndex[1]-1)) * 300, (*vertices.at(vertexIndex[2]-1)) * 300, Vector3d(0.8,0.6,0.4));
			triangles.push_back(temp);
		}


	}
	return true;
}

ObjReader::~ObjReader()
{
}
