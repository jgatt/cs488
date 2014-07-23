#include "primitive.hpp"
#include "polyroots.hpp"
#include <stdlib.h>
#include <iostream>
#include "perlin.hpp"
#include "helpers.hpp"

using namespace std;

Primitive::~Primitive()
{
}


Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierCylinder::~NonhierCylinder()
{
}

NonhierCone::~NonhierCone()
{
}

NonhierBox::~NonhierBox()
{
}

bool Primitive::intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]) {

}

bool Cube::intersection(Point3D rayOrigin, Vector3D rayDir, double &ret, Point3D &intersection, Vector3D &normal) {

} 

bool Sphere::intersection(Point3D rayOrigin, Vector3D rayDir, double &ret, Point3D &intersection, Vector3D &normal) {

}

bool NonhierBox::intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]) {
	float tmin, tmax, tymin, tymax, tzmin, tzmax, divx, divy, divz;
	Point3D max_pos = Point3D(m_pos[0] + m_size, m_pos[1] + m_size, m_pos[2] + m_size);
	Point3D centre_pos = Point3D(m_pos[0] + (m_size/2), m_pos[1] + (m_size/2), m_pos[2] + (m_size/2));

	divx = 1.0f / rayDir[0]; 
	if (divx >= 0) {
		tmin = (m_pos[0] - rayOrigin[0]) * divx;
		tmax = (max_pos[0] - rayOrigin[0]) * divx;
	} else {
		tmin = (max_pos[0] - rayOrigin[0]) * divx;
		tmax = (m_pos[0] - rayOrigin[0]) * divx;
	}	
	
	divy = 1.0f / rayDir[1];
	if (divy >= 0) {
		tymin = (m_pos[1] - rayOrigin[1]) * divy; 
		tymax = (max_pos[1] - rayOrigin[1]) * divy;
	} else {
		tymax = (m_pos[1] - rayOrigin[1]) * divy;
		tymin = (max_pos[1] - rayOrigin[1]) * divy;
	}	

	if ((tmin > tymax) || (tymin > tmax)) {
		return false;
	}	
	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}
	
	divz = 1.0f / rayDir[2];
	if (divz >= 0) {
		tzmin = (m_pos[2] - rayOrigin[2]) * divz;
		tzmax = (max_pos[2] - rayOrigin[2]) * divz;
	} else {
		tzmin = (max_pos[2] - rayOrigin[2]) * divz;
		tzmax = (m_pos[2] - rayOrigin[2]) * divz;
	}	
	if ((tmin > tzmax) || (tzmin > tmax)) {
		return false;
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}

	if ((tmin < 500.0) && (tmax > 0)) {
		ret[0] = tmin;
		ret[1] = tmax;

		intersection[0] = rayOrigin + ret[0]*rayDir;
		intersection[1] = rayOrigin + ret[1]*rayDir;

		normal[0] = intersection[0] - centre_pos;
		normal[1] = intersection[1] - centre_pos;
		return true;
	} else {
		return false;
	}
} 

bool NonhierSphere::intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]) {
	double a, b, c;

	Vector3D orgpos = rayOrigin - m_pos;
	double *roots = (double*)malloc(2*sizeof(double));

	a = rayDir.dot(rayDir);
	b = 2*(orgpos.dot(rayDir));
	c = orgpos.dot(orgpos) - (m_radius * m_radius); 

	//double bump = (rand() % 101) / 100.0;

	size_t num_roots = quadraticRoots( a, b, c, roots);
	if (num_roots > 0) {

		if (roots[0] <= roots[1] || num_roots == 1) {
			ret[0] =  roots[0];
			ret[1] = roots[1];
		} else {
			ret[0] = roots[1];
			ret[1] = roots[0];
		}	

		intersection[0] = rayOrigin + ret[0]*rayDir;  
		intersection[1] = rayOrigin + ret[1]*rayDir;  

		// //bump mapping

		// double noiseX = perlinNoise(0.1 * intersection[0][0],
		// 	0.1 * intersection[0][1],
		// 	0.1 * intersection[0][2]); 

		// double noiseY = perlinNoise(0.1 * intersection[0][1],
		// 	0.1 * intersection[0][2],
		// 	0.1 * intersection[0][0]); 

		// double noiseZ = perlinNoise(0.1 * intersection[0][2],
		// 	0.1 * intersection[0][0],
		// 	0.1 * intersection[0][1]); 

		//cout << "x: " << noiseX << " y: " << noiseY << " z: " << noiseZ << endl;

		normal[0] = intersection[0] - m_pos;

		//cout << "normal1: " << normal[0] << endl;

		// normal[0][0] = (1.0 - bump) * normal[0][0] + bump*noiseX; 
		// normal[0][1] = (1.0 - bump) * normal[0][1] + bump*noiseY; 
		// normal[0][2] = (1.0 - bump) * normal[0][2] + bump*noiseZ; 

		normal[1] = intersection[1] - m_pos;

		delete roots;
		return true;
	}

	delete roots;
	return false;
}	


