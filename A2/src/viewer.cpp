#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "draw.hpp"
#include "a2.hpp"

using namespace std;

double prevMouse = 0;
int button_pressed = 0;

Vector3D scale_factor(10, 10, 1);

double vangle = 30.0;
double near = 1;
double far = 10;

double model_rotation[] = {0, 0, 0};
double view_rotation[] = {0, 0, 0};

int mode = 0;

Point3D cube_lines[][2] = {
  {Point3D(-1, -1, -1), Point3D(-1, 1, -1)},
  {Point3D(-1, 1, -1), Point3D(1, 1, -1)},
  {Point3D(1, 1, -1), Point3D(1, -1, -1)},
  {Point3D(1, -1, -1), Point3D(-1, -1, -1)},

  {Point3D(-1, -1, -2), Point3D(-1, 1, -2)},
  {Point3D(-1, 1, -2), Point3D(1, 1, -2)},
  {Point3D(1, 1, -2), Point3D(1, -1, -2)},
  {Point3D(1, -1, -2), Point3D(-1, -1, -2)},

  {Point3D(-1, -1, -1), Point3D(-1, -1, -2)},
  {Point3D(-1, -1, -2), Point3D(-1, 1, -2)},
  {Point3D(-1, 1, -2), Point3D(-1, 1, -1)},
  {Point3D(-1, 1, -1), Point3D(-1, -1, -1)},

  {Point3D(1, -1, -1), Point3D(1, -1, -2)},
  {Point3D(1, -1, -2), Point3D(1, 1, -2)},
  {Point3D(1, 1, -2), Point3D(1, 1, -1)},
  {Point3D(1, 1, -1), Point3D(1, -1, -1)} 
};


// m_box_2[0] = Point3D(-1, -1, -2);
  // m_box_2[1] = Point3D(-1, 1, -2); 
  // m_box_2[2] = Point3D(1, 1, -2); 
  // m_box_2[3] = Point3D(1, -1, -2); 
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


  m_model_scaling = scaling(scale_factor); 
  m_model_translation = translation(Vector3D(50, 50, -2));
  m_model = m_model_translation * m_model_scaling * m_model;
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
  vangle = 30.0;
  near = 1;
  far = 5;
  scale_factor = Vector3D(10, 10, 1);

  for (int i = 0; i < 3; i++){
      model_rotation[i] = 0;
      view_rotation[i] = 0; 

      m_model_rotations[i] = Matrix4x4(); 
      m_viewing_rotations[i] = Matrix4x4(); 
  }

  m_model_rotation = Matrix4x4();

  m_model_scaling = scaling(scale_factor); 
  m_model_translation = translation(Vector3D(50, 50, -2));

  m_model = m_model_translation * m_model_scaling;

  m_viewing_translation = Matrix4x4();
  m_viewing = m_viewing_translation * m_viewing_rotation;

  invalidate();
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

void Viewer::setMode(int m) {
  mode = m; 
}

void Viewer::userTransform(int diff) {
  Matrix4x4 m_transform;
  Vector3D v_transform(0, 0, 0);
  char axis[] = {'x', 'y', 'z'};
  switch(mode) {
    case 0:
      v_transform[button_pressed - 1] = diff;
      m_transform = translation(v_transform);
      m_viewing_translation = m_transform * m_viewing_translation;
      break;
    case 1: 
      v_transform[button_pressed - 1] = diff;
      m_transform = translation(v_transform);
      m_model_translation = m_transform * m_model_translation;
      break;
    case 2:
      model_rotation[button_pressed -1] += diff;
      m_model_rotations[button_pressed - 1] = rotation(model_rotation[button_pressed -1], axis[button_pressed - 1]);
      m_model_rotation = m_model_rotations[0] * m_model_rotations[1] * m_model_rotations[2]; 
      break;
    case 3:
      scale_factor[button_pressed -1] += diff;
      if (scale_factor[button_pressed -1] > 200) {
        scale_factor[button_pressed -1] = 200;
      } else if (scale_factor[button_pressed -1] < 0.5) {
        scale_factor[button_pressed -1] = 0.5;
      }

      m_model_scaling = scaling(scale_factor);
      break;
    case 4:
      view_rotation[button_pressed -1] += diff;
      m_viewing_rotations[button_pressed - 1] = rotation(view_rotation[button_pressed -1], axis[button_pressed - 1]);
      m_viewing_rotation = m_viewing_rotations[0] * m_viewing_rotations[1] * m_viewing_rotations[2]; 
      break;
    case 5:
      if (button_pressed == 1) {
        vangle += diff; 
        if (vangle < 5) {
          vangle = 5;
        } else if (vangle > 160) {
          vangle = 160;
        }
      } else if (button_pressed == 2) {
        near += diff; 
      } else if (button_pressed == 3) {
        far += diff;
      }
      break;
    default: 
      break;
  }

  m_model = m_model_translation *  m_model_rotation * m_model_scaling;
  m_viewing = m_viewing_translation * m_viewing_rotation; 

  invalidate();
}

