#include "KDtree.h"
#include <iostream>
using namespace std;

struct GeoWithIndex {
	Geometry * ge;
	double pos;
	bool max;
};
struct by_Pos {
	bool operator()(GeoWithIndex *a, GeoWithIndex *b) const noexcept {
		return a->pos < b->pos;
	}
};

/* 
struct by_maxX {
	bool operator()(GeoWithIndex *a, GeoWithIndex *b) const noexcept {
		return a->ge->maxCoord[0] < b->ge->maxCoord[0];
	}
};

struct by_maxY {
	bool operator()(GeoWithIndex *a, GeoWithIndex *b) const noexcept {
		return a->ge->maxCoord[1] < b->ge->maxCoord[1];
	}
};

struct by_maxZ {
	bool operator()(GeoWithIndex *a, GeoWithIndex *b) const noexcept {
		return a->ge->maxCoord[2] < b->ge->maxCoord[2];
	}
};

struct by_minX {
	bool operator()(GeoWithIndex *a, GeoWithIndex *b) const noexcept {
		return a->ge->minCoord[0] < b->ge->minCoord[0];
	}
};

struct by_minY {
	bool operator()(GeoWithIndex *a, GeoWithIndex *b) const noexcept {
		return a->ge->minCoord[1] < b->ge->minCoord[1];
	}
};

struct by_minZ {
	bool operator()(GeoWithIndex *a, GeoWithIndex *b) const noexcept {
		return a->ge->minCoord[2] < b->ge->minCoord[2];
	}
};*/


