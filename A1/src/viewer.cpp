#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

using namespace std;

double rotate_x = 0.0f;
double rotate_y = 0.0f;
double rotate_z = 0.0f;

double scale_factor = 1.0f;

int rotationMode = 0; 
int previousMousePosition = 0;

bool shiftOn = false;

 int delayInMillis = 1000; 

 bool gameStarted = false;

sigc::connection timer;
sigc::slot0<bool> tslot; 

static int colours[][3] = {
  {1, 0, 0}, //red
  {0, 1, 0}, //green
  {0, 0, 1}, //blue
  {1.0, 0.5, 0.0}, //orange
  {1.0, 1.0, 0.0}, //yellow
  {1.0, 0.0, 1.0}, //magenta
  {0.0, 1.0, 1.0}
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

}

Viewer::~Viewer()
{
  delete game;
}


bool Viewer::tick() {
  game->tick();
  invalidate();
  return true;
}

void Viewer::invalidate()
{
  //Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
  
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

  // Just enable depth testing and set the background colour.
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.7, 0.7, 1.0, 0.0);

  gldrawable->gl_end();
}

void Viewer::setDrawMode(int mode) {
  cout << mode << endl;
  if (mode == 0) {
    cout << "WIREFRAME" << endl;
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  } else if (mode == 1){
    cout << "NOT WIREFRAME" << endl;
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  } else {
    cout << "MULTICOLOUR" << endl;
  }

  invalidate();
}

void Viewer::setSpeed(int speed) {
  timer.disconnect();   
  switch(speed) {
    case 0:
      timer = Glib::signal_timeout().connect(tslot, 2000);
      break;
    case 1:
      timer = Glib::signal_timeout().connect(tslot, 1000);
      break;
    case 2:
      timer = Glib::signal_timeout().connect(tslot, 500);
      break;
    default:
      break;
  }
}

void Viewer::gameKeyUp(int keyval) {
  shiftOn = false;
}

void Viewer::gameKeyDown(int keyval) {
  switch(keyval) {
    case 32:
      game->drop();
      break;
    case 65361:
      game->moveLeft();
      break;
    case  65362:
      game->rotateCW(); 
      break;
    case 65363:
      game->moveRight();
      break;
    case 65364:
      game->rotateCCW(); 
      break;
    case 65505:
      shiftOn = true; 
    default:
      break;
  }  
  invalidate();
}

void Viewer::scaleScene(int mousePos) {
  int diff = mousePos - previousMousePosition;
  scale_factor += (diff / 10000.0f);
  cout << scale_factor << endl;

  if (scale_factor < 0.4)  {
    scale_factor = 0.4;
  } else if (scale_factor > 1.4) {
    scale_factor = 1.4;
  }

  invalidate();
}

void Viewer::rotateAboutAxis(int mousePos) {
  int diff = mousePos - previousMousePosition;
  switch(rotationMode) {
    case 0:
      break;
    case 1:
      rotate_x += (diff / 100.0f);
      break;
    case 2:
      rotate_y += (diff / 100.0f);
      break;
    case 3:
      rotate_z += (diff / 100.0f);
      break;
    default:
      break;
  }

  invalidate();
}

void Viewer::drawGame() {
  for (int r =0; r < game->getHeight() + 4; r++) {
    for (int c = 0; c < game->getWidth(); c++) {
      int temp = game->get(r, c);
      if (temp != -1) {
        drawCube(c, r, 0, temp);
      }
    }
  }
}

void Viewer::drawCube(double x, double y, double z, int colour) {
  glPushMatrix();
  glTranslated(x, y, z);
  glBegin(GL_QUADS);
  
  int *colour_def = colours[colour];
  glColor3f(colour_def[0], colour_def[1], colour_def[2]);     // Green

  glVertex3d( 1.0, 1.0, 0);
  glVertex3d(0, 1.0, 0);
  glVertex3d(0, 1.0,  1.0);
  glVertex3d( 1.0, 1.0,  1.0);

  // Bottom face (y = 0f)
  glVertex3d( 1.0, 0,  1.0);
  glVertex3d(0, 0,  1.0);
  glVertex3d(0, 0, 0);
  glVertex3d( 1.0, 0, 0);

  // Front face  (z = 1.0f)
  glVertex3d( 1.0,  1.0, 1.0);
  glVertex3d(0,  1.0, 1.0);
  glVertex3d(0, 0, 1.0);
  glVertex3d( 1.0, 0, 1.0f);

  // Back face (z = 0f)
  glVertex3d( 1.0, 0, 0);
  glVertex3d(0, 0, 0);
  glVertex3d(0,  1.0, 0);
  glVertex3d( 1.0,  1.0, 0);

  // Left face (x = 0f)
  glVertex3d(0,  1.0,  1.0);
  glVertex3d(0,  1.0, 0);
  glVertex3d(0, 0, 0);
  glVertex3d(0, 0,  1.0);

  // Right face (x = 1.0f)
  glVertex3d(1.0,  1.0, 0);
  glVertex3d(1.0,  1.0,  1.0);
  glVertex3d(1.0, 0,  1.0);
  glVertex3d(1.0, 0, 0);

  glEnd(); 
  glPopMatrix();
}

void Viewer::drawWell() {
  for (int i = 0; i < 24; i++) {
    drawCube(-1.0, (double)i, 0, 6);
  }

  for (int i = -1; i < 11; i++) {
    drawCube((double)i, -1.0, 0, 0); 
  } 

  for (int i = 0; i < 24; i++) {
    drawCube(10.0, (double)i, 0, 0); 
  } 
}

void Viewer::newGame() {
  cout << "IN NEW GAME << endl";
  game = new Game(10, 20); 

  tslot = sigc::mem_fun(this, &Viewer::tick);
  timer = Glib::signal_timeout().connect(tslot, delayInMillis);
  gameStarted = true;
}

void Viewer::reset() {

}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Clear the screen

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Modify the current projection matrix so that we move the 
  // camera away from the origin.  We'll draw the game at the
  // origin, and we need to back up to see it.
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glTranslated(0.0, 0.0, -40.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Not implemented: set up lighting (if necessary)

  // Not implemented: scale and rotate the scene
  glScaled(scale_factor, scale_factor, scale_factor);

  glRotated(rotate_x, 1, 0, 0);
  glRotated(rotate_y, 0, 1, 0);
  glRotated(rotate_z, 0, 0, 1);

  // You'll be drawing unit cubes, so the game will have width
  // 10 and height 24 (game = 20, stripe = 4).  Let's translate
  // the game so that we can draw it starting at (0,0) but have
  // it appear centered in the window.
  glTranslated(-5.0, -12.0, 0.0);

  drawWell(); 
  if (gameStarted) {
    drawGame();
  }

  // We pushed a matrix onto the PROJECTION stack earlier, we 
  // need to pop it.
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

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

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;

  previousMousePosition = event->x;
  rotationMode = event->button;

  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " released" << std::endl;
  rotationMode = 0;
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
  if (shiftOn) {
    scaleScene(event->x);
  } else {
    rotateAboutAxis(event->x);
  }
  return true;
}
