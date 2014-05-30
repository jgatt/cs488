#include "a2.hpp"

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis)
{
  Matrix4x4 r;
  // Fill me in!
  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
  Matrix4x4 t;
	
  t[3] = displacement[0];
  t[7] = displacement[1]; 
  t[11] = displacement[2]; 

  // Fill me in!
  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
  Matrix4x4 s;
  s[0] = scale[0];
  s[5] = scale[1];
  s[10] = scale[2];
  // Fill me in!
  return s;
}