bool NonhierCylinder::intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]) {
	double a, b, c;
	double *roots = (double*)malloc(2*sizeof(double));

	Vector3D orgpos = rayOrigin - m_pos;

	Point3D bottom = m_pos;
	Point3D top = m_pos + Vector3D(0, m_height, 0);

	Vector3D AB = (top - bottom);
	Vector3D A0 = rayOrigin - bottom;
	Vector3D A0XAB = A0.cross(AB);
	Vector3D VXAB = rayDir.cross(AB);
	double ab2 = AB.dot(AB);

	a = VXAB.dot(VXAB);
	b = 2*(VXAB.dot(A0XAB));
	c = A0XAB.dot(A0XAB) - (m_radius*m_radius * ab2);

	size_t num_roots = quadraticRoots( a, b, c, roots);
	if (num_roots > 0) {

		if (roots[0] <= roots[1] || num_roots == 1) {
			ret[0] =  roots[0];
			ret[1] = roots[1];
		} else {
			ret[0] = roots[1];
			ret[1] = roots[0];
		}	

		intersection[0] = rayOrigin + ret[0]*rayDir;  
		intersection[1] = rayOrigin + ret[1]*rayDir;  

		Point3D projection = bottom + ((AB.dot(intersection[0] - bottom) / ab2) * AB);
		if ( (projection - bottom).length() + (top - projection).length() > AB.length()) {
			delete roots;
			return false;
		}

		normal[0] = intersection[0] - (m_pos + Vector3D(0, m_height / 2, 0));
		normal[1] = intersection[1] - (m_pos + Vector3D(0, m_height / 2, 0));

		delete roots;
		return true;
	}

	delete roots;
	return false;
}


bool NonhierCone::intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]) {
	double a, b, c;
	double *roots = (double*)malloc(2*sizeof(double));
	//double angleRadians = (m_angle * M_PI) / 180.0; 
	
	// Vector3D coneAxis = Vector3D(0, 1, 0);
	// double AdD = coneAxis.dot(rayDir);
	// Vector3D E = rayOrigin - m_pos;	
	// double AdE = coneAxis.dot(E);
	// double Dde = rayDir.dot(E);
	// double Ede = E.dot(E); 

	// double cosSquared = cos(angleRadians)*cos(angleRadians);
	// Vector3D A = Vector3D(0, 1, 0);
	// double M = A.dot(A) - cosSquared;
	// Vector3D triangle = rayOrigin - m_pos;

	// a = rayDir.dot(M*rayDir);
	// b = 2*rayDir.dot(M*triangle);
	// c = triangle.dot(M*triangle);

	// a = AdD*AdD - cosSquared; 
	// b = 2 * (AdD*AdE - cosSquared*Dde);
	// c = AdE*AdE - cosSquared*Ede;

	double k = m_radius / m_height;
	double yend = m_pos[1] + m_height; 

	a = (rayDir[0]*rayDir[0] + rayDir[2]*rayDir[2] - k*k*rayDir[1]*rayDir[1]);
	b = 2*(rayOrigin[0]*rayDir[0] + rayOrigin[2]*rayDir[2] - k*k*rayOrigin[1]*rayDir[1]);
	c = (rayOrigin[0]*rayOrigin[0] + rayOrigin[2]*rayOrigin[2] - k*k*rayOrigin[1]*rayOrigin[1]);

	size_t num_roots = quadraticRoots( a, b, c, roots);
	if (num_roots > 0) {
		//cout << "hi?" << endl;

		if (roots[0] <= roots[1] || num_roots == 1) {
			ret[0] =  roots[0];
			ret[1] = roots[1];
		} else {
			ret[0] = roots[1];
			ret[1] = roots[0];
		}	

		intersection[0] = rayOrigin + ret[0]*rayDir;  
		intersection[1] = rayOrigin + ret[1]*rayDir;  

		if (intersection[0][1] < 0 || intersection[0][1] > yend) {
			delete roots;
			return false;
		}

		normal[0] = intersection[0] - (m_pos + Vector3D(0, m_height / 2, 0));
		normal[1] = intersection[1] - (m_pos + Vector3D(0, m_height / 2, 0));

		delete roots;
		return true;
	}

	delete roots;
	return false;
}

