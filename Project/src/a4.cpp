#include "a4.hpp"
#include "image.hpp"
#include "CubeMap.hpp"
#include <iostream>

#define REFLECTION            0
#define ALIAS_STRATA       5
#define SHADOW_STRATA   1
#define DEPTH_OF_FIELD    0

using namespace std;

CubeMap *backgroundMap;

Colour directLight(Point3D intersection, Vector3D normal, Light *light, SceneNode *root, double timePassed) {
  Vector3D lightVect; 
  Colour kd, ks;

  Vector3D *tempNormal = (Vector3D*)malloc(sizeof(Vector3D)*2);
  Point3D *intPoint = (Point3D*)malloc(sizeof(Point3D)*2);
  double *t = (double*)malloc(sizeof(double)*2);
  t[0] = 50; t[1] = 50;
  
  double shin = 0;
  double distance = 0; 
  double strataX = 0;
  double strataZ = 0;
  Colour shadow;

  double startingX = light->position[0] - light->radius;
  double startingZ = light->position[2] - light->radius;
  double factor = (light->radius + light->radius)  / (double)(SHADOW_STRATA * SHADOW_STRATA);
  factor *= 100;

  int ints = 0;
  double retval = 0;

  double shadow_samples = SHADOW_STRATA * SHADOW_STRATA;

  for (int i = 0; i < SHADOW_STRATA; i++) {
    for (int j = 0; j < SHADOW_STRATA; j++) {
      t[0] = 50; t[1] = 50;
      strataX = (factor*i + (rand() % (int)(factor+1))) / 100.0;
      strataZ = (factor*j + (rand() % (int)(factor+1))) / 100.0;

      lightVect = Point3D(startingX + strataX, light->position[1], startingZ + strataZ) - intersection; 

      shin = 0;
      distance = lightVect.length();

      if (root->calculateIntersection(intersection, lightVect, timePassed, t, intPoint, tempNormal, kd, ks, shin)) {
        if (t[0] >= 0) {// && t[0] <= 1) {
          if (! ((intersection - intPoint[0]).length() > distance)) {
            continue;
          }
        }
      } else { 
        lightVect.normalize();

        double attenuation = 1 / (light->falloff[0] + (light->falloff[1] * distance) + (light->falloff[2] * (distance * distance))); 
        Colour iD = attenuation * light->colour; 

        double ndotlight = normal.dot(lightVect);
        
        if (ndotlight <= 0) {
          ndotlight = 0;
        } else if (ndotlight >= 1) {
          ndotlight = 1;
        }

        shadow = shadow + (ndotlight * iD);
      }
    }
  }

  shadow = (1 / shadow_samples) * shadow; 

  delete tempNormal;
  delete intPoint;
  delete t;

  return shadow;
}

