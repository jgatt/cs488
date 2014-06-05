#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "algebra.hpp"
#include <vector>

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer();
  virtual ~Viewer();

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();

  // *** Fill in these functions (in viewer.cpp) ***

  // Set the parameters of the current perspective projection using
  // the semantics of gluPerspective().
  void set_perspective(double fov, double aspect,
                       double near, double far);

  // Restore all the transforms and perspective parameters to their
  // original state. Set the viewport to its initial size.
  void reset_view();

  //MY STUFF  
  void setMode(int); 

protected:

  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.

  // Called when GL is first initialized
  virtual void on_realize();
  // Called when our window needs to be redrawn
  virtual bool on_expose_event(GdkEventExpose* event);
  // Called when the window is resized
  virtual bool on_configure_event(GdkEventConfigure* event);
  // Called when a mouse button is pressed
  virtual bool on_button_press_event(GdkEventButton* event);
  // Called when a mouse button is released
  virtual bool on_button_release_event(GdkEventButton* event);
  // Called when the mouse moves
  virtual bool on_motion_notify_event(GdkEventMotion* event);

private:

  void drawPlane(Matrix4x4);
  void userTransform(int); 
  std::vector<Point2D> transformPoints(Point3D[]);
  int clipInPlace(Point3D &A, Point3D &B);

  // *** Fill me in ***
  // You will want to declare some more matrices here
  Matrix4x4 m_projection;
  Matrix4x4 m_world;

  Matrix4x4 m_model;
  Matrix4x4 m_model_scaling;
  Matrix4x4 m_model_translation;
  Matrix4x4 m_model_rotation;
  Matrix4x4 m_model_rotations[3];

  Matrix4x4 m_viewing;
  Matrix4x4 m_viewing_translation;
  Matrix4x4 m_viewing_rotation;
  Matrix4x4 m_viewing_rotations[3];

  //Vectors and Points
  Point3D world_gnome;
  
  Point3D m_box[6][4];   
};

#endif
