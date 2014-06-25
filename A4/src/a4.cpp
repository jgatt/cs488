#include "a4.hpp"
#include "image.hpp"
#include <iostream>

using namespace std;

void cast_ray(double x, double y) {

  //rayImage.translate(); 

}

Matrix4x4 translateMatrix(Vector3D d) {
  Matrix4x4 t;

  t[0][3] = d[0];
  t[1][3] = d[1];
  t[2][3] = d[2];

  return t; 
}

Matrix4x4 scaleMatrix(Vector3D s) {
  Matrix4x4 t;

  t[0][0] = s[0];
  t[1][1] = s[1];
  t[2][2] = s[2];

  return t; 
}

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               )
{
  // Fill in raytracing code here.

  std::cerr << "Stub: a4_render(" << root << ",\n     "
            << "fname: " << filename << ", width: " << width << ", height: " << height << ",\n"
            << " eye: " << eye << ", view: " << view << ", up: " << up << ", fov: " << fov << ",\n     "
            << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;
  
  Image img(width, height, 3);

  double worldWidth, worldHeight, d;
  d = 500.0;
  worldHeight = 2*d*tan(((fov/2.0) * M_PI) / 180.0); 
  worldWidth = ((double)width / (double)height) * worldHeight;

  cout << "ww: " << worldWidth  << " wh: " << worldHeight << endl;
  SceneNode* temp;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      Point3D rayImage = Point3D(x, y, 0);  

      rayImage[0] += (-width / 2);
      rayImage[1] += (-height / 2);
      rayImage[2] += d;

      rayImage[0] *= (-worldHeight / height); 
      rayImage[1] *= (worldWidth / width); 

      Vector3D u, v, w;

      w = view;
      u = w.cross(up);
      v = u.cross(w);

      Matrix4x4 r3 = Matrix4x4(Vector4D(u[0], v[0], w[0], 0),
        Vector4D(u[1], v[1], w[1], 0), Vector4D(u[2], v[2], w[2], 0), Vector4D(0, 0, 0, 1));

      rayImage = r3 * rayImage;

      rayImage[0] += eye[0]; 
      rayImage[1] += eye[1]; 
      rayImage[2] += eye[2]; 

      Vector3D rayDir = rayImage - eye; 

      //cout << rayDir << endl;
      Point3D intPoint; 
      Vector3D normal;
      Colour pixel = Colour(0);
      temp = root->calculateIntersection(eye, rayDir, intPoint, normal); 
      if (intPoint[0] != 0 || intPoint[1] != 0 || intPoint[2] != 0) {
        //cout << intPoint << endl;

        //check lights
        for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
          Vector3D lightVect = ((*I)->position - intPoint);
          Vector3D viewDir = eye - intPoint;

          viewDir.normalize();
          double distance = lightVect.length();
          lightVect.normalize();

          Colour iA = ambient; 
          double attenuation = 1 / ((*I)->falloff[0] + ((*I)->falloff[1] * distance) + ((*I)->falloff[2] * (distance * distance))); 
          Colour iD = attenuation * (*I)->colour; 
          Colour iS = iD;

          double ndotlight = normal.dot(lightVect);
          if (ndotlight <= 0) {
            ndotlight = 0;
          } else if (ndotlight >= 1) {
            ndotlight = 1;
          }

          Colour diffuse = ndotlight * iD; 

          Vector3D halfAngle = lightVect + viewDir;
          halfAngle.normalize();
          double ndoth = normal.dot(halfAngle);

          if (ndoth <= 0) {
            ndoth = 0;
          } else if (ndoth >= 1) {
            ndoth = 1;
          }

          //don't cast ray or w/e
          if (ndotlight <= 0) {
            ndoth = 0;
          }

          Colour specular = pow(ndoth, 20) * iS;
          temp->calculateColour(iA, diffuse, specular, pixel); 
        }
      }
      img(x, (height - y), 0) = pixel.R();
      img(x, (height - y), 1) = pixel.G();
      img(x, (height - y), 2) = pixel.B();
    }
  }
  img.savePng(filename);
  
}
