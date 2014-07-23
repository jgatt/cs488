#include "helpers.hpp"

using namespace std;

void readTexture(Image *m_texture, double u, double v, Colour &colour) {
 u = abs(u);
 v = abs(v);

  int sizeU = m_texture->width();
  int sizeV = m_texture->height();

  int umin = (sizeU * u);
  int vmin = (sizeV * v);
  int umax = (sizeU * u) + 1;
  int vmax = (sizeV * v) + 1;
  double ucoef = abs(sizeU * u - umin);
  double vcoef = abs(sizeV * v - vmin);

  // clamping
  umin = min(max(umin, 0), sizeU - 1);
  umax = min(max(umax, 0), sizeU - 1);
  vmin = min(max(vmin, 0), sizeV - 1);
  vmax = min(max(vmax, 0), sizeV - 1);

  // defining colours
  Colour p0((*m_texture)(umin, vmin, 0), (*m_texture)(umin, vmin, 1), (*m_texture)(umin, vmin, 2)); // top left
  Colour p2((*m_texture)(umin, vmax, 0), (*m_texture)(umin, vmax, 1), (*m_texture)(umin, vmax, 2)); // top right
  Colour p1((*m_texture)(umax, vmin, 0), (*m_texture)(umax, vmin, 1), (*m_texture)(umax, vmin, 2)); // bottom left
  Colour p3((*m_texture)(umax, vmax, 0), (*m_texture)(umax, vmax, 1), (*m_texture)(umax, vmax, 2)); // bottom right

  // bilinear interpolation
  colour = ((1 - ucoef) * p0 + ucoef * p1) * (1 - vcoef) + ((1 - ucoef) * p2 + ucoef * p3) * vcoef;
}

void readCubeMap(Vector3D vp, CubeMap* texture, Colour &colour) {

}
