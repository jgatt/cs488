#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "draw.hpp"
#include "a2.hpp"

using namespace std;

double prevMouse = 0;
int button_pressed[] = {0, 0, 0};

Vector3D scale_factor(1.0, 1.0, 1.0);

double aspect = 1.0;

double vangle = 30.0;
double near = 1;
double far = 10;

double model_rotation[] = {0, 0, 0};
double view_rotation[] = {0, 0, 0};

int mode = 1;

int xmin = 30;
int xmax = 270;
int ymin = 30;
int ymax = 270;

int tempMouseOne[] = {0, 0};

string labels[] = {
  "View Translate",
  "Model Translate",
  "Model Rotate",
  "Model Scale",
  "View Rotate",
  "View Perspective",
  "Viewport Mode"
};

Point3D cube_lines[][2] = {
  {Point3D(-1, -1, -1), Point3D(-1, 1, -1)},
  {Point3D(-1, 1, -1), Point3D(1, 1, -1)},
  {Point3D(1, 1, -1), Point3D(1, -1, -1)},
  {Point3D(1, -1, -1), Point3D(-1, -1, -1)},

  {Point3D(-1, -1, 1), Point3D(-1, 1, 1)},
  {Point3D(-1, 1, 1), Point3D(1, 1, 1)},
  {Point3D(1, 1, 1), Point3D(1, -1, 1)},
  {Point3D(1, -1, 1), Point3D(-1, -1, 1)},

  {Point3D(-1, -1, -1), Point3D(-1, -1, 1)},
  {Point3D(-1, -1, 1), Point3D(-1, 1, 1)},
  {Point3D(-1, 1, 1), Point3D(-1, 1, -1)},
  {Point3D(-1, 1, -1), Point3D(-1, -1, -1)},

  {Point3D(1, -1, -1), Point3D(1, -1, 1)},
  {Point3D(1, -1, 1), Point3D(1, 1, 1)},
  {Point3D(1, 1, 1), Point3D(1, 1, -1)},
  {Point3D(1, 1, -1), Point3D(1, -1, -1)} 
};

Point3D model_coords[][2] = {
  {Point3D(0, 0, 0), Point3D(0.5, 0, 0)},
  {Point3D(0, 0, 0), Point3D(0, -0.5, 0)},
  {Point3D(0, 0, 0), Point3D(0, 0, 0.5)}
};

Point3D world_coords[][2] = {
  {Point3D(0, 0, 0), Point3D(0.5, 0, 0)},
  {Point3D(0, 0, 0), Point3D(0, -0.5, 0)},
  {Point3D(0, 0, 0), Point3D(0, 0, 0.5)}
};

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
  m_model_translation = translation(Vector3D(0, 0, 0));
  m_model = m_model_translation * m_model_scaling * m_model;

  m_viewing_translation = translation(Vector3D(0, 0, 4)); 
  m_viewing = m_viewing_translation;
  label.set_text(labels[1]);
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
  far = 10;
  scale_factor = Vector3D(1.0, 1.0, 1.0);

  for (int i = 0; i < 3; i++){
      model_rotation[i] = 0;
      view_rotation[i] = 0; 

      m_model_rotations[i] = Matrix4x4(); 
      m_viewing_rotations[i] = Matrix4x4(); 
  }

  m_model_rotation = Matrix4x4();

  m_model_scaling = scaling(scale_factor); 
  m_model_translation = translation(Vector3D(0, 0, 0));

  m_model = m_model_translation * m_model_scaling * m_model_rotation;

  m_viewing_translation = translation(Vector3D(0, 0, 4)); 
  m_viewing = m_viewing_translation * m_viewing_rotation;

  xmin = 0.1*get_width(); ymin = 0.1*get_height();
  xmax = 0.9*get_width(); ymax = 0.9*get_height(); 

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
  label.set_text(labels[m]);
}

