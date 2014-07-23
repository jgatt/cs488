#ifndef		__HELPERS__
#define		__HELPERS__

#include "image.hpp"
#include "CubeMap.hpp"
#include "algebra.hpp"
#include <stdlib.h>
#include <math.h>

void readTexture(Image *m_texture, double u, double v, Colour &colour);

void readCubeMap(Vector3D vp, CubeMap* texture, Colour &colour); 

#endif