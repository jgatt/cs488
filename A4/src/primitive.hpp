#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual bool intersection(Point3D, Vector3D, Point3D&, Vector3D &normal);
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual bool intersection(Point3D, Vector3D, Point3D&, Vector3D &normal);
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual bool intersection(Point3D, Vector3D, Point3D&, Vector3D &normal);
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius), trans_pos(pos)
  {
  }
  virtual ~NonhierSphere();
  virtual bool intersection(Point3D rayOrigin, Vector3D rayDir, Point3D &ret, Vector3D &normal);

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
  virtual bool intersection(Point3D, Vector3D, Point3D&, Vector3D &normal);

private:
  Point3D m_pos;
  double m_size;
};

#endif
