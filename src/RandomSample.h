#pragma once


#include <random>
#include <Eigen/Dense>
#include <stdlib.h>

using namespace std;
using namespace Eigen;
float randomFloat(float a, float b);
int randInt(int Min, int Max);


class Sampler {
public:
	int num_samples;
	int num_sets;
	vector<Vector2d> samples;
	vector<int>  shuffledIndices;
	vector<Vector2d> diskSamples;
	vector<Vector3d> hemisphereSamples;
	vector<Vector3d> sphereSamples;
	unsigned long count;
	int jump;

	Sampler(void);
	Sampler(const int num);
	Sampler(const int num, const int num_sets);
	//Sampler(const Sampler& s);

	Vector2d sampleSquare();
	Vector2d sampleDisk();
	Vector3d sampleHemi();
	Vector3d sampleSphere();
	void generateSamples();
	void generateSphere();
	void generateHemi(float exp);
	void generateDisk();
	void setup_shuffled_indices();
};

