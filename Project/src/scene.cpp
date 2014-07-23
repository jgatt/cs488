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

  has_velocity = false;
  intNormal = (Vector3D*)malloc(sizeof(Vector3D)*2);
  intPoint = (Point3D*)malloc(sizeof(Point3D)*2);
  tempT = (double*)malloc(sizeof(double)*2);
}

SceneNode::~SceneNode()
{
  delete intNormal;
  delete intPoint;
  delete tempT;  
}

bool SceneNode::calculateIntersection(Point3D eye, Vector3D rayDir, double timePassed, double t[0], Point3D retPoint[0], Vector3D retNormal[0], Colour &kd, Colour &ks, double &shin) {
  if (has_velocity && timePassed > 0) {
    ray_transform =  m_invtrans * (timePassed*m_invvelocity);
    reverse_ray_transform =  m_trans * (timePassed*m_velocity); 
  } else {
    ray_transform = m_invtrans;
    reverse_ray_transform = m_trans;
  }

  eye_transformed = ray_transform * eye;
  ray_transformed = ray_transform * rayDir;

  double tempShin = 0; 
  Colour tempKd, tempKs, tempKe; 

  bool ret = false; 
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    tempT[0] = 50; tempT[1] = 50;

    if ((*ci)->calculateIntersection(eye_transformed, ray_transformed, timePassed, tempT, intPoint, intNormal, tempKd, tempKs, tempShin)) {
      if (tempT[0] < t[0] && tempT[0] > pow(10, -5)) {
        t[0] = tempT[0];

        kd = tempKd;
        ks = tempKs;
        shin = tempShin;

        retPoint[0] = reverse_ray_transform * intPoint[0];
        retPoint[1] = reverse_ray_transform * intPoint[1];

        retNormal[0] = ray_transform.transpose() * intNormal[0]; 
        retNormal[1] = ray_transform.transpose() * intNormal[1]; 

        retNormal[0].normalize();
        retNormal[1].normalize();
        ret = true;
      }
    }
  }

  return ret;
}

bool GeometryNode::calculateIntersection(Point3D eye, Vector3D rayDir, double timePassed, double t[2], Point3D retPoint[2], Vector3D retNormal[2], Colour &kd, Colour &ks, double &shin) {
  if (has_velocity && timePassed > 0) {
    ray_transform =  m_invtrans * (timePassed*m_invvelocity);
    reverse_ray_transform =  m_trans * (timePassed*m_velocity); 
  } else {
    ray_transform = m_invtrans;
    reverse_ray_transform = m_trans;
  }

  eye_transformed = ray_transform * eye;
  ray_transformed = ray_transform * rayDir;

  if (m_primitive->intersection(eye_transformed, ray_transformed, tempT, intPoint, intNormal)) {
    t[0] = tempT[0];
    t[1] = tempT[1];

    if (has_texture) {
      if (t[0] > 0) {
        Point3D tempPoint = eye + (t[0] - 0.0001)*rayDir;
        double bump = 0;

        m_primitive->getColourFromRay(tempPoint, m_texture, kd, bump); 
        ks = Colour();
        shin = 0;

       //  //if (bump > 0) {
       //    //bump = (rand() % 101) / 100.0;

       // double noiseX = perlinNoise(0.1 * intPoint[0][0],
       //   0.1 * intPoint[0][1],
       //   0.1 * intPoint[0][2]); 

       //  double noiseY = perlinNoise(0.1 * intPoint[0][1],
       //   0.1 * intPoint[0][2],
       //   0.1 * intPoint[0][0]); 

       //  double noiseZ = perlinNoise(0.1 * intPoint[0][2],
       //   0.1 * intPoint[0][0],
       //   0.1 * intPoint[0][1]); 

       //  // cout << "bump: " << bump << endl;
       //  // cout << "x: " << noiseX << " y: " << noiseY << " z: " << noiseZ << endl;

       //  // cout << "normal0: " << intNormal[0] << endl;

       //  intNormal[0][0] = (1.0 - bump) * intNormal[0][0] + bump*noiseX; 
       //  intNormal[0][1] = (1.0 - bump) * intNormal[0][1] + bump*noiseY; 
       //  intNormal[0][2] = (1.0 - bump) * intNormal[0][2] + bump*noiseZ; 

       //  //cout << "normal1: " << intNormal[0] << endl; 
       //  kd = Colour(0.5, 0.5, 0.5);// 1, 1);
      //}
      }
    } else {
      kd = m_material->get_kd(); 
      ks = m_material->get_ks();
      shin = m_material->get_shininess();
    }

    retPoint[0] = reverse_ray_transform * intPoint[0];
    retPoint[1] = reverse_ray_transform * intPoint[1];

    retNormal[0] = ray_transform.transpose() * intNormal[0]; 
    retNormal[1] = ray_transform.transpose() * intNormal[1]; 

    return true;
  }

  return false;
}

