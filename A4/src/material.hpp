#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"

class Material {
public:
  virtual ~Material();
  virtual Colour apply_gl(Colour, Colour, Colour) const = 0;

  const virtual Colour& get_kd() const { return 0; }
  const virtual Colour& get_ks() const { return 0; }
  const virtual double get_shininess() const { return 0; }

protected:
  Material()
  {
  }

  
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
  virtual ~PhongMaterial();

  virtual Colour apply_gl(Colour, Colour, Colour) const;

  const virtual Colour& get_kd() const { return m_kd; }
  const virtual Colour& get_ks() const { return m_ks; }
  const virtual double get_shininess() const { return m_shininess; }

private:
  Colour m_ks;
  Colour m_kd; 
  double m_shininess;
  
};


#endif
