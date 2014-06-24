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

} 

bool NonhierSphere::intersection(Point3D rayOrigin, Vector3D rayDir, Point3D &ret, Vector3D &normal) {
	double a, b, c;

	Vector3D orgpos = rayOrigin - m_pos;
	double *roots = (double*)malloc(3*sizeof(double));

	a = rayDir.dot(rayDir);
	b = 2*(orgpos.dot(rayDir));
	c = orgpos.dot(orgpos) - (m_radius * m_radius); 

	if (quadraticRoots( a, b, c, roots) > 0) {
		double t = 0;

		if (roots[0] < roots[1]) {
			t =  roots[0];
		} else {
			t = roots[1];
		}

		ret = rayOrigin + t*rayDir; 
		normal = (ret - m_pos) / (ret - m_pos).length();
		return true;
	}

	return false;
}	

NonhierBox::~NonhierBox()
{
}
