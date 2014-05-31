#include "appwindow.hpp"

AppWindow::AppWindow()
{
  set_title("CS488 Assignment Two");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem; 
  
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
  m_menu_app.items().push_back(MenuElem("_Reset", Gtk::AccelKey("a"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));

  //m_menu_mode.items().push_back(Radi 
  sigc::slot1<void, int> setModeSlot = sigc::mem_fun(&m_viewer, &Viewer::setMode);   
  Gtk::RadioMenuItem::Group radioGroupMode; 

  m_menu_mode.items().push_back(RadioMenuElem(radioGroupMode, "_View Translate", Gtk::AccelKey("n"),
    sigc::bind(setModeSlot, 0)));

  m_menu_mode.items().push_back(RadioMenuElem(radioGroupMode, "_Model Translate", Gtk::AccelKey("t"),
    sigc::bind(setModeSlot, 1)));

  m_menu_mode.items().push_back(RadioMenuElem(radioGroupMode, "_Model Rotate", Gtk::AccelKey("r"),
    sigc::bind(setModeSlot, 2)));

  m_menu_mode.items().push_back(RadioMenuElem(radioGroupMode, "_Model Scale", Gtk::AccelKey("s"),
    sigc::bind(setModeSlot, 3)));

  m_menu_mode.items().push_back(RadioMenuElem(radioGroupMode, "_View Rotate", Gtk::AccelKey("o"),
    sigc::bind(setModeSlot, 4)));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_menu_mode));
  
  // Pack in our widgets
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 300);
  m_vbox.pack_start(m_viewer);

  show_all();
}
