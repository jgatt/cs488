#include "a4.hpp"
#include "image.hpp"
#include <iostream>

#define REFLECTION           0
#define ALIAS_STRATA        4
#define TIME_STRATA         1

using namespace std;

Colour directLight(Point3D intersection, Vector3D normal, Light *light, SceneNode *root, double timePassed, Colour kd2) {
  Vector3D lightVect = light->position - intersection; 
  Colour kd, ks;

  Vector3D *tempNormal = (Vector3D*)malloc(sizeof(Vector3D)*2);
  Point3D *intPoint = (Point3D*)malloc(sizeof(Point3D)*2);
  double *t = (double*)malloc(sizeof(double)*2);
  t[0] = 2; t[1] = 2;
  
  double shin = 0;
  double distance = lightVect.length();

  if (root->calculateIntersection(intersection, lightVect, timePassed, t, intPoint, tempNormal, kd, ks, shin)) {
    if (t[0] >= 0 && t[0] <= 1) {
      if (! ((intersection - intPoint[0]).length() > distance)) {

        delete tempNormal;
        delete intPoint;
        delete t;

        return Colour(0, 0, 0);
      }
    }
  }

    lightVect.normalize();

    double attenuation = 1 / (light->falloff[0] + (light->falloff[1] * distance) + (light->falloff[2] * (distance * distance))); 
    Colour iD = attenuation * light->colour; 

    double ndotlight = normal.dot(lightVect);
    
    if (ndotlight <= 0) {
      ndotlight = 0;
    } else if (ndotlight >= 1) {
      ndotlight = 1;
    } 

    delete tempNormal;
    delete intPoint;
    delete t;

    return ndotlight * iD;
}

Colour ray_colour(Point3D rayOrigin, Vector3D rayDir, Light *light, SceneNode *root, Colour ambient, Colour background, double timePassed, int numBounces) {
  Colour colour;
  Colour kd, ks;

  Vector3D *normal = (Vector3D*)malloc(sizeof(Vector3D)*2);
  Point3D *intPoint = (Point3D*)malloc(sizeof(Point3D)*2);
  double *t = (double*)malloc(sizeof(double)*2);
  t[0] = 2; t[1] = 2;

  double shin = 0;

  if (root->calculateIntersection(rayOrigin, rayDir, timePassed, t, intPoint, normal, kd, ks, shin)) {
    if (t[0] >= 0.0001 && t[0] <= 1) {

      Vector3D viewDir = rayOrigin - intPoint[0];
      viewDir.normalize();

      Vector3D lightVect = light->position - intPoint[0];
      double distance = lightVect.length();

      Colour iA = ambient; 
      double attenuation = 1 / (light->falloff[0] + (light->falloff[1] * distance) + (light->falloff[2] * (distance * distance))); 
      Colour iD = attenuation * light->colour; 
      Colour iS = iD; 

      double ndotlight = normal[0].dot(lightVect);
      if (ndotlight <= 0) {
        ndotlight = 0;
      } else if (ndotlight >= 1) {
        ndotlight = 1;
      }

      Colour diffuse = ndotlight * iD; 

      Vector3D halfAngle = lightVect + viewDir;
      halfAngle.normalize();
      double ndoth = normal[0].dot(halfAngle);

      if (ndoth <= 0) {
        ndoth = 0;
      } else if (ndoth >= 1) {
        ndoth = 1;
      }

      //don't cast ray or w/e
      if (ndotlight <= 0) {
        ndoth = 0;
      }

      Colour specular = pow(ndoth, shin) * iS; 

      colour = ambient*kd; 

      if (kd.R() != 0 || kd.B() != 0 || kd.G() != 0) { //has k
        colour = colour + directLight(intPoint[0], normal[0], light, root, timePassed, kd) * kd;
      } 

      if (shin > 0) { //has k
    	if (REFLECTION && numBounces < 10) {
    		Vector3D reflection = rayDir - 2*rayDir.dot(normal[0])*normal[0];
            	colour = colour + 0.4*ks*iS*ray_colour(intPoint[0], reflection, light, root, ambient, background, timePassed, numBounces+1);
    	} else {
    		colour = colour + specular*ks*iS;
    	}
      } 

    } else {
      colour = background; 
    }
  } else {
    colour =  background;
  }

  delete normal;
  delete intPoint;
  delete t; 

  return colour;
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

void generateStrata(int width, int height, double worldWidth, double worldHeight, double d, const Vector3D& view, const Vector3D& up, const Point3D& eye, Point3D& rayImage) {
    rayImage[0] += (-width / 2);
    rayImage[1] += (-height / 2);
    rayImage[2] += d;

    rayImage[0] *= (worldWidth / width); 
    rayImage[1] *= (worldHeight / height); 

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
  Image img(width, height, 3);

  double worldWidth, worldHeight, d;
  d = 1500.0;
  worldHeight = 2*d*tan(((fov/2.0) * M_PI) / 180.0); 
  worldWidth = ((double)width / (double)height) * worldHeight;

  double timePassed = 0;
  for (int y = 0; y < height; y++) {
    cout << 100 * ((double)y / (double)height) << " % complete" << endl;
    for (int x = 0; x < width; x++) {
      Colour pixel = Colour(0);
      Colour background = Colour(0, 0, y / (double)width); 

      for (int i = 0; i < ALIAS_STRATA; i++) {
        for (int j = 0; j < ALIAS_STRATA; j++) {

         double strataX = (25*i + (rand() % 26)) / 100.0; // i = 0; i+1 = 1; 
         double strataY = (25*j + (rand() % 26)) / 100.0; // i = 0; i+1 = 1; 
          Point3D rayImage = Point3D(x + strataX, (height - y) + strataY, 0);
          Vector3D rayDir;
          generateStrata(width, height, worldWidth, worldHeight, d, view, up, eye, rayImage);
          rayDir = rayImage - eye;

          timePassed+= 0.5;
          for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
            pixel = pixel + ray_colour(eye, rayDir, (*I), root, ambient, background, timePassed, 0);
          }
        }
      } 

      pixel = (1/ (double)(ALIAS_STRATA*ALIAS_STRATA)) * pixel;

      img(x, y, 0) = pixel.R();
      img(x, y, 1) = pixel.G();
      img(x, y, 2) = pixel.B();
    }
  }
  img.savePng(filename);
  
}