void Viewer::userTransform(int diff) {
  Matrix4x4 m_transform;
  Vector3D v_transform(0, 0, 0);
  char axis[] = {'x', 'y', 'z'};

  //loop for all possible buttons
  for (int i = 0; i < 3; i++) {
    if (button_pressed[i] == 0) {
      continue;
    }

    switch(mode) {
      case 0:
        v_transform[i] = diff / 100.0;
        m_transform = translation(v_transform);
        m_viewing_translation = m_transform * m_viewing_translation;
        break;
      case 1: 
        v_transform[i] = diff / 100.0;
        m_transform = translation(v_transform);
        m_model_translation = m_transform * m_model_translation;
        break;
      case 2:
        model_rotation[i] += diff;
        m_model_rotations[i] = rotation(model_rotation[i], axis[i]);
        m_model_rotation = m_model_rotations[0] * m_model_rotations[1] * m_model_rotations[2]; 
        break;
      case 3:
        scale_factor[i] += diff / 100.0;
        if (scale_factor[i] > 1.5) {
          scale_factor[i] = 1.5;
        } else if (scale_factor[i] < 0.05) {
          scale_factor[i] = 0.05;
        }
        m_model_scaling = scaling(scale_factor);
        break;
      case 4:
        view_rotation[i] += diff ;
        m_viewing_rotations[i] = rotation(view_rotation[i], axis[i]);
        m_viewing_rotation = m_viewing_rotations[0] * m_viewing_rotations[1] * m_viewing_rotations[2]; 
        break;
      case 5:
        if (i == 0) {
          vangle += diff; 
          if (vangle < 5) {
            vangle = 5;
          } else if (vangle > 160) {
            vangle = 160;
          }
        } else if (i == 1) {
          near += diff / 10.0; 
          if (near < 0) {
            near = 0;
          } else if (near > far - 1) {
            near = far - 1;
          }

        } else if (i == 2) {
          far += diff / 10.0;
          if (far < near + 1) {
            far = near + 1;
          }
        }
        break;
      default: 
        break;
    }
  }

  

  invalidate();
}

int bitRep(Point3D A, bool mode2d) {
  int bit = 0;
  if (mode2d) {
    if (A[1] > ymax) {
      bit |= 1 << 1;
    } if (A[1] < ymin) {
      bit |= 1 << 2;
    } if (A[0] > xmax) {
      bit |= 1 << 3;
    } if (A[0] < xmin) {
      bit |= 1 << 4;
    }
  } else if (!mode2d) {
    if (A[2] > -near) {
      bit |= 1 << 6;
    } if (A[2] < -far) {
      bit |= 1 << 5;
    }
  }

  return bit; 
}

void calculateClippedPoints(Point3D &A, Point3D &B, int bit1, int bit2, int check_bit, double clip_value) {
  Point3D *in;
  Point3D *out;

  if (bit1 & (1 << check_bit)) {
    out = &A;
    in = &B;
  } else if (bit2 & (1 << check_bit)) {
    out = &B;
    in = &A;
  }

  double x0 = (*in)[0];
  double x1 = (*out)[0];

  double y0 = (*in)[1];
  double y1 = (*out)[1];

  double z0 = (*in)[2];
  double z1 = (*out)[2];

  double t;

  switch (check_bit) {
    case 1: case 2:
      t = (clip_value - y0) / (y1 - y0);

      (*out)[0] = x0 + t*(x1 - x0); 
      (*out)[1] = clip_value;
      (*out)[2] = z0 + t*(z1 - z0);
      break;
    case 3: case 4:
      t = (clip_value - x0) / (x1 - x0);

      (*out)[0] = clip_value; 
      (*out)[1] = y0 + t*(y1 - y0);
      (*out)[2] = z0 + t*(z1 - z0);
      break;
    case 5: case 6:
      t = (clip_value - z0) / (z1 - z0);

      (*out)[0] = x0 + t*(x1 - x0); 
      (*out)[1] = y0 + t*(y1 - y0);
      (*out)[2] = clip_value;
      break;
    default:
      break;
  }
  
}

int Viewer::clip(Point3D &A, Point3D &B, int face, bool mode2d) {
  int bit1 = bitRep(A, mode2d); 
  int bit2 = bitRep(B, mode2d); 
  int clip_values[] = {0, ymax, ymin, xmax, xmin, -far, -near}; 

  if ((bit1 == 0) && (bit2 == 0)){
    return 1;
  } else if ((bit1 & bit2) != 0) {
    return 0;
    } else {
    if ((bit1 & (1 << face)) || (bit2 & (1 << face))) {
      calculateClippedPoints(A, B, bit1, bit2, face, clip_values[face]);
    }

    return 1;
  } 
}

