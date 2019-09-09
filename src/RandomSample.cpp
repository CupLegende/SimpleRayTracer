
#include <string>
#include "RandomSample.h"
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <Eigen/Dense>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace Eigen;




	float randomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}

	int randInt(int Min, int Max) {
		return rand() % (Max + 1 - Min) + Min;
	}





	Sampler::Sampler(int ns, int nset) {
		num_samples = ns;
		num_sets = nset;
		count = 0;
		jump = 0;
		samples.reserve(num_samples * num_sets);
		setup_shuffled_indices();
		generateSamples();
	}

	Sampler::Sampler(const int ns) {
		num_samples = ns;
		num_sets = 20;
		count = 0;
		jump = 0;
		samples.reserve(num_samples * num_sets);
		setup_shuffled_indices();
		generateSamples();
	}

	Sampler::Sampler() {
		num_samples = 4;
		num_sets = 20;
		count = 0;
		jump = 0;
		samples.reserve(num_samples * num_sets);
		setup_shuffled_indices();
		generateSamples();
	}


	void Sampler::setup_shuffled_indices() {
		shuffledIndices.reserve(num_samples * num_sets);
		vector<int> indices;

		for (int j = 0; j < num_samples; j++)
			indices.push_back(j);

		for (int p = 0; p < num_sets; p++) {
			random_shuffle(indices.begin(), indices.end());

			for (int j = 0; j < num_samples; j++)
				shuffledIndices.push_back(indices[j]);
		}
	}



	void Sampler::generateSamples() {
		int n = (int)sqrt((float)num_samples);
		float width = 1.0 / ((float)num_samples);
		//Vector2d temp = Vector2d(0.0,0.0);
		for (int i = 0; i < num_samples * num_sets; i++) {
			Vector2d temp = Vector2d(0.0, 0.0);
			samples.push_back(temp);
		}
		for (int set = 0; set < num_sets; set++) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {

					samples[i * n + j + set * num_samples][0] = (i * n + j) * width + randomFloat(0.0, width);
					samples[i * n + j + set * num_samples][1] = (j * n + i) * width + randomFloat(0.0, width);
				}
			}

		}

		for (int set = 0; set < num_sets; set++) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					int k = randInt(j, n - 1);
					float t = samples[i * n + j + set * num_samples][0];
					samples[i * n + j + set * num_samples][0] = samples[i * n + k + set * num_samples][0];
					samples[i * n + k + set * num_samples][0] = t;
				}
			}
		}

		for (int set = 0; set < num_sets; set++) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					int k = randInt(j, n - 1);
					float t = samples[i * n + j + set * num_samples][1];
					samples[i * n + j + set * num_samples][1] = samples[i * n + k + set * num_samples][1];
					samples[i * n + k + set * num_samples][1] = t;
				}
			}
		}


	}

	void
		Sampler::generateDisk(void) {
		int size = samples.size();
		float r, phi;
		Vector2d sp;

		diskSamples.reserve(size);

		for (int j = 0; j < size; j++) {
			// map sample point to [-1, 1] X [-1,1]

			sp[0] = 2.0 * samples[j][0] - 1.0;
			sp[1] = 2.0 * samples[j][1] - 1.0;

			if (sp[0] > -sp[1]) {			// sectors 1 and 2
				if (sp[0] > sp[1]) {		// sector 1
					r = sp[0];
					phi = sp[1] / sp[0];
				}
				else {					// sector 2
					r = sp[1];
					phi = 2 - sp[0] / sp[1];
				}
			}
			else {						// sectors 3 and 4
				if (sp[0] < sp[1]) {		// sector 3
					r = -sp[0];
					phi = 4 + sp[1] / sp[0];
				}
				else {					// sector 4
					r = -sp[1];
					if (sp[1] != 0.0)	// avoid division by zero at origin
						phi = 6 - sp[0] / sp[1];
					else
						phi = 0.0;
				}
			}

			phi *= M_PI / 4.0;

			diskSamples[j][0] = r * cos(phi);
			diskSamples[j][1] = r * sin(phi);
		}

		//samples.erase(samples.begin(), samples.end());
	}





	void Sampler::generateHemi(float exp) {
		int size = samples.size();
		hemisphereSamples.reserve(num_samples * num_sets);

		for (int j = 0; j < size; j++) {
			float cos_phi = cos(2.0 * M_PI * samples[j][0]);
			float sin_phi = sin(2.0 * M_PI * samples[j][0]);
			float cos_theta = pow((1.0 - samples[j][1]), 1.0 / (exp + 1.0));
			float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
			float pu = sin_theta * cos_phi;
			float pv = sin_theta * sin_phi;
			float pw = cos_theta;
			hemisphereSamples.push_back(Vector3d(pu, pv, pw));
		}

	}
	void Sampler::generateSphere() {
		float r1, r2;
		float x, y, z;
		float r, phi;

		sphereSamples.reserve(num_samples * num_sets);

		for (int j = 0; j < num_samples * num_sets; j++) {
			r1 = samples[j][0];
			r2 = samples[j][1];
			z = 1.0 - 2.0 * r1;
			r = sqrt(1.0 - z * z);
			phi = 2 * M_PI * r2;
			x = r * cos(phi);
			y = r * sin(phi);
			sphereSamples.push_back(Vector3d(x, y, z));
		}
	}


	Vector2d Sampler::sampleSquare() {
		if (count % num_samples == 0)
			jump = (rand() % num_sets) * num_samples;

		return (samples[jump + shuffledIndices[jump + count++ % num_samples]]);
	}


	Vector2d Sampler::sampleDisk() {
		if (count % num_samples == 0)
			jump = (rand() % num_sets) * num_samples;

		return (diskSamples[jump + shuffledIndices[jump + count++ % num_samples]]);
	}

	Vector3d Sampler::sampleHemi() {
		if (count % num_samples == 0)
			jump = (rand() % num_sets) * num_samples;

		return (hemisphereSamples[jump + shuffledIndices[jump + count++ % num_samples]]);
	}



	Vector3d Sampler::sampleSphere() {
		if (count % num_samples == 0)
			jump = (rand() % num_sets) * num_samples;

		return (sphereSamples[jump + shuffledIndices[jump + count++ % num_samples]]);
	}




