#include "scene.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

int SceneNode::cur_id = 0;

SceneNode::SceneNode(const std::string& name)
  : m_name(name) 
{
  m_id = cur_id;
  cur_id++;
}

SceneNode::~SceneNode()
{
}

bool SceneNode::calculateIntersection(Point3D eye, Vector3D rayDir, double &t, Point3D& retPoint, Vector3D& retNormal, Colour &kd, Colour &ks, double &shin) {
  Point3D tempEye = m_invtrans * eye;
  Vector3D tempDir = m_invtrans * rayDir;
  // Point3D tempEye = eye;
  // Vector3D tempDir = rayDir; 
  Vector3D intNormal;
  Point3D intPoint;

  double tempT = 0; double tempShin = 0; 
  Colour tempKd, tempKs, tempKe; 

  bool ret = false; 
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    tempT = 2;
    if ((*ci)->calculateIntersection(tempEye, tempDir, tempT, intPoint, intNormal, tempKd, tempKs, tempShin)) {
      if (tempT < t && tempT > pow(10, -5)) {
        t = tempT;

        kd = tempKd;
        ks = tempKs;
        shin = tempShin;

        retPoint = m_trans * intPoint;
        retNormal = m_invtrans.transpose() * intNormal; 

        retNormal.normalize();
        ret = true;
      }
    }
  }

  return ret;
}

bool GeometryNode::calculateIntersection(Point3D eye, Vector3D rayDir, double &t, Point3D &retPoint, Vector3D& retNormal, Colour &kd, Colour &ks, double &shin) {
  double tempT; 
  Point3D tempEye = m_invtrans * eye;
  Vector3D tempDir = m_invtrans * rayDir;

  Vector3D tempNormal;
  Point3D tempPoint;
  if (m_primitive->intersection(tempEye, tempDir, tempT, tempPoint, tempNormal)) {

    retPoint = m_trans * tempPoint;
    retNormal = m_invtrans.transpose() * tempNormal; 

    kd = m_material->get_kd(); 
    ks = m_material->get_ks();
    shin = m_material->get_shininess();
    
    t = tempT;
    return true;
  }
  return false;
}

void SceneNode::calculateColour(Colour ambiant, Colour diffuse, Colour specular, Colour &ret) {
}

void GeometryNode::calculateColour(Colour ambiant, Colour diffuse, Colour specular, Colour &ret) {
  ret = m_material->apply_gl(ambiant, diffuse, specular);
}

void SceneNode::rotate(char axis, double angle)
{
  std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
    // Fill me in
    Matrix4x4 r;
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
    set_transform(m_trans*r);
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  Matrix4x4 s;
  s[0][0] = amount[0];
  s[1][1] = amount[1];
  s[2][2] = amount[2];

  set_transform(m_trans*s);
}

void SceneNode::translate(const Vector3D& amount)
{
  Matrix4x4 t;
  t[0][3] = amount[0];
  t[1][3] = amount[1];
  t[2][3] = amount[2];
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;

  set_transform(m_trans*t);
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}
