#include "a2.hpp"

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis)
{
  Matrix4x4 r;
  // Fill me in!
  switch(axis) {
  	case 'z':
  		r[0][0] = cos((angle * M_PI) / 180.0);
  		r[0][1] = -sin((angle * M_PI) / 180.0);
  		r[1][0] = sin((angle * M_PI) / 180.0);
  		r[1][1] = cos((angle * M_PI) / 180.0);
  		break;
  	case 'x':
  		r[1][1] = cos((angle * M_PI) / 180.0);
  		r[1][2] = -sin((angle * M_PI) / 180.0);
  		r[2][1] = sin((angle * M_PI) / 180.0);
  		r[2][2] = cos((angle * M_PI) / 180.0);
  		break;
  	case 'y':
		r[0][0] = cos((angle * M_PI) / 180.0);
  		r[0][2] = sin((angle * M_PI) / 180.0);
  		r[2][0] = -sin((angle * M_PI) / 180.0);
  		r[2][2] = cos((angle * M_PI) / 180.0);
  		break;
  	default:
  		break;
  }

  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
  Matrix4x4 t;
  t[0][3] = displacement[0]; 	
  t[1][3] = displacement[1];
  t[2][3] = displacement[2]; 

  // Fill me in!
  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
  Matrix4x4 s;
  s[0][0] = scale[0];
  s[1][1] = scale[1];
  s[2][2] = scale[2];
  // Fill me in!
  return s;
}

Matrix4x4 perspective(double angle, double aspect, double near, double far) {
  Matrix4x4 p;

  double f =  1 / (tan(((angle/2.0) * M_PI) / 180.0));

  p[0][0] = f / aspect;
  p[1][1] = f;
  p[2][2] = (far + near) / (near - far);
  p[2][3] = (2*far*near) / (near - far);
  p[3][2] = -1;
  p[3][3] = 0;

  return p; 
}