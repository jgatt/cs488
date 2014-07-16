#include "material.hpp"
#include <iostream>

using namespace std;

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}



Colour PhongMaterial::apply_gl(Colour amb, Colour diff, Colour spec) const {
	Colour temp = amb*m_kd + diff*m_kd + spec*m_ks;	
	return temp;
}