Colour ray_colour(Point3D rayOrigin, Vector3D rayDir, Light *light, SceneNode *root, Colour ambient, Colour background, double timePassed, int numBounces) {
  Colour colour;
  Colour kd, ks;

  Vector3D *normal = (Vector3D*)malloc(sizeof(Vector3D)*2);
  Point3D *intPoint = (Point3D*)malloc(sizeof(Point3D)*2);
  double *t = (double*)malloc(sizeof(double)*2);
  t[0] = 50; t[1] = 50;

  double shin = 0;

  if (root->calculateIntersection(rayOrigin, rayDir, timePassed, t, intPoint, normal, kd, ks, shin)) {
    if (t[0] >= 0.0001) { //} && t[0] <= 1) {

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

      colour = iA*kd; 

      if (kd.R() != 0 || kd.B() != 0 || kd.G() != 0) { //has k
        colour = colour + directLight(intPoint[0], normal[0], light, root, timePassed) * kd;
      } 

      if (shin > 0) { //has k
    	if (REFLECTION && numBounces < 10) {
    		Vector3D reflection = rayDir - 2*rayDir.dot(normal[0])*normal[0];
            	colour = colour + (shin/100.0)*ks*iS*ray_colour(intPoint[0], reflection, light, root, ambient, background, timePassed, numBounces+1);
    	} else {
    		colour = colour + specular*ks*iS;
    	}
      } 

    } else {
      colour =  background;
      //readCubeMap(rayDir, backgroundMap, colour); 
    }
  } else {
    //readCubeMap(rayDir, backgroundMap, colour); 
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

void generateStrata(int width, int height, double worldWidth, double worldHeight, double d, const Matrix4x4& r3, const Point3D& eye, Point3D& rayImage) {
    rayImage[0] += (-width / 2);
    rayImage[1] += (-height / 2);
    rayImage[2] += d;

    rayImage[0] *= (worldWidth / width); 
    rayImage[1] *= (worldHeight / height); 

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
  backgroundMap = new CubeMap();
  Image temp;
  // temp.loadPng("posz.png");
  // backgroundMap->set_face(0, temp);
  // temp.loadPng("posx.png");
  // backgroundMap->set_face(1, temp);
  // temp.loadPng("negx.png");
  // backgroundMap->set_face(2, temp);
  // temp.loadPng("negz.png");
  // backgroundMap->set_face(3, temp);
  // temp.loadPng("posy.png");
  // backgroundMap->set_face(4, temp);
  // temp.loadPng("negy.png");
  // backgroundMap->set_face(5, temp);

  double worldWidth, worldHeight, d;
  d = 50;
  worldHeight = 2*d*tan(((fov/2.0) * M_PI) / 180.0); 
  worldWidth = ((double)width / (double)height) * worldHeight;

  double strataX = 0;
  double strataY = 0;

  int samples = ALIAS_STRATA * ALIAS_STRATA;
  //pthread_t * threads = malloc(sizeof(pthread_t) * samples);
  Vector3D rayDir;
  Point3D rayImage;
  Point3D pointAimed;
  Point3D lenPos;

  int focal = 5;
  Vector3D u, v, w;

  w = view;
  w.normalize();
  u = w.cross(up);
  v = u.cross(w);

  double apertureRadius = 0.5;

  Matrix4x4 r3 = Matrix4x4(Vector4D(u[0], v[0], w[0], 0),
    Vector4D(u[1], v[1], w[1], 0), Vector4D(u[2], v[2], w[2], 0), Vector4D(0, 0, 0, 1));

  u.normalize();
  v.normalize();

  Vector3D xAperture = apertureRadius * u; 
  Vector3D yAperture = apertureRadius * v; 

  double factor = (1.0)  / (double)(samples);
  factor *= 100;

  for (int y = 0; y < height; y++) {
    cout << 100 * ((double)y / (double)height) << " % complete" << endl;
    for (int x = 0; x < width; x++) {
      Colour pixel = Colour(0);
      Colour background = Colour(0, 0, y / (double)width); 

      double timePassed = 0;
      int k = 0;

      rayImage = Point3D(x + 0.5, (height - y) + 0.5, 0);
      generateStrata(width, height, worldWidth, worldHeight, d, r3, eye, rayImage);

      for (int i = 0; i < ALIAS_STRATA; i++) {
        for (int j = 0; j < ALIAS_STRATA; j++) {
            Point3D newPos = eye; 
            if (DEPTH_OF_FIELD) {
              strataX = ((rand() % 201) - 100.0) / 100.0;
              strataY = ((rand() % 201) - 100.0) / 100.0; 
              newPos = newPos + strataX*xAperture + strataY*yAperture;
            } else {
              strataX = (factor*i + (rand() % (int)(factor + 1))) / 100.0; // i = 0; i+1 = 1; 
              strataY = (factor*j + (rand() % (int)(factor + 1))) / 100.0; // i = 0; i+1 = 1; 
              rayImage = Point3D(x + strataX, (height - y) + strataY, 0);
              generateStrata(width, height, worldWidth, worldHeight, d, r3, newPos, rayImage); 
            }
            
            rayDir = rayImage - newPos;

          timePassed = (10*k + (rand() % 11)) / 100.0;
          for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
            //int ret = pthread_create(&threads[i], NULL, &ray_colour, eye, rayDir, (*I), root, ambient, background, timePassed, 0);
            pixel = pixel + ray_colour(newPos, rayDir, (*I), root, ambient, background, timePassed, 0);
          }
          k++;
        }
      } 

      // for (int i = 0; i < samples; i++) {
      //   int ret = 0; 
      //   pthread_join(threads[i], &ret);
      // }

      pixel = (1/ (double)(samples)) * pixel;

      img(x, y, 0) = pixel.R();
      img(x, y, 1) = pixel.G();
      img(x, y, 2) = pixel.B();
    }
  }
  img.savePng(filename);
  
}
