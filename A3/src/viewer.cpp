#include "viewer.hpp"
#include "algebra.hpp"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define BUFSIZE 512

using namespace std;

GLfloat light_position[] = { 0, 0, 1.0, 0.0 };
GLint viewport[4];
GLuint selectBuf[BUFSIZE];
bool picking_mode = false;
int previousX = 0;
int previousY = 0;
int mode = 1;

bool buttonPressed[] = {false, false, false};

bool zOn = false;
bool bOn = false;
bool fOn = false;
bool cOn = false;

bool somethingChanged = false;

Matrix4x4 rootStart;

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
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::load_lua(string filename) {
  root = import_lua(filename);
  if (!root) {
    std::cerr << "Could not open " << filename << std::endl;
  }
  root->saveChange();
  rootStart = root->get_transform(); 
}

void Viewer::setMode(int m) {
  mode = m;
}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
}

void Viewer::toggleZBuffer() {
  if (zOn) {
    zOn = false;
    glDisable(GL_DEPTH_TEST);
  } else {
    zOn = true;
    glEnable(GL_DEPTH_TEST);
  }
  invalidate();
}

void Viewer::toggleBCull() {
  if (bOn) {
    bOn = false;
    if (fOn) {
      glCullFace(GL_FRONT);
    } else {
      glDisable(GL_CULL_FACE);
    }
  } else {
    bOn = true;
    if (fOn) {
      glCullFace(GL_FRONT_AND_BACK);
    } else {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
    }
  }
  invalidate();
}

void Viewer::toggleFCull() {
  if (fOn) {
    fOn = false;
    if (bOn) {
      glCullFace(GL_BACK);
    } else {
      glDisable(GL_CULL_FACE);
    }
  } else {
    fOn = true;
    if (bOn) {
      glCullFace(GL_FRONT_AND_BACK);
    } else {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
    }
  }
  invalidate();
}

void Viewer::toggleCircle() {
  cOn = !cOn;
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

  glShadeModel(GL_SMOOTH);
  glClearColor( 0.4, 0.4, 0.4, 0.0 );

  glEnable(GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);

  gldrawable->gl_end();
}

void Viewer::pick(int x, int y) {
  glSelectBuffer(BUFSIZE,selectBuf);
  glRenderMode(GL_SELECT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glViewport(0, 0, get_width(), get_height());

  glGetIntegerv(GL_VIEWPORT,viewport);
  gluPickMatrix(x,viewport[3]-y, 5,5,viewport);

  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glInitNames();

  root->walk_gl(true); 

  int hits;
  // restoring the original projection matrix
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glFlush();

  // returning to normal rendering mode
  hits = glRenderMode(GL_RENDER);
    
  // if there are hits process them
  if (hits != 0)  {
    processHits(hits);
    }
}
void Viewer::processHits(GLint hits) {
  unsigned int i, j;
     GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

     printf ("hits = %d\n", hits);
     ptr = (GLuint *) selectBuf;
     minZ = 0xffffffff;
     for (i = 0; i < hits; i++) { 
        names = *ptr;
      ptr++;
      if (*ptr < minZ) {
        numberOfNames = names;
        minZ = *ptr;
        ptrNames = ptr+2;
      }
      
      ptr += names+2;
    }
    printf ("The closest hit names are ");
    ptr = ptrNames;
    for (j = 0; j < numberOfNames; j++,ptr++) {
       printf ("%d\n", *ptr);
       root->flag_picked(*ptr, false);
    }
    printf ("\n");
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up for perspective drawing 
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, get_width(), get_height());
  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear framebuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up lighting
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);

  // Draw stuff
  glEnable(GL_NORMALIZE);
  root->walk_gl(false); 

  if (cOn) {
    draw_trackball_circle();
  }

  glDisable(GL_NORMALIZE);
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

  // Set up perspective projection, using current size and aspect
  // ratio of display

  
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glViewport(0, 0, event->width, event->height);
  
  gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);

  // Reset to modelview matrix mode
  glMatrixMode(GL_MODELVIEW);
  
  gldrawable->gl_end();

  return true;
}

void Viewer::undo() {
  root->undoChange(); 
  invalidate();
}

void Viewer::redo() {
  root->redoChange(); 
  invalidate();
}

void Viewer::reset_position() {
  root->reset(rootStart);
  invalidate();
}

void Viewer::reset_orientation() {
  invalidate();
}

void Viewer::reset_joints() {
  root->reset_joints(); 
  invalidate();
}

void Viewer::reset_all() {
  root->reset(rootStart); 
  root->reset_joints(); 
  invalidate();
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
  if (mode == 2 && event->button == 1) {
    pick(event->x, event->y);
  }
  
  buttonPressed[event->button - 1] = true;
  previousX = event->x;
  previousY = event->y;

  invalidate();
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " released" << std::endl;
  buttonPressed[event->button - 1] = false;

  if (mode == 2 && somethingChanged) {
    root->saveChange();
    somethingChanged = false;
  } 
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
  if (mode == 1 && buttonPressed[0]) {
    double diffx = (event->x - previousX) / 100.0f;
    double diffy = (event->y - previousY) / -100.0f;

    root->translate(Vector3D(diffx, diffy, 0));
  } 
  if (mode == 1 && buttonPressed[1]) {
    double diffy = (event->y - previousY) / -100.0f;

    root->translate(Vector3D(0, 0, diffy));
  }

  if (mode == 2 && buttonPressed[1]) {
    somethingChanged = true;
    root->animate(event->y - previousY, false);
  }
  if (mode == 2 && buttonPressed[2]) {
    somethingChanged = true;
    root->animate(event->x - previousX, true);
  }
  
  previousX = event->x;
  previousY = event->y;

  invalidate();
  return true;
}

void Viewer::draw_trackball_circle()
{
  int current_width = get_width();
  int current_height = get_height();
  
  // Set up for orthogonal drawing to draw a circle on screen.
  // You'll want to make the rest of the function conditional on
  // whether or not we want to draw the circle this time around.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, current_width, current_height);
  glOrtho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);

  // Reset modelview matrix
  glLoadIdentity();

  // draw a circle for use with the trackball 
  glDisable(GL_LIGHTING);
  glEnable(GL_LINE_SMOOTH);
  glColor3f(1.0, 1.0, 1.0);
  double radius = current_width < current_height ? 
    (float)current_width * 0.25 : (float)current_height * 0.25;
  glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
  glBegin(GL_LINE_LOOP);
  for(size_t i=0; i<40; ++i) {
    double cosine = radius * cos(i*2*M_PI/40);
    double sine = radius * sin(i*2*M_PI/40);
    glVertex2f(cosine, sine);
  }
  glEnd();
  glColor3f(0.0, 0.0, 0.0);
  glDisable(GL_LINE_SMOOTH);
}
