#include "material.hpp"
#include <iostream>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
	paramskd = (GLfloat*)malloc(4*sizeof(GLfloat));
	paramsks = (GLfloat*)malloc(4*sizeof(GLfloat));
	paramspicked = (GLfloat*)malloc(4*sizeof(GLfloat));

	paramskd[0] = kd.R(); 	
	paramskd[1] = kd.G(); 	
	paramskd[2] = kd.B();	
	paramskd[3] = 1; 

	paramsks[0] = ks.R(); 	
	paramsks[1] = ks.G(); 	
	paramsks[2] = kd.B();	
	paramsks[3] = 1; 

	paramspicked[0] = 1; 
	paramspicked[1] = 0.5; 
	paramspicked[2] = 0; 
	paramspicked[3] = 1; 
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl(bool picked) const
{
  	// Perform OpenGL calls necessary to set up this material.
	if (picked) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, paramspicked);
	} else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, paramskd);
	}
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, paramsks);
}