void viewPortMap(Point3D &p) {
  p[0] = (p[0] + 1)*((xmax - xmin) / 2.0) + xmin;
  p[1] = (p[1] + 1)*((ymax - ymin) / 2.0) + ymin;
}

vector<Point2D> Viewer::transformPoints(Point3D box_line[], int mode) {
  vector<Point2D> drawPoints;
  Matrix4x4 project = perspective(vangle, 1, near, far);
  Matrix4x4 model, viewing;

  if (mode == 0) {
    model = m_model_rotation * m_model_translation;
  } else if (mode == 1) {
    model = m_model_rotation * m_model_translation * m_model_scaling;
  } 

  viewing = m_viewing_translation * m_viewing_rotation; 

  Point3D point1 = box_line[0]; 
  Point3D point2 = box_line[1]; 

  point1 = viewing.invert()*model*point1; 
  point2 = viewing.invert()*model*point2; 

  if ((clip(point1, point2, 6, 0) == 0) || (clip(point1, point2, 5, 0) == 0)) {
    return drawPoints;
  }

  double w1 = -1*point1[2];
  double w2 = -1*point2[2];

  point1 = project*point1; 
  point2 = project*point2; 

  for (int i = 0; i < 4; i++) {
    point1[i] = point1[i] / w1;
    point2[i] = point2[i] / w2;
  }

  viewPortMap(point1);
  viewPortMap(point2);

  for (int i = 1; i < 6; i++) {
    if (clip(point1, point2, i, 1) == 0) {
      return drawPoints;
    }
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
  draw_line(Point2D(xmin, ymin), Point2D(xmax, ymin));
  draw_line(Point2D(xmin, ymin), Point2D(xmin, ymax));

  draw_line(Point2D(xmax, ymax), Point2D(xmin, ymax));
  draw_line(Point2D(xmax, ymax), Point2D(xmax, ymin));


  set_colour(Colour(1.0, 0, 0));

  //Transform all dem points
  for (int i = 0; i < 16; i++) {
    vector<Point2D> drawPoints = transformPoints(cube_lines[i], 1); 
    if (drawPoints.size() == 2) {
      draw_line(drawPoints[0], drawPoints[1]);
    }
  }


  set_colour(Colour(0, 1.0, 0));

  //draw gnomes
  for (int i = 0; i < 3; i++) {
    vector<Point2D> drawPoints = transformPoints(model_coords[i], 0); 
    if (drawPoints.size() == 2) {
      draw_line(drawPoints[0], drawPoints[1]);
    }
  }

  set_colour(Colour(0, 0, 1.0));

  for (int i = 0; i < 3; i++) {
    vector<Point2D> drawPoints = transformPoints(world_coords[i], 2); 
    if (drawPoints.size() == 2) {
      draw_line(drawPoints[0], drawPoints[1]);
    }
  }

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

  aspect = get_width() / get_height();

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
  button_pressed[event->button - 1] = 1;
  prevMouse = event->x;

  if (mode == 6) {
    tempMouseOne[0] = event->x;
    tempMouseOne[1] = event->y;
  }

  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " released" << std::endl;

  button_pressed[event->button - 1] = 0;

  if (mode == 6) {
    if (event->x < tempMouseOne[0]) {
      xmax = tempMouseOne[0];
      xmin = event->x; 
    } else {
      xmax = event->x;
      xmin = tempMouseOne[0];
    }

    if (event->y < tempMouseOne[1]) {
      ymax = tempMouseOne[1];
      ymin = event->y;
    } else {
      ymax = event->y;
      ymin = tempMouseOne[1];
    }

    if (xmin < 0) {
      xmin = 0;
    }
    if (xmax > get_width()) {
      xmax = get_width();
    }

    if (ymin < 0) {
      ymin = 0;
    }
    if (ymax > get_height()) {
      ymax = get_height();
    }

    invalidate(); 
  }

  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{

  double diff = event->x - prevMouse;
  userTransform(diff); 
  prevMouse = event->x; 

  return true;
}
