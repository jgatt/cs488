#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <math.h>

using namespace std;

double scale_factor = 1.0f;
double gravityAmount = 0;
double gravityTotal = 0;
double rotate_amount[] = {0, 0, 0};

int rotationMode = 0; 
int previousMousePosition = 0;
int gravityDirection = 0;
int game_speed = 500; 

bool inputAllowed = true;
bool shiftOn = false;
bool gameStarted = false;
bool compatibility_mode = false;
bool multi_colour_mode = false;

sigc::connection timer;
sigc::connection gravityTimer;

sigc::slot0<bool> tslot; 
sigc::slot0<bool> tslotGravity; 

GLfloat light0_position[] = { 0, 0, 0, 0, 0};
GLfloat light0_ambient[] = { 0, 0, 0, 1.0 };
GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat material_emission[] = { 0, 0, 0, 1.0 };

static float colours[][3] = {
  {1.0, 0, 0}, //red
  {0, 1.0, 0}, //green
  {0, 0, 1.0}, //blue
  {1.0, 0.5, 0.0}, //orange
  {1.0, 1.0, 0.0}, //yellow
  {1.0, 0.0, 1.0}, //magenta
  {0.0, 1.0, 1.0},
  {0.5, 0.0, 0.5},
  {1.0, 1.0, 1.0},
  {1.0, 1.0, 1.0}
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

  game = NULL;
}

Viewer::~Viewer()
{
  delete game;
}

void Viewer::toggleCompatibility() {
  compatibility_mode = !compatibility_mode;
  game->setCompatibilityMode(compatibility_mode);
  invalidate();
}

bool Viewer::tick() {
 
	if (game->tick() < 0 && inputAllowed) {
        inputAllowed = false;
        gravityAmount = 31.5;
        gravityTotal = 31.5;
        gravityDirection = -1;
        rotationMode = 2;
        tslotGravity = sigc::mem_fun(this, &Viewer::gravityRotate);
        gravityTimer = Glib::signal_timeout().connect(tslotGravity, 16);  
	} 	

  invalidate();
  return true;
};

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
  multi_colour_mode = false;
  if (mode == 0) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  } else if (mode == 1) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  } else if (mode == 2) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    multi_colour_mode = true;
  }

  invalidate();
}

void Viewer::setSpeed(int speed) {
  switch(speed) {
    case 0:
    game_speed = 500;
    break;
    case 1:
    game_speed = 250;
    break;
    case 2:
    game_speed = 100;
    break;
    default:
    break;
  }
  if (gameStarted) {
    timer.disconnect();
    timer = Glib::signal_timeout().connect(tslot, game_speed);
  } 
}

void Viewer::gameKeyUp(int keyval) {
  if (keyval == GDK_KEY_Shift_L || keyval == GDK_KEY_Shift_R) {
  	shiftOn = false;
	}
}

void Viewer::gameKeyDown(int keyval) {
  if (keyval == GDK_KEY_Shift_L || keyval == GDK_KEY_Shift_R) {
    shiftOn = true;
    return;
  } else if (gameStarted && inputAllowed) {
    switch(keyval) {
      case GDK_KEY_space:
        game->drop();
        break;
      case GDK_KEY_Left:
        game->moveLeft();
        break;
      case GDK_KEY_Up:
        game->rotateCCW(); 
        break;
      case GDK_KEY_Right:
        game->moveRight();
        break;
      case GDK_KEY_Down:
        game->rotateCW(); 
        break; 
      default:
        break;
    }  
    invalidate();
  } 
}

void Viewer::scaleScene(int mousePos) {
  int diff = mousePos - previousMousePosition;
  scale_factor += (diff / 100.0f);
  if (scale_factor < 0.2)  {
    scale_factor = 0.2;
  } else if (scale_factor > 1.2) {
    scale_factor = 1.2;
  }

  invalidate();
}

void Viewer::rotateAboutAxis(int mousePos) {
  int diff = mousePos - previousMousePosition;
  rotate_amount[rotationMode - 1] += (diff); 
  invalidate();
}

void Viewer::drawGame() {
  for (int r =0; r < game->getHeight() + 4; r++) {
    for (int c = 0; c < game->getWidth(); c++) {
      int colour = game->get(r, c);
      if (colour != -1) {
        drawCube(c, r, 0, colour);
      }
    }
  }
}