KDtreeNode::KDtreeNode(vector <Geometry*>& P, int depth, int end,  Vector3d &Vmin, Vector3d &Vmax) {
	maxCoord = Vmax;
	minCoord = Vmin;
	dep = depth;
	int rotate = depth % 3;
	vector<GeoWithIndex*> temp;
	for (auto & i : P) {
		GeoWithIndex * stu1 = new GeoWithIndex{
			i,
			i->minCoord[rotate],
			false
		};
		GeoWithIndex * stu2 = new GeoWithIndex{
			i,
			i->maxCoord[rotate],
			true
		};

		temp.push_back(stu1);
		temp.push_back(stu2);
	}
	sort(temp.begin(), temp.end(), by_Pos());

	/* 
	if (rotate == 0) {
		sort(temp.begin(), temp.end(), by_minX());
		sort(temp2.begin(), temp2.end(), by_maxX());
	}
	else if (rotate == 1) {
		sort(temp.begin(), temp.end(), by_minY());
		sort(temp2.begin(), temp2.end(), by_maxY());
	}
	else {
		sort(temp.begin(), temp.end(), by_minZ());
		sort(temp2.begin(), temp2.end(), by_maxZ());
	}
	

	int count = 0;
	for (auto & i : temp) {
		i->indi = count;
		count++;
	}
	*/

	//hardcode cri
	if (P.size() >= 3 && (Vmax[rotate] - Vmin[rotate]) / KDtree::totalRange[rotate] >= 0.02 && end <= 2)
	{
		//double cvisit = 1 / 80;
		int triCount = 0;
		
		int optimalIndex = -1;
		double optimalCost = DBL_MAX;
		int total = P.size();
		double surface = Vmax[rotate] - Vmin[rotate];
		vector <Geometry*> leftVec;
		vector <Geometry*> rightVec;
		for (auto & j : temp) {
			if (j->max)
			{
				total--;
			}
			
			auto &i = j->ge;
			double clipPos = min(max(j->pos, Vmin[rotate]), Vmax[rotate]);
			double currentCost = ( triCount) * (clipPos - Vmin[rotate]) / surface + 
				(total) * (Vmax[rotate]-clipPos) / surface;

			if (currentCost < optimalCost) {
				optimalCost = currentCost;
				//optimalIndex = triCount-1;
				//middle = temp.at(optimalIndex)->maxCoord[depth % 3];
				middle = clipPos;
			}

			if (!j->max)
			{
				triCount++;
			}

			/* 
			minCoord[0] = min(i->minCoord[0], minCoord[0]);
			minCoord[1] = min(i->minCoord[1], minCoord[1]);
			minCoord[2] = min(i->minCoord[2], minCoord[2]);
			maxCoord[0] = max(i->maxCoord[0], maxCoord[0]);
			maxCoord[1] = max(i->maxCoord[1], maxCoord[1]);
			maxCoord[2] = max(i->maxCoord[2], maxCoord[2]);
			*/
		}

		for (auto & i : temp) {
			if (i->pos >= middle && i->max) {
				rightVec.push_back(i->ge);
			}
			if (i->pos <= middle && !(i->max)) {
				leftVec.push_back(i->ge);
			}
		}
		/* 
		for (auto & i : temp) {
			if (i->minCoord[depth % 3] <= middle) {
				leftVec.push_back(i);
			}
		}
		*/
		//push left by least, may be optimized
		/*
		if (depth % 3 == 0) {
			sort(temp.begin(), temp.end(), by_minX());
		}
		else if (depth % 3 == 1) {
			sort(temp.begin(), temp.end(), by_minY());
		}
		else {
			sort(temp.begin(), temp.end(), by_minZ());
		}
		*/

		Vector3d nextMin1 = Vmin;
		Vector3d nextMin2 = Vmin;
		Vector3d nextMax1 = Vmax;
		Vector3d nextMax2 = Vmax;
		nextMax1[rotate] = middle;
		nextMin2[rotate] = middle;
		for (auto & i : temp) {
			delete i;
		}
		int leftR = P.size() - leftVec.size();
		int rightR = P.size() - rightVec.size();
		double reduced = (min( (middle - Vmin[rotate]) * leftR, (Vmax[rotate] - middle) * rightR)/ KDtree::totalRange[rotate]);
		if (leftR < 3 || rightR < 3 || reduced < 0.01) {
			if (end < 2) {
				//cout << reduced << "\n";
				isLeaf = false;
				left = new KDtreeNode(leftVec, depth + 1, end + 1, nextMin1, nextMax1);
				right = new KDtreeNode(rightVec, depth + 1, end + 1, nextMin2, nextMax2);
			}
		}
		else {
			//cout << reduced << "\n";
			isLeaf = false;
			left = new KDtreeNode(leftVec, depth + 1, 0, nextMin1, nextMax1);
			right = new KDtreeNode(rightVec, depth + 1, 0, nextMin2, nextMax2);
		}
	}

	if(isLeaf){
		objects = P;
		cout << objects.size() << "\n";
	}
}

KDtreeNode::~KDtreeNode() {

}

ShadeData* KDtree::traverse(Ray & r) {
	double t;
	double t1 = (- r.origin[0] + mainMinCoord[0]) / r.direction[0];
	double t2 = (- r.origin[0] + mainMaxCoord[0]) / r.direction[0];

	double t3 = (- r.origin[1] + mainMinCoord[1]) / r.direction[1];
	double t4 = (- r.origin[1] + mainMaxCoord[1]) / r.direction[1];

	double t5 = (- r.origin[2] + mainMinCoord[2]) / r.direction[2];
	double t6 = (- r.origin[2] + mainMaxCoord[2]) / r.direction[2];

	double tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	double tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
		return nullptr;
	}
	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return nullptr;
	}
	return childTraverse(r, root, tmin, tmax);
}

