#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);
  ~Mesh();

  typedef std::vector<int> Face;
  virtual bool intersection(Point3D rayOrigin, Vector3D rayDir, double ret[2], Point3D intersection[2], Vector3D normal[2]);
  
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  NonhierBox *bound;

  double *tempT;
  Point3D *intPoint;
  Vector3D *intNormal;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