int bitRep(Point3D A) {
  int bit = 0;

  if (A[2] > -near) {
    bit |= 1 << 6;
  } else if (A[2] < -far) {
    bit |= 1 << 5;
  }

  return bit; 
}

void temp(Point3D &A, Point3D &B, int bit1, int bit2, int check_bit, double clip_value) {
  Point3D in;
  Point3D out;

  if (bit1 & (1 << check_bit)) {
    out = A;
    in = B;
  } else if (bit2 & (1 << check_bit)) {
    out = B;
    in = A;
  }

  double t = (-1*clip_value - in[2]) / (in[2] - out[2]);

  out[0] = in[0] + t*(in[0] - out[0]); 
  out[1] = in[1] + t*(in[1] - out[1]);
  out[2] = clip_value;

  A = in;
  B = out; 
}

int Viewer::clipInPlace(Point3D &A, Point3D &B) {
  int bit1 = bitRep(A); 
  int bit2 = bitRep(B); 
  int clip_values[] = {0, 0, 0, 0, 0, -far, -near}; 

  if ((bit1 == 0) && (bit2 == 0)){
    return 1;
  } else if ((bit1 & bit2) != 0) {
    return 0;
    } else {
    cout << "THERE IS AN INTERSECTION POINT" << endl;
    for (int i = 0; i < 8; i++) {
      if ((bit1 & (1 << i)) || (bit2 & (1 << i))) {
        temp(A, B, bit1, bit2, i, clip_values[i]);
      } 
    }
    return 2;
  } 
}

vector<Point2D> Viewer::transformPoints(Point3D box_line[]) {
  vector<Point2D> drawPoints;
  Matrix4x4 project = perspective(vangle, 1, near, far);

  Point3D point1 = box_line[0]; 
  Point3D point2 = box_line[1]; 

  point1 = m_viewing.invert()*m_model*point1; 
  point2 = m_viewing.invert()*m_model*point2; 

  cout << "p1: " << point1 << " p2: " << point2 << endl;

  if (clipInPlace(point1, point2) == 0) {
    cout << "rejected!" << endl;
    return drawPoints;
  }

  double w1 = -1*point2[2];
  double w2 = -1*point1[2];

  cout << "p1: " << point1 << " p2: " << point2 << " w: " << w1 << " w2: " << w2 << endl;

  point1 = project*point1; 
  point2 = project*point2; 

  for (int i = 0; i < 4; i++) {
    point1[i] = point1[i] / w1;
    point2[i] = point2[i] / w2;
  }

  drawPoints.push_back(Point2D(point1[0], point1[1]));
  drawPoints.push_back(Point2D(point2[0], point2[1]));

  return drawPoints;
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
  

  

  //Transform all dem points

  for (int i = 0; i < 16; i++) {
    cout << "NEW POINTS: " << endl;
    vector<Point2D> drawPoints = transformPoints(cube_lines[i]); 
    if (drawPoints.size() == 2) {
      draw_line(drawPoints[0], drawPoints[1]);
    }
  }

  // for (int j = 0; j < 4; j++) {
  //   vector<Point2D> drawPoints = transformPoints(m_box[j]); 

  //   //draw all dem points

  //   for (int i =0; i < drawPoints.size(); i++) {
  //     if (i < drawPoints.size() -1) {
  //       draw_line(drawPoints[i], drawPoints[i+1]); 
  //     } else {
  //       draw_line(drawPoints[i], drawPoints[0]); 
  //     }
  //   } 
  // }
  

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

  double diff = event->x - prevMouse;
  userTransform(diff); 
  prevMouse = event->x; 

  return true;
}
