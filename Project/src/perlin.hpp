#include "algebra.hpp"

class perlin {
public:
	int p[512];
	perlin(); 
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

double perlinNoise(double x, double y, double z);
