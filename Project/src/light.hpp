#ifndef CS488_LIGHT_HPP
#define CS488_LIGHT_HPP

#include "primitive.hpp"
#include "algebra.hpp"
#include <iosfwd>

// Represents a simple point light.
struct Light {
  Light();
  
  Colour colour;
  Point3D position;
  double radius;
  double falloff[3];
};

std::ostream& operator<<(std::ostream& out, const Light& l);

#endif
