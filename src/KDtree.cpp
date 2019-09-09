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
			double cost1 = (clipPos - Vmin[rotate]) / surface;
			double cost2 = (Vmax[rotate] - clipPos) / surface;
			double currentCost = ( triCount) * cost1 + (total) * cost2;

			if (currentCost < optimalCost && cost1 != 0 && cost2 !=0) {
				optimalCost = currentCost;
				//optimalIndex = triCount-1;
				//middle = temp.at(optimalIndex)->maxCoord[depth % 3];
				middle = clipPos;
			}

			if (!j->max)
			{
				triCount++;
			}
		}

		for (auto & i : temp) {
			
			if (i->pos > middle && i->max) {
				//repeat push
				//bool ri = true;
				//for (auto & j : rightVec) {
					
					//if (j == i->ge) {
						
						//ri = false;
					//}
				//}
				//if(ri)
					rightVec.push_back(i->ge);
			}
			if (i->pos < middle && !(i->max)) {
				//bool le = true;
				//for (auto & j : leftVec) {
					
					//if (j == i->ge) {
						//le = false;
					//}
				//}
			//	if(le)
					leftVec.push_back(i->ge);
				
			}
			//may need to test

			if (i->ge->maxCoord[rotate] == i->ge->minCoord[rotate]) {
				if (i->ge->maxCoord[rotate] == middle && i->max)
				{
					rightVec.push_back(i->ge);

				}
				if (i->ge->minCoord[rotate] == middle && !(i->max))
				{
					leftVec.push_back(i->ge);

				}
			}
			


		}

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
		cout << minCoord << "\n";
		cout << maxCoord << "\n";
		

		for (auto tempi : objects) {
			cout << tempi->minCoord[0] << "\n";
			cout << tempi->maxCoord[0] << "\n";
		}
		cout << "\n";
	}
}

KDtreeNode::~KDtreeNode() {

}


ShadeData* KDtree::traverseShallow(Ray & r, float d) {
	double t;
	double t1 = (-r.origin[0] + mainMinCoord[0]) / r.direction[0];
	double t2 = (-r.origin[0] + mainMaxCoord[0]) / r.direction[0];

	double t3 = (-r.origin[1] + mainMinCoord[1]) / r.direction[1];
	double t4 = (-r.origin[1] + mainMaxCoord[1]) / r.direction[1];

	double t5 = (-r.origin[2] + mainMinCoord[2]) / r.direction[2];
	double t6 = (-r.origin[2] + mainMaxCoord[2]) / r.direction[2];

	double tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	double tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}
	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	return childTraverseShallow(r, root, tmin, tmax, d);
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