void Primitive::getColourFromRay(Point3D intersection, CubeMap* m_texture, Colour &colour, double& bump) {

} 

void NonhierSphere::getColourFromRay(Point3D intersection, CubeMap* m_texture, Colour &colour, double& bump) {
	Vector3D vp = intersection - m_pos;
	vp.normalize();
	double u= 0, v = 0;

	u = 0.5 + (atan2(vp[2], vp[0]) / (2* M_PI));	
	v = 0.5 - (asin(vp[1]) / M_PI);

	readTexture(m_texture->front(), u, v, colour);
} 

void NonhierBox::getColourFromRay(Point3D intersectionPoint, CubeMap* m_texture, Colour &colour, double& bump) {
 Point3D centre_pos(m_pos[0] + m_size/2.0, m_pos[1] + m_size/2.0, m_pos[2] + m_size/2.0);
  Vector3D d(intersectionPoint - centre_pos);
  d.normalize();
  double u = 0, v = 0;

  if (abs(d[2]) >= abs(d[0]) && abs(d[2]) >= abs(d[1])) { // front or back face
    if (d[2] > 0) { // back face
    	u = 1 - ((intersectionPoint[0] - m_pos[0]) / m_size);
      v = 1 -  ((intersectionPoint[1] - m_pos[1]) / m_size);
      //cout << "BACK" << endl;
      readTexture(m_texture->front(), u, v, colour);
    } else { // front face
    	u = ((intersectionPoint[0] - m_pos[0]) / m_size);
      v = 1 -  ((intersectionPoint[1] - m_pos[1]) / m_size);

      readTexture(m_texture->back(), u, v, colour);
    }
  } else if (abs(d[1]) >= abs(d[0]) && abs(d[1]) >= abs(d[2])) { // top or bottom face
    if (d[1] > 0) { // top face
    	u = ((intersectionPoint[0] - m_pos[0]) / m_size);
      v = ((intersectionPoint[2] - m_pos[2]) / m_size);

      readTexture(m_texture->bottom(), u, v, colour);
    } else { // bottom face
    	u = ((intersectionPoint[0] - m_pos[0]) / m_size);
      v = 1 - ((intersectionPoint[2] - m_pos[2]) / m_size);

      readTexture(m_texture->top(), u, v, colour);
    }
  } else if (abs(d[0]) >= abs(d[1]) && abs(d[0]) >= abs(d[2])) { // left or right face
    if (d[0] > 0) { // right face
    	u = 1 - ((intersectionPoint[2] - m_pos[2]) / m_size);
      v = 1 - ((intersectionPoint[1] - m_pos[1]) / m_size);

      readTexture(m_texture->right(), u, v, colour);
    } else { // left face
    	u = ((intersectionPoint[2] - m_pos[2]) / m_size);
      v = 1 - ((intersectionPoint[1] - m_pos[1]) / m_size);

      readTexture(m_texture->left(), u, v, colour);
    }
  }	
} 
