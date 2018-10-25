#include <string>
#include "RandomSample.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

Sampler::Sampler(int ne, int ns) {
	numOfSet = ns;
	numEachRow = ne;
	numOfSample = ne * ne;
	ifstream file("Random.txt");
	if (file.is_open()) {
		for (int i = 0; i < numEachRow * numEachRow * numOfSet; i++) {
			file >> sample[i][0];
			file >> sample[i][1];
			sample[i][0] /= numEachRow;
			sample[i][1] /= numEachRow;
		}
	}
}

double * Sampler::getSample() {
	if (count % numOfSample == 0) {
		int ran = rand() % 16;
		jump = (ran % numOfSet) * numOfSample;
	}
	count += 1;
	return sample[jump + (count%numOfSample)];
}