#include "Shader.h"
#include <Eigen/dense>
#include <math.h>

using namespace Eigen;
using namespace std;


Shader::Shader(Vector3d & ambi)
{
	ambientLight = ambi;
}

Vector3d* Shader::shade(Ray& r, ShadeData* sd, KDtree* tree, 
	vector<Light*>* Li)
{
	float tmin = FLT_MAX;
	Vector3d material;
	Vector3d normal;
	Vector3d color;
	tmin = sd->t;
	material = sd->material;
	Vector3d hitPoint = r.origin + sd->t*r.direction;
	normal = sd->normal;
	Vector3d * output = new Vector3d;
	*output = sd->ambientCoeff * sd->material.cwiseProduct(ambientLight);
	int num_lights = (Li)->size();

	for (int j = 0; j < num_lights; j++) {
		Vector3d wi = (Li)->at(j)->r.origin - sd->hitPoint;
		wi = wi / wi.norm();
		float ndotwi = max((sd->normal).dot(wi),0.0);
		if (ndotwi > 0.0) {
			bool inShadow = false;
			Ray shadowRay = { sd->hitPoint + shadowEpsilon * wi, wi };
			ShadeData * shadowData = new ShadeData;
			double lightT = ((Li)->at(j)->r.origin[0] - shadowRay.origin[0]) / shadowRay.direction[0];
			shadowData = tree->traverse(shadowRay);

			//shadowData = nullptr;

			if (shadowData == nullptr || shadowData->t > lightT) {
				Vector3d halfWay = wi + -1.0 * r.direction;
				halfWay = halfWay / halfWay.norm();
				float rdotn = max(halfWay.dot(sd->normal), 0.0);
				 *output += ((sd->material.cwiseProduct( (Li)->at(j)->diffuse * ndotwi *sd->diffuseCoeff))
					+ sd->specCoeff * (Li)->at(j)->spec * pow(rdotn, sd->specAlpha));

				//*output += (sd->material.cwiseProduct((Li)->at(j)->diffuse * ndotwi *sd->diffuseCoeff));

				//*output += sd->specCoeff * (Li)->at(j)->spec * pow(rdotn, sd->specAlpha);

			}
			delete shadowData;
		}
	}
	//*output = (*output).cwiseProduct(sd->material);
	return output;
}

Shader::Shader()
{
}

Shader::~Shader()
{
}
