#pragma once
#include <vector>
#include "Geometry.h"

using namespace std;
class KDtreeNode 
{
public:
	Vector3d maxCoord;
	Vector3d minCoord;
	//double maxCoord[3] = { -DBL_MAX, -DBL_MAX, -DBL_MAX };
	//double minCoord[3] = { DBL_MAX, DBL_MAX, DBL_MAX };
	KDtreeNode* left;
	KDtreeNode* right;
	bool isLeaf = true;
	int dep;
	vector <Geometry*> objects;
	double middle = -1;
	KDtreeNode(vector <Geometry*>& P, int depth, int end, Vector3d& min, Vector3d& max);
	~KDtreeNode();
	

};



class KDtree
{
public:

	static double mainMaxCoord[3];
	static double mainMinCoord[3];
	static double totalRange[3];
	KDtreeNode *root;
	//ViewPlane *vp;
	ShadeData* traverse(Ray &);
	ShadeData* childTraverse(Ray & r, KDtreeNode * nd, double tmin, double tma);
	KDtree();
	KDtree(vector<Geometry*>&);
	~KDtree();
};