void Viewer::drawCube(double x, double y, double z, int colour) {
  glPushMatrix();
  glTranslated(x, y, z);
  glBegin(GL_QUADS);
  
  float *colour_def = colours[colour];
  glColor3f(colour_def[0], colour_def[1], colour_def[2]);  

  glVertex3d( 1.0, 1.0, 0);
  glVertex3d(0, 1.0, 0);
  glVertex3d(0, 1.0,  1.0);
  glVertex3d( 1.0, 1.0,  1.0);

  if (multi_colour_mode) {
    colour_def = colours[(colour + 1) % 9];
    glColor3f(colour_def[0], colour_def[1], colour_def[2]);    
  }

  // Bottom face (y = 0f)
  glVertex3d( 1.0, 0,  1.0);
  glVertex3d(0, 0,  1.0);
  glVertex3d(0, 0, 0);
  glVertex3d( 1.0, 0, 0);

 if (multi_colour_mode) {
    colour_def = colours[(colour + 2) % 9];
    glColor3f(colour_def[0], colour_def[1], colour_def[2]);    
  } 

  // Front face  (z = 1.0f)
  glVertex3d( 1.0,  1.0, 1.0);
  glVertex3d(0,  1.0, 1.0);
  glVertex3d(0, 0, 1.0);
  glVertex3d( 1.0, 0, 1.0f);

  if (multi_colour_mode) {
    colour_def = colours[(colour + 3) % 9];
    glColor3f(colour_def[0], colour_def[1], colour_def[2]);     
  }
  // Back face (z = 0f)
  glVertex3d( 1.0, 0, 0);
  glVertex3d(0, 0, 0);
  glVertex3d(0,  1.0, 0);
  glVertex3d( 1.0,  1.0, 0);

  if (multi_colour_mode) {
    colour_def = colours[(colour + 4) % 9];
    glColor3f(colour_def[0], colour_def[1], colour_def[2]);    
  }
  // Left face (x = 0f)
  glVertex3d(0,  1.0,  1.0);
  glVertex3d(0,  1.0, 0);
  glVertex3d(0, 0, 0);
  glVertex3d(0, 0,  1.0);

  if (multi_colour_mode) {
    colour_def = colours[(colour + 5) % 9];
    glColor3f(colour_def[0], colour_def[1], colour_def[2]);    
  }
  // Right face (x = 1.0f)
  glVertex3d(1.0,  1.0, 0);
  glVertex3d(1.0,  1.0,  1.0);
  glVertex3d(1.0, 0,  1.0);
  glVertex3d(1.0, 0, 0);

  glEnd(); 
  glPopMatrix();
}

void Viewer::drawWell() {
  for (int i = 0; i < 20; i++) {
    drawCube(-1.0, (double)i, 0, 8);
  }

  for (int i = -1; i < 11; i++) {
    drawCube((double)i, -1.0, 0, 8); 
  } 

  for (int i = 0; i < 20; i++) {
    drawCube(10.0, (double)i, 0, 8); 
  } 
}

//NOTE: For new game I made the assumption it doesn't reset the board
void Viewer::newGame() { 
  if (game == NULL) {
    game = new Game(10, 20); 
    gameStarted = true;
  } else {
    timer.disconnect();
    game->reset();
  }
  inputAllowed = true;
  
  tslot = sigc::mem_fun(this, &Viewer::tick);
  timer = Glib::signal_timeout().connect(tslot, game_speed);
	
  invalidate();
}

void Viewer::reset() {
  scale_factor = 1.0f;
  for (int i = 0; i < 3; i++) {
    rotate_amount[i] = 0;
  } 

  invalidate();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glTranslated(0.0, 0.0, -40.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //Lighting  
  if (!compatibility_mode) {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
  } else {
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT0);
  }

  //Scale and Rotate
  glScaled(scale_factor, scale_factor, scale_factor);

  glRotated(rotate_amount[0], 1, 0, 0);
  glRotated(rotate_amount[1], 0, 1, 0);
  glRotated(rotate_amount[2], 0, 0, 1);
 
  glTranslated(-5.0, -12.0, 0.0);

  //Draw the Game
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



bool Viewer::gravityRotate() {
  rotate_amount[rotationMode - 1] += (gravityDirection) * (gravityTotal * (gravityAmount / gravityTotal)); 
  gravityAmount -= (gravityTotal / gravityAmount);

  if (gravityAmount <= 0) {
    gravityTotal = 0;
    gravityTimer.disconnect(); 
  }

  invalidate();
  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  previousMousePosition = event->x;
  rotationMode = event->button;

  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  if (gravityAmount != 0 && gravityTotal == 0) {
    gravityTotal = gravityAmount;
    tslotGravity = sigc::mem_fun(this, &Viewer::gravityRotate);
    gravityTimer = Glib::signal_timeout().connect(tslotGravity, 16);
  }

  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  if (shiftOn) {
    scaleScene(event->x);
  } else if (gravityTotal == 0){
    int diff = event->x - previousMousePosition;
    if (abs(diff) > 1) {
      gravityAmount = abs(diff);
      gravityDirection = (diff / abs(diff));
    } else {
      gravityAmount = 0;
    }

    rotateAboutAxis(event->x);
  }

  previousMousePosition = event->x;

  return true;
}
