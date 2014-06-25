#include "primitive.hpp"
#include "polyroots.hpp"
#include <stdlib.h>
#include <iostream>

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

bool Primitive::intersection(Point3D rayOrigin, Vector3D rayDir, Point3D &ret, Vector3D &normal) {

}

bool Cube::intersection(Point3D rayOrigin, Vector3D rayDir, Point3D &ret, Vector3D &normal) {

} 

bool Sphere::intersection(Point3D rayOrigin, Vector3D rayDir, Point3D &ret, Vector3D &normal) {

}

bool NonhierBox::intersection(Point3D rayOrigin, Vector3D rayDir, Point3D &ret, Vector3D &normal) {
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
	if (rayDir[2] >= 0) {
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

	if ((tmin < 1) && (tmax > 0)) {
		//cout << tmin << " tmax: " << tmax << endl; 
		ret = rayOrigin + tmin*(rayDir);
		normal = ret - centre_pos;
		return true;
	} else {
		return false;
	}
} 

bool NonhierSphere::intersection(Point3D rayOrigin, Vector3D rayDir, Point3D &ret, Vector3D &normal) {
	double a, b, c;

	Vector3D orgpos = rayOrigin - m_pos;
	double *roots = (double*)malloc(3*sizeof(double));

	a = rayDir.dot(rayDir);
	b = 2*(orgpos.dot(rayDir));
	c = orgpos.dot(orgpos) - (m_radius * m_radius); 

	size_t num_roots = quadraticRoots( a, b, c, roots);
	if (num_roots > 0) {
		double t = 0;

		if (roots[0] <= roots[1] || num_roots == 1) {
			t =  roots[0];
		} else {
			t = roots[1];
		}

		ret = rayOrigin + t*rayDir; 
		normal = ret - m_pos;
		return true;
	}

	return false;
}	

NonhierBox::~NonhierBox()
{
}