ShadeData* KDtree::childTraverseShallow(Ray & r, KDtreeNode * nd, double tmin, double tmax, float d) {
	KDtreeNode* temp = nd;
	//	cout << tmin << "\n";
		//cout << tmax << "\n";

		/*
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
		}*/

	if ((temp->isLeaf == false)) {
		int i = temp->dep % 3;
		KDtreeNode* nearNode = r.origin[i] < temp->middle ? temp->left : temp->right;
		KDtreeNode* farNode = r.origin[i] < temp->middle ? temp->right : temp->left;
		double splitT = (temp->middle - r.origin[i]) / r.direction[i];

		if (splitT > tmax) {
			return childTraverseShallow(r, nearNode, tmin, tmax, d);
		}
		else if (splitT < tmin) {
			if (splitT > 0) {
				return childTraverseShallow(r, farNode, tmin, tmax, d);
			}
			else if (splitT < 0) {
				return childTraverseShallow(r, nearNode, tmin, tmax, d);
			}
			else {
				if (r.direction[i] < 0)
					return childTraverseShallow(r, farNode, tmin, tmax, d);
				else
					return childTraverseShallow(r, nearNode, tmin, tmax, d);
			}
		}
		else {
			if (splitT > 0) {
				ShadeData* tempSD = childTraverseShallow(r, nearNode, tmin, splitT, d);

				//else {
				ShadeData* tempSD2 = childTraverseShallow(r, farNode, splitT, tmax, d);
				//}

				if (tempSD != nullptr && tempSD2 != nullptr) {
					if (tempSD2->t < tempSD->t) {
						delete tempSD;
						return tempSD2;
					}
					delete tempSD2;
					return tempSD;
				}
				if (tempSD != nullptr) {
					return tempSD;
				}
				return tempSD2;
			}
			else {
				return childTraverseShallow(r, nearNode, splitT, tmax, d);
			}
		}

	}

	if (temp->isLeaf) {
		ShadeData * output = new ShadeData;
		float localTmin = FLT_MAX;
		bool hitObject = false;
		for (auto & i : temp->objects) {
			//if (i->minCoord[0] == 213.0)
				//cout << "hit" << "\n";
			bool flag = i->hit(r, output);
			if (flag && output->t < localTmin) {
				hitObject = true;
				localTmin = output->t;
				output->castShadow = i->castShadow;
			}
		}
		if (hitObject) {

			//if (output->castShadow)
			//{

			//	cout << tmin << "\n";
				//cout << tmax << "\n";
			//}
			return output;
		}




		return nullptr;
	}
}


ShadeData* KDtree::childTraverse(Ray & r, KDtreeNode * nd, double tmin, double tmax) {
	KDtreeNode* temp = nd;
//	cout << tmin << "\n";
	//cout << tmax << "\n";

	/* 
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
	}*/

	if ((temp->isLeaf == false)) {
		int i = temp->dep % 3;
		KDtreeNode* nearNode = r.origin[i] < temp->middle ? temp->left : temp->right;
		KDtreeNode* farNode = r.origin[i] < temp->middle ? temp->right : temp->left;
		double splitT = (temp->middle - r.origin[i]) / r.direction[i];

		if (splitT > tmax) {
			return childTraverse(r, nearNode, tmin, tmax);
		}
		else if (splitT < tmin) {
			if (splitT > 0) {
				return childTraverse(r, farNode, tmin, tmax);
			}
			else if (splitT < 0) {
				return childTraverse(r, nearNode, tmin, tmax);
			}
			else {
				if (r.direction[i] < 0)
					return childTraverse(r, farNode, tmin, tmax);
				else
					return childTraverse(r, nearNode, tmin, tmax);
			}
		}
		else {
			if (splitT > 0) {
				ShadeData* tempSD= childTraverse(r, nearNode, tmin, splitT);
				
				//else {
				ShadeData* tempSD2=  childTraverse(r, farNode, splitT, tmax);
				//}

				if (tempSD != nullptr && tempSD2 != nullptr) {
					if (tempSD2->t < tempSD->t) {
						delete tempSD;
						return tempSD2;
					}
					delete tempSD2;
					return tempSD;
				}
				if (tempSD != nullptr) {
					return tempSD;
				}
				return tempSD2;
			}
			else {
				return childTraverse(r, nearNode, splitT, tmax);
			}
		}

	}

	if (temp->isLeaf) {
			ShadeData * output = new ShadeData;
			float localTmin = FLT_MAX;
			bool hitObject = false;
			for (auto & i : temp->objects) {
				//if (i->minCoord[0] == 213.0)
					//cout << "hit" << "\n";
				bool flag = i->hit(r, output);
				if (flag && output->t < localTmin) {
					hitObject = true;
					localTmin = output->t;
					output->castShadow = i->castShadow;
					
				}
			}
			if (hitObject) {

				//if (output->castShadow)
				//{
					
				//	cout << tmin << "\n";
					//cout << tmax << "\n";
				//}
				//if (output->castShadow == false)
				//{	
					//cout << "hit" << "\n";
				//}
				return output;
			}

			
			

			return nullptr;
	}
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