bool BooleanNode::calculateIntersection(Point3D eye, Vector3D rayDir, double timePassed, double t[2], Point3D retPoint[2], Vector3D retNormal[2], Colour &kd, Colour &ks, double &shin) {
  if (has_velocity && timePassed > 0) {
    ray_transform =  m_invtrans * (timePassed*m_invvelocity);
    reverse_ray_transform =  m_trans * (timePassed*m_velocity); 
  } else {
    ray_transform = m_invtrans;
    reverse_ray_transform = m_trans;
  }

  eye_transformed = ray_transform * eye;
  ray_transformed = ray_transform * rayDir;

  intersected[0] = false; intersected[1] = false;
  //stuff
  int i = 0;
  for (ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ++ci) {
    tempT[i][0] = 50; tempT[i][1] = 50;

    if ((*ci)->calculateIntersection(eye_transformed, ray_transformed, timePassed, tempT[i], intPoint[i], intNormal[i], kds[i], kss[i], shins[i])) {
      if (tempT[i][0] < t[0] && tempT[i][0] > pow(10, -5)) {
          intersected[i] = true;
      }
    }
    i++;
  }

  bool ret = false;
  if (intersected[0] || intersected[1]) {
    int retIndex = 0;
    int retPrimitive = 0;
    int backIndex = 1;
    int backPrimitive = 1;
    int factor = 1;

    if (m_type == 1) { //union..get first intersection
      ret = true;
      if (intersected[0] && intersected[1]) {
        if (tempT[1][0] < tempT[0][0]) {
          retPrimitive = 1; 
        } 

        if (tempT[1][1] < tempT[0][1]) {
          backPrimitive = 1;
        }

      } else if (intersected[0] && !intersected[1]) {
        backPrimitive = 0;
      } else {
        retPrimitive = 1;
      }
    } else if (m_type == 2) {
      if (intersected[0] && intersected[1]) {

        if (tempT[0][0] < tempT[1][0]) {
          if (tempT[1][0] <= tempT[0][1]) {
            ret = true;
            retPrimitive = 1;
            backPrimitive = 0;
          }

        } else if (tempT[1][0] < tempT[0][0]) {
          if (tempT[0][0] <= tempT[1][1]) {
            ret = true;
          }
        }
      } 

    } else if (m_type == 3) {
      if (intersected[0] && intersected[1]) {
        if (tempT[0][0] < tempT[1][0]) {
          ret = true;

          if (tempT[0][1] < tempT[1][0]) {
            backPrimitive = 0;
          } else if (tempT[1][0] < tempT[0][1]) {
            backIndex = 0;
          } 
        } else {

          if (tempT[1][1] <= tempT[0][1]) {
            retPrimitive = 1;
            retIndex = 1;
            backPrimitive = 0;
            ret = true; 
            factor = -1;
          }
        }
      } else if (intersected[0] && !intersected[1]) {
        ret = true;
        backPrimitive = 0;
      }
    }

    t[0] = tempT[retPrimitive][retIndex];
    t[1] = tempT[backPrimitive][backIndex];

    retPoint[0] = reverse_ray_transform * intPoint[retPrimitive][retIndex];
    retPoint[1] = reverse_ray_transform * intPoint[backPrimitive][backIndex];

    retNormal[0] = factor*ray_transform.transpose() * intNormal[retPrimitive][retIndex];
    retNormal[1] = ray_transform.transpose() * intNormal[backPrimitive][backIndex];

    kd = kds[retPrimitive];
    ks = kss[retPrimitive];
    shin = shins[retPrimitive];
  }

  return ret; 
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

void SceneNode::velocity(const Vector3D& vel) {
  Matrix4x4 t;
  t[0][3] = vel[0];
  t[1][3] = vel[1];
  t[2][3] = vel[2];
  cout << "velocity: " << t << endl;
  has_velocity = true;
  set_velocity(t);
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
  intNormal = (Vector3D*)malloc(sizeof(Vector3D)*2);
  intPoint = (Point3D*)malloc(sizeof(Point3D)*2);
  tempT = (double*)malloc(sizeof(double)*2);
  has_texture = false;
  m_texture = new CubeMap();
}

GeometryNode::~GeometryNode()
{
  delete intNormal;
  delete intPoint;
  delete tempT; 
  delete m_material;
  delete m_texture;
}

BooleanNode::BooleanNode(const std::string& name, int type)
  : SceneNode(name),m_type(type)
{
  intNormal = (Vector3D**)malloc(sizeof(Vector3D*) * 2);
  for (int i = 0; i < 2; i++){
    intNormal[i] = (Vector3D*)malloc(sizeof(Vector3D)*2);
  }

  intPoint = (Point3D**)malloc(sizeof(Point3D*)*2);
  for (int i = 0; i < 2; i++) {
    intPoint[i] = (Point3D*)malloc(sizeof(Point3D)*2);
  }

  tempT = (double**)malloc(sizeof(double*)*2);
  for (int i = 0; i < 2; i++) {
    tempT[i] = (double*)malloc(sizeof(double)*2);
  }

  kds = (Colour*)malloc(sizeof(Colour) * 2);
  kss = (Colour*)malloc(sizeof(Colour) * 2);
  shins = (double*)malloc(sizeof(Colour) * 2);
  intersected = (bool*)malloc(sizeof(bool)*2);
}

BooleanNode::~BooleanNode() {
  for (int i = 0; i < 2; i++) {
    delete intNormal[i];
    delete intPoint[i];
    delete tempT[i];
  }

  delete kds;
  delete kss;
  delete shins;
  delete intNormal;
  delete intPoint;
  delete tempT;
  delete intersected;
}

