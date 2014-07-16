#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual bool intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]);
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual bool intersection(Point3D rayOrigin, Vector3D rayDir, double &ret, Point3D &intersection, Vector3D &normal);
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual bool intersection(Point3D rayOrigin, Vector3D rayDir, double &ret, Point3D &intersection, Vector3D &normal);
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius), trans_pos(pos)
  {
  }
  virtual ~NonhierSphere();
  virtual bool intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]);

private:
  Point3D m_pos;
  Point3D trans_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();
  virtual bool intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]);

private:
  Point3D m_pos;
  double m_size;
};

#endif
