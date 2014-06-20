#include "scene.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

int SceneNode::cur_id = 0;

SceneNode::SceneNode(const std::string& name)
  : m_name(name), m_picked(false)
{
  m_id = cur_id;
  cur_id++;
  stack_pointer = 0;
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
  glPushMatrix();

  glMultMatrixd(m_trans.multMatrix());
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->walk_gl(picking);
  }
  glPopMatrix();
}

bool SceneNode::flag_picked(int name, bool childOfJoint) {
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->flag_picked(name, false);
  } 

  return false;
}

bool JointNode::flag_picked(int name, bool childOfJoint) {
  bool child_picked = false;
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    bool temp = (*ci)->flag_picked(name, true);
    if (temp == true) {
      child_picked = true;
    } 
  } 

  m_picked = child_picked;
  return false;
}

bool GeometryNode::flag_picked(int name, bool childOfJoint) {
  if (name == m_id && childOfJoint) {
    m_picked = !m_picked;
  }
  return m_picked;
}

void SceneNode::animate(int diff, bool rotateHead) {
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->animate(diff, rotateHead);
  }
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
    m_trans = m_trans*r;
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  Matrix4x4 s;
  s[0][0] = amount[0];
  s[1][1] = amount[1];
  s[2][2] = amount[2];

  m_trans = m_trans*s;
}

void SceneNode::translate(const Vector3D& amount)
{
  Matrix4x4 t;
  t[0][3] = amount[0];
  t[1][3] = amount[1];
  t[2][3] = amount[2];
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;

  m_trans = m_trans*t; 
}

bool SceneNode::is_joint() const
{
  return false;
}

void SceneNode::saveChange() {
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->saveChange();
  }
}

void JointNode::saveChange() {
  while (stack_pointer < undoStack.size() - 1 && undoStack.size() > 1) {
    undoStack.pop_back();
  }

  struct undoObject temp;
  temp.m_trans = m_trans;
  temp.rotateX = rotateAngleX;
  temp.rotateY = rotateAngleY;

  undoStack.push_back(temp);

  stack_pointer = undoStack.size() - 1;
  SceneNode::saveChange();
}

void SceneNode::undoChange() {
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->undoChange();
  }
}

void JointNode::undoChange() {
  if (stack_pointer > 0) {
    stack_pointer -= 1;
    m_trans = undoStack[stack_pointer].m_trans;
    rotateAngleX = undoStack[stack_pointer].rotateX;
    rotateAngleY = undoStack[stack_pointer].rotateY;
  }
  SceneNode::undoChange();
}

void SceneNode::redoChange() {
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->redoChange();
  }
}

void JointNode::reset_joints() {
  while (undoStack.size() > 1) {
    undoStack.pop_back();
  }

  stack_pointer = 0;
  m_trans = undoStack[stack_pointer].m_trans;
  rotateAngleX = undoStack[stack_pointer].rotateX;
  rotateAngleY = undoStack[stack_pointer].rotateY;
  
  SceneNode::reset_joints();
}

void SceneNode::reset_joints() {
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->reset_joints();
  }
}

void SceneNode::reset(Matrix4x4 reset) {
  m_trans = reset; 
}

void JointNode::redoChange() {
  if (stack_pointer < undoStack.size() -1) {
    stack_pointer += 1;
    m_trans = undoStack[stack_pointer].m_trans;
    rotateAngleX = undoStack[stack_pointer].rotateX;
    rotateAngleY = undoStack[stack_pointer].rotateY;
  }
  SceneNode::redoChange();
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
  glPushMatrix();

  glMultMatrixd(m_trans.multMatrix());
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    (*ci)->walk_gl(picking);
  }

  glPopMatrix();
  // Fill me in
}

void JointNode::animate(int diff, bool rotateHead) {
  if (m_picked) {
    if (rotateHead && m_name == "Neck top") {
      if (rotateAngleY + diff > m_joint_y.min && rotateAngleY + diff < m_joint_y.max) {
        rotateAngleY += diff;
        rotate('y', diff);
      }
    } else if (!rotateHead) {
      if (rotateAngleX + diff > m_joint_x.min && rotateAngleX + diff < m_joint_x.max) {
        rotateAngleX += diff;
        rotate('x', diff);
      }
    }
  }
  SceneNode::animate(diff, rotateHead);
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
  rotateAngleX = init;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
  rotateAngleY = init;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking) const
{
  GLUquadricObj *sphere = gluNewQuadric();
  gluQuadricDrawStyle( sphere, GLU_FILL);
  gluQuadricNormals( sphere, GLU_SMOOTH);
  gluQuadricOrientation( sphere, GLU_OUTSIDE);

  glPushMatrix();
  glPushName(m_id);

  glMultMatrixd(m_trans.multMatrix());

  m_material->apply_gl(m_picked);
  gluSphere( sphere, 1.0005, 32, 32);

  glPopName();
  glPopMatrix();
}
 
