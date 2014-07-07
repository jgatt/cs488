#include "mesh.hpp"
#include <iostream>

#define BOUNDING_BOX 0

using namespace std;

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
	double s_bound, m_bound;
	s_bound = m_verts[m_faces[0][0]][0];
	m_bound = m_verts[m_faces[0][0]][0];
	for (int i = 0; i < m_faces.size(); i++) {
		for (int j = 0; j < m_faces[i].size(); j++) {
			Point3D temp = m_verts[m_faces[i][j]];
			if (temp[0] < s_bound) {
				s_bound = temp[0];
			}
			if (temp[1] < s_bound) {
				s_bound = temp[1];
			}
			if (temp[2] < s_bound) {
				s_bound = temp[2];
			}

			if (temp[0] > m_bound) {
				m_bound = temp[0];
			}
			if (temp[1] > m_bound) {
				m_bound = temp[1];
			}
			if (temp[2] > m_bound) {
				m_bound = temp[2];
			}
		}
	}
	cout << "s_bound: " << s_bound << " m_bound: " << m_bound << endl;
	bound = new NonhierBox(Point3D(s_bound, s_bound, s_bound), m_bound - s_bound);
}

bool Mesh::intersection(Point3D rayOrigin, Vector3D rayDir, double &ret, Point3D &intersection, Vector3D &normal) {
	if (BOUNDING_BOX) {
		return bound->intersection(rayOrigin, rayDir, ret, intersection, normal);
	} else 
	if (!(bound->intersection(rayOrigin, rayDir, ret, intersection, normal))) {
		return false;
	} 

  double t = 2.0; 
  Vector3D normalFace;
  Point3D intPoint;


  for (int i = 0; i < m_faces.size(); i++) {
    Point3D A = m_verts[m_faces[i][0]];
    Point3D B = m_verts[m_faces[i][1]];
    Point3D C = m_verts[m_faces[i][2]];

    Vector3D AB = B - A;
    Vector3D BC = C - B;
    Vector3D ns = AB.cross(BC);

    if ((rayDir.dot(ns)) == 0) {
      continue;
    }

    double tsolution = ((A - rayOrigin).dot(ns)) / (rayDir.dot(ns));
    if (tsolution > 0 && tsolution < t) {
      intPoint = rayOrigin + tsolution*rayDir; 

      Vector3D nf;
      bool inFace = true;
      for (int j = 0; j < m_faces[i].size(); j++) {
        Vector3D edge = m_verts[m_faces[i][(j + 1) % m_faces[i].size()]] - m_verts[m_faces[i][j]]; 
        nf = ns.cross(edge);

        if (nf.dot(intPoint - m_verts[m_faces[i][j]]) < 0) {
          inFace = false;
        }
      }

      if (inFace) {
        t = tsolution;
        intersection = rayOrigin + t*rayDir;
        normal = ns;
      } 
    }
  }

  if ((t <= 1.0) && t > pow(10, -3)) {
    ret = t;
    return true;
  }

  return false;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}
