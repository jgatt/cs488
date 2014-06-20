#include "appwindow.hpp"

using namespace std;

AppWindow::AppWindow(string filename)
{
  set_title("Advanced Ergonomics Laboratory");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem; 
  using Gtk::Menu_Helpers::CheckMenuElem; 
  
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
  m_menu_app.items().push_back(MenuElem("_Reset Position", Gtk::AccelKey("i"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  m_menu_app.items().push_back(MenuElem("_Reset Orientation", Gtk::AccelKey("o"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  m_menu_app.items().push_back(MenuElem("_Reset Joints", Gtk::AccelKey("n"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  m_menu_app.items().push_back(MenuElem("_Reset All", Gtk::AccelKey("a"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));

  sigc::slot1<void, int> setModeSlot = sigc::mem_fun(&m_viewer, &Viewer::setMode);   
  Gtk::RadioMenuItem::Group radioGroupMode; 

  m_menu_mode.items().push_back(RadioMenuElem(radioGroupMode, "_Position/Orientation", Gtk::AccelKey("p"),
    sigc::bind(setModeSlot, 1)));

  m_menu_mode.items().push_back(RadioMenuElem(radioGroupMode, "_Joints", Gtk::AccelKey("j"),
    sigc::bind(setModeSlot, 2)));
  
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_menu_mode));

  m_menu_edit.items().push_back(MenuElem("_Undo", Gtk::AccelKey("u"),
    sigc::mem_fun(&m_viewer, &Viewer::undo)));

  m_menu_edit.items().push_back(MenuElem("_Redo", Gtk::AccelKey("r"),
    sigc::mem_fun(&m_viewer, &Viewer::redo)));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Edit", m_menu_edit));

  m_menu_options.items().push_back(CheckMenuElem("_Circle", Gtk::AccelKey("c"),
    sigc::mem_fun(&m_viewer, &Viewer::toggleCircle)));

  m_menu_options.items().push_back(CheckMenuElem("_Z-buffer", Gtk::AccelKey("z"),
    sigc::mem_fun(&m_viewer, &Viewer::toggleZBuffer)));

  m_menu_options.items().push_back(CheckMenuElem("_Backface cull", Gtk::AccelKey("b"),
    sigc::mem_fun(&m_viewer, &Viewer::toggleBCull)));

  m_menu_options.items().push_back(CheckMenuElem("_Frontface cull", Gtk::AccelKey("f"),
    sigc::mem_fun(&m_viewer, &Viewer::toggleFCull)));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Options", m_menu_options));
  // Pack in our widgets
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.load_lua(filename);
  m_viewer.set_size_request(400, 400);
  m_vbox.pack_start(m_viewer);

  show_all();
}