ShadeData* KDtree::childTraverse(Ray & r, KDtreeNode * nd, double tmin, double tmax) {
	KDtreeNode* temp = nd;
	while (temp->isLeaf == false) {
		int i = temp->dep % 3;
		KDtreeNode* nearNode = r.origin[i] < temp->middle ? temp->left : temp->right;
		KDtreeNode* farNode = r.origin[i] < temp->middle ? temp->right : temp->left;
		double splitT = (temp->middle - r.origin[i]) / r.direction[i];
		if (splitT <= tmin) {
			temp = farNode;
			//tmin = splitT;
		}
		if (splitT >= tmax || splitT<0) {
			temp = nearNode;
			//tmax = splitT;
		}
		else {
			ShadeData* tempSD = childTraverse(r, nearNode, tmin, splitT);
			if (tempSD != nullptr) {
				return tempSD;
			}
			else {
				return childTraverse(r, farNode, splitT, tmax);
			}
		}
	}

	if (temp->isLeaf) {
		 /* 
		double t1 = (r.origin[0] - temp->minCoord[0]) / r.direction[0];
		double t2 = (r.origin[0] - temp->maxCoord[0]) / r.direction[0];

		double t3 = (r.origin[1] - temp->minCoord[1]) / r.direction[1];
		double t4 = (r.origin[1] - temp->maxCoord[1]) / r.direction[1];

		double t5 = (r.origin[2] - temp->minCoord[2]) / r.direction[2];
		double t6 = (r.origin[2] - temp->maxCoord[2]) / r.direction[2];

		double tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
		double tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
		
		// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us

		if (tmax < 0)
		{
			return nullptr;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{
			return nullptr;
		}
		*/
	//	else {
			ShadeData * output = new ShadeData;
			int localTmin = INT_MAX;
			bool hitObject = false;
			for (auto & i : temp->objects) {
				bool flag = i->hit(r, output);
				if (flag && output->t < localTmin) {
					hitObject = true;
					localTmin = output->t;
				}
			}
			if (hitObject) {
				return output;
			}
			return nullptr;
	//	}
	}
	/*else {
		int i = nd.dep % 3;
		double splitT = (nd.middle - r.origin[i]) / r.direction[i];
		if (splitT > tmin) {
			return traverse(r, *(nd.left));
		}
		else {
			return traverse(r, *(nd.right));
		}
	}*/
}



double KDtree::mainMaxCoord[3] = { -DBL_MAX, -DBL_MAX, -DBL_MAX };
double KDtree::mainMinCoord[3] = { DBL_MAX, DBL_MAX, DBL_MAX };
double KDtree::totalRange[3] = { 0.0, 0.0, 0.0 };
KDtree::KDtree(vector<Geometry*>& Geo)
{	
	totalRange[0] = -1;
	totalRange[1] = -1;
	totalRange[2] = -1;
	bool stop = false;

	for (auto & i : Geo) {
		mainMinCoord[0] = min(i->minCoord[0], mainMinCoord[0]);
		mainMinCoord[1] = min(i->minCoord[1], mainMinCoord[1]);
		mainMinCoord[2] = min(i->minCoord[2], mainMinCoord[2]);
		mainMaxCoord[0] = max(i->maxCoord[0], mainMaxCoord[0]);
		mainMaxCoord[1] = max(i->maxCoord[1], mainMaxCoord[1]);
		mainMaxCoord[2] = max(i->maxCoord[2], mainMaxCoord[2]);

	}
	totalRange[0] = mainMaxCoord[0] - mainMinCoord[0];
	totalRange[1] = mainMaxCoord[1] - mainMinCoord[1];
	totalRange[2] = mainMaxCoord[2] - mainMinCoord[2];

	Vector3d nextMin(mainMinCoord[0], mainMinCoord[1], mainMinCoord[2]);
	Vector3d nextMax(mainMaxCoord[0], mainMaxCoord[1], mainMaxCoord[2]);
	root = new KDtreeNode(Geo, 0, 0, nextMin, nextMax);

	root->maxCoord[0] = mainMaxCoord[0];
	root->maxCoord[1] = mainMaxCoord[1];
	root->maxCoord[2] = mainMaxCoord[2];
	root->minCoord[0] = mainMinCoord[0];
	root->minCoord[1] = mainMinCoord[1];
	root->minCoord[2] = mainMinCoord[2];
}


KDtree::KDtree()
{

}

KDtree::~KDtree()
{
	
}
