#pragma once
#include <random>

class Sampler {
public:
	double sample[64][2];
	int numOfSet;
	int numOfSample;
	int count = 0;
	int numEachRow;
	int jump = 0;
	
	Sampler(int = 4, int = 4);
	double * getSample();
};

