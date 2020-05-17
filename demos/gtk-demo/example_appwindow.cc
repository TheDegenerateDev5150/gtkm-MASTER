/* Application main window
 *
 * Demonstrates a typical application window, with menubar, toolbar, statusbar.
 */

#include <gtkmm.h>

class Example_AppWindow : public Gtk::Window
{
public:
  Example_AppWindow();
  ~Example_AppWindow() override;

protected:
  //Signal handlers:
  virtual void on_menu_item();
  virtual void on_text_changed();
  virtual void on_text_mark_set(const Gtk::TextBuffer::iterator& new_location, const Glib::RefPtr<Gtk::TextBuffer::Mark>& mark);

  //Member widgets:
  Gtk::Grid m_Grid;
  //Gtk::Menu m_Menubar;
  Gtk::Box m_Toolbar;
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::Statusbar m_Statusbar;
  Gtk::TextView m_TextView;
};


//Called by DemoWindow;
Gtk::Window* do_appwindow()
{
  return new Example_AppWindow();
}


Example_AppWindow::Example_AppWindow()
{
  set_title("Application Window");

  set_child(m_Grid);

/*
  //Menu:
  {
    using namespace Gtk::Menu_Helpers;

    //File menu:
    auto pMenuFile = Gtk::make_managed<Gtk::Menu>();
    MenuList& list_file = pMenuFile->items();
    list_file.push_back( MenuElem("_New", "<control>N", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_file.push_back( MenuElem("_Open", "<control>O", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_file.push_back( MenuElem("_Save", "<control>S", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_file.push_back( MenuElem("Save _As", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_file.push_back(SeparatorElem());
    list_file.push_back( MenuElem("_Quit", "<control>Q", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );

    //Preferences menu:
    auto pMenuPreferences = Gtk::make_managed<Gtk::Menu>();
    MenuList& list_preferences = pMenuPreferences->items();

    // Create a submenu
    auto pMenuSub_Color = Gtk::make_managed<Gtk::Menu>();
    MenuList& list_sub = pMenuSub_Color->items();
    list_sub.push_back( MenuElem("_Red", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_sub.push_back( MenuElem("_Green", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_sub.push_back( MenuElem("_Blue", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );

    list_preferences.push_back( MenuElem("_Color", *pMenuSub_Color) );

    // Create a submenu
    auto pMenuSub_Shape = Gtk::make_managed<Gtk::Menu>();
    list_sub = pMenuSub_Shape->items();
    list_sub.push_back( MenuElem("_Square", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_sub.push_back( MenuElem("_Rectangle", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );
    list_sub.push_back( MenuElem("_Oval", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );

    list_preferences.push_back( MenuElem("_Shape", *pMenuSub_Shape) );

    //Help menu:
    auto pMenuHelp = Gtk::make_managed<Gtk::Menu>();
    MenuList& list_help = pMenuHelp->items();
    list_help.push_back( MenuElem("_About", sigc::mem_fun(*this, &Example_AppWindow::on_menu_item)) );


    //Create the menu bar
    MenuList& list_bar = m_Menubar.items();
    list_bar.push_front(MenuElem("_Help", *pMenuHelp));
    list_bar.front()->set_right_justified();
    list_bar.push_front(MenuElem("_Preferences", *pMenuPreferences));
    list_bar.push_front(MenuElem("_File", *pMenuFile));

    //Add the menu bar to the Grid:
    //                       left  top  width  height
    m_Grid.attach(m_Menubar, 0,    0,   1,     1);
  } //menu

*/
  //Toolbar:
  {
    m_Toolbar.set_hexpand();
    //                       left  top  width  height
    m_Grid.attach(m_Toolbar, 0,    1,   1,     1);
  }


  m_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  m_ScrolledWindow.set_has_frame(true);
  //                              left  top  width  height
  m_Grid.attach(m_ScrolledWindow, 0,    2,   1,     1);

  set_default_size(200, 200);

  m_ScrolledWindow.set_child(m_TextView);

  /* Create statusbar */
  m_Statusbar.set_hexpand();
  //                         left  top  width  height
  m_Grid.attach(m_Statusbar, 0,    3,   1,     1);

  /* Show text widget info in the statusbar */
  auto refTextBuffer = m_TextView.get_buffer();
  refTextBuffer->signal_changed().connect(sigc::mem_fun(*this, &Example_AppWindow::on_text_changed));
  refTextBuffer->signal_mark_set().connect(sigc::mem_fun(*this, &Example_AppWindow::on_text_mark_set));
  on_text_changed();
}

Example_AppWindow::~Example_AppWindow()
{
}

void Example_AppWindow::on_menu_item()
{
  Gtk::MessageDialog dialog(*this, "You selected or toggled the menu item", false,
                            Gtk::MessageType::INFO, Gtk::ButtonsType::CLOSE, true);
  // This is not by itself an acceptable replacement for the removed Gtk::Dialog::run().
  // Won't be fixed now. All uses of on_menu_item() are commented out.
  dialog.show();
}

void Example_AppWindow::on_text_changed()
{
  m_Statusbar.pop();

  auto refBuffer = m_TextView.get_buffer();
  gint count = refBuffer->get_char_count();

  auto iter = refBuffer->get_iter_at_mark(refBuffer->get_insert());

  gint row = iter.get_line();
  gint col = iter.get_line_offset();

  gchar* msg = g_strdup_printf ("Cursor at row %d column %d - %d chars in document",
                         row, col, count);
  m_Statusbar.push(msg);
  g_free (msg);
}


void Example_AppWindow::on_text_mark_set(const Gtk::TextBuffer::iterator&, const Glib::RefPtr<Gtk::TextBuffer::Mark>&)
{
  on_text_changed();
}

