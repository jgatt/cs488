#include "appwindow.hpp"
#include <iostream>

using namespace std;

AppWindow::AppWindow()
{
  set_title("488 Tetrominoes on the Wall");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem; 

  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
  m_menu_app.items().push_back(MenuElem("_New Game", Gtk::AccelKey("n"),
    sigc::mem_fun(&m_viewer, &Viewer::newGame)));

  m_menu_app.items().push_back(MenuElem("_Reset", Gtk::AccelKey("r"),
    sigc::mem_fun(&m_viewer, &Viewer::reset)));

  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));

  sigc::slot1<void, int> setDrawSlot = sigc::mem_fun(&m_viewer, &Viewer::setDrawMode);   
  Gtk::RadioMenuItem::Group radioGroupDraw; 

  m_menu_draw.items().push_back(RadioMenuElem(radioGroupDraw, "_Fill", Gtk::AccelKey("f"),
    sigc::bind(setDrawSlot, 1)));

  m_menu_draw.items().push_back(RadioMenuElem(radioGroupDraw, "_Wireframe", Gtk::AccelKey("w"),
    sigc::bind(setDrawSlot, 0))); 

  m_menu_draw.items().push_back(RadioMenuElem(radioGroupDraw, "_Multicoloured", Gtk::AccelKey("m"),
    sigc::bind(setDrawSlot, 2)));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Draw Mode", m_menu_draw));

  sigc::slot1<void, int> setSpeedSlot = sigc::mem_fun(&m_viewer, &Viewer::setSpeed); 
  Gtk::RadioMenuItem::Group radioGroupSpeed; 

  m_menu_speed.items().push_back(RadioMenuElem(radioGroupSpeed, "_Slow", Gtk::AccelKey("1"),
    sigc::bind(setSpeedSlot, 0)));

  m_menu_speed.items().push_back(RadioMenuElem(radioGroupSpeed, "_Medium", Gtk::AccelKey("2"),
    sigc::bind(setSpeedSlot, 1)));

  m_menu_speed.items().push_back(RadioMenuElem(radioGroupSpeed, "_Fast", Gtk::AccelKey("3"),
    sigc::bind(setSpeedSlot, 2)));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Speed", m_menu_speed));
  // Pack in our widgets
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 600);
  m_vbox.pack_start(m_viewer);

  show_all();
}

bool AppWindow::on_key_press_event( GdkEventKey *ev )
{
        // This is a convenient place to handle non-shortcut
        // keys.  You'll want to look at ev->keyval.

	// An example key; delete and replace with the
	// keys you want to process
      if (ev->keyval == ' ' ||
        ev->keyval == 65361 ||
        ev->keyval == 65362 ||
        ev->keyval == 65363 || 
        ev->keyval == 65364 ||
        ev->keyval == 65505) {
          m_viewer.gameKeyDown(ev->keyval);
          return true;
        } else {
                return Gtk::Window::on_key_press_event( ev );
        }
}

bool AppWindow::on_key_release_event( GdkEventKey *ev )
{
  if (ev->keyval == 65505) {
    m_viewer.gameKeyUp(ev->keyval);
    return true;
  } else {
    return Gtk::Window::on_key_press_event( ev );
  }
}
