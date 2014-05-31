#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "draw.hpp"
#include "a2.hpp"

using namespace std;

double prevMouse = 0;
int button_pressed = 0;

int mode = 0;

Viewer::Viewer()
{
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);


  world_gnome[0] = 1;
  world_gnome[1] = 1;
  world_gnome[2] = 1; //set up at points 1,1,1?

  m_box.push_back(Point3D(-1, -1, 0)); 
  m_box.push_back(Point3D(-1, 1, 0)); 
  m_box.push_back(Point3D(1, 1, 0)); 
  m_box.push_back(Point3D(1, -1, 0)); 

  //set up initial model transforms
  m_model = translation(Vector3D(150, 150, 0)) * scaling(Vector3D(100, 100, 100)) * m_model;   
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
}

void Viewer::set_perspective(double fov, double aspect,
                             double near, double far)
{
  // Fill me in!
}

void Viewer::reset_view()
{
  // Fill me in!
}

void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();
  
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  
  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  gldrawable->gl_end();
}

void drawPlane(Matrix4x4 d) {
  //draw_line();
  //draw_line();
}

void Viewer::setMode(int m) {
  mode = m; 
}

void Viewer::userTransform(int diff) {
  Matrix4x4 m_transform;
  Vector3D v_transform(0, 0, 0);
  switch(mode) {
    case 0:
      //FOR NOW VIEW STUFF 
      v_transform[button_pressed - 1] = diff;
      m_transform = translation(v_transform);
      m_viewing = m_transform * m_viewing;
      break;
    case 1: 
      v_transform[button_pressed - 1] = diff;
      m_transform = translation(v_transform);
      m_model = m_transform * m_model;
      break;
    default: 
      break;
  }

  invalidate();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Here is where your drawing code should go.
  
  draw_init(get_width(), get_height());
  
  /* A few of lines are drawn below to show how it's done. */

  set_colour(Colour(0.1, 0.1, 0.1));

  //draw the viewport
  
 // draw_line(Point2D(0.05*get_width(), 0.05*get_height()), 
 //    Point2D(0.05*get_width(), 0.95*get_height())); 
 // draw_line(Point2D(0.95*get_width(), 0.05*get_height()), 
 //    Point2D(0.95*get_width(), 0.95*get_height())); 

 // draw_line(Point2D(0.05*get_width(), 0.05*get_height()), 
 //    Point2D(0.95*get_width(), 0.05*get_height())); 
 // draw_line(Point2D(0.05*get_width(), 0.95*get_height()), 
 //    Point2D(0.95*get_width(), 0.95*get_height())); 

  //Set up the points TEMPORARY
  m_box[0] = Point3D(-1, -1, 0);
  m_box[1] = Point3D(-1, 1, 0); 
  m_box[2] = Point3D(1, 1, 0); 
  m_box[3] = Point3D(1, -1, 0); 

  //MODEL TRANSFORMATIONS

  //View Transformations 
  cout << m_viewing << endl;
  //Calculate inverse of view transform
  Matrix4x4 i_view = m_viewing.invert(); 

  //Transform all dem points
  for (int i = 0; i < m_box.size(); i++) {
    m_box[i] = i_view*m_model*m_box[i];
  } 

  cout << m_box[0] << endl;
  cout << m_box[1] << endl;
  cout << m_box[2] << endl;
  cout << m_box[3] << endl;

  //draw all dem points
  for (int i =0; i < m_box.size(); i++) {
    if (i < m_box.size() -1) {
      draw_line(Point2D(m_box[i][0], m_box[i][1]), Point2D(m_box[i+1][0], m_box[i+1][1])); 
    } else {
      draw_line(Point2D(m_box[i][0], m_box[i][1]), Point2D(m_box[0][0], m_box[0][1])); 
    }
  } 

  //draw random stuff
  draw_line(Point2D(150, 150), Point2D(180, 150));
  draw_line(Point2D(150, 150), Point2D(150, 180));

  draw_complete();
            
  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
  button_pressed = event->button;
  prevMouse = event->x;
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " released" << std::endl;
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;

  double diff = event->x - prevMouse;
  userTransform(diff); 
  prevMouse = event->x; 

  return true;
}
