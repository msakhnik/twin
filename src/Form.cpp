/* 
 * File:   Form.cpp
 * Author: morwin
 * 
 * Created on 4 липня 2012, 10:02
 */

#include "Form.h"
#include <iostream>
#include <gtkmm-3.0/gtkmm/container.h>
#include <glibmm-2.4/glibmm/refptr.h>

using namespace std;

cForm::cForm() :
m_image("Big picture"),
m_ButtonBox(Gtk::ORIENTATION_VERTICAL),
m_ImageBox(Gtk::ORIENTATION_VERTICAL),
m_MainBox(Gtk::ORIENTATION_HORIZONTAL),
m_Button_File("Choose File"),
m_Button_Folder("Choose Folder"),
m_button("Hello World")
// Создает новую кнопку с надписью "Hello World". 
{
    //Устанавливает рамку окна
    set_title("Gtk::FileSelection example");

    add(m_MainBox);
//    m_MainBox.pack_start(m_fixed);
//    m_ButtonBox.property_expand() = true;
    m_MainBox.pack_start(m_ButtonBox);
    
    m_MainBox.pack_start(m_ImageBox);
//    m_ButtonBox.set_size_request(70, 100);
    //    m_ButtonBox.pack_start(m_image);

    m_ButtonBox.pack_start(m_Button_File, true, false, 0);
    m_ImageBox.set_size_request(300, 300);
    m_ImageBox.pack_start(m_image, true, true);
    m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &cForm::on_button_file_clicked));

    m_ButtonBox.pack_start(m_Button_Folder);
    m_ButtonBox.pack_start(m_button);
    m_Button_Folder.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &cForm::on_button_folder_clicked));

    show_all_children();
    m_button.signal_clicked().connect(sigc::mem_fun(*this, &cForm::on_button_clicked));
}

cForm::~cForm()
{
}

void cForm::on_button_clicked()
{
    std::cout << "Hello World" << std::endl;
}

void cForm::on_button_folder_clicked()
{
    Gtk::FileChooserDialog dialog("Please choose a folder",
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();

    //Handle the response:
    switch (result)
    {
    case(Gtk::RESPONSE_OK):
    {
        std::cout << "Select clicked." << std::endl;
        std::cout << "Folder selected: " << dialog.get_filename()
                << std::endl;
        break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
        std::cout << "Cancel clicked." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }
}

void cForm::on_button_file_clicked()
{
    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:
    /*
      Glib::RefPtr<Gtk::FileFilter> filter_text = Gtk::FileFilter::create();
      filter_text->set_name("Text files");
      filter_text->add_mime_type("text/plain");
      dialog.add_filter(filter_text);

      Glib::RefPtr<Gtk::FileFilter> filter_cpp = Gtk::FileFilter::create();
      filter_cpp->set_name("C/C++ files");
      filter_cpp->add_mime_type("text/x-c");
      filter_cpp->add_mime_type("text/x-c++");
      filter_cpp->add_mime_type("text/x-c-header");
        dialog.add_filter(filter_cpp);
     */
    Glib::RefPtr<Gtk::FileFilter> filter_image = Gtk::FileFilter::create();
    filter_image->set_name("Images");
    filter_image->add_mime_type("image/jpg");
    filter_image->add_pattern("*.jpg");
    dialog.add_filter(filter_image);

    //  Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
    //  filter_any->set_name("Any files");
    //  filter_any->add_pattern("*");
    //  dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch (result)
    {
    case(Gtk::RESPONSE_OK):
    {
        std::cout << "Open clicked." << std::endl;

        //Notice that this is a std::string, not a Glib::ustring.
        std::string filename = dialog.get_filename();
        std::cout << "File selected: " << filename << std::endl;
        _filename = filename;
        show_img();
        break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
        std::cout << "Cancel clicked." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }
}

void cForm::show_img()
{
    
    
    
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    pixbuf = Gdk::Pixbuf::create_from_file(_filename, 300, 300);
    m_image.set(pixbuf);
    m_image.show();
//    m_image.set(_filename);
//    m_image.set_size_request(300, 200);
//    std::cerr << m_image.get_pixel_size() << std::endl;
    //    add(m_image);
    /*try
 {
   // The fractal image has been created by the XaoS program.
   // http://xaos.sourceforge.net
   m_image = Gdk::Pixbuf::create_from_file(_filename.c_str());
    
 }
 catch(const Glib::FileError& ex)
 {
   std::cerr << "FileError: " << ex.what() << std::endl;
 }
 catch(const Gdk::PixbufError& ex)
 {
   std::cerr << "PixbufError: " << ex.what() << std::endl;
 }

 // Show at least a quarter of the image.
 if (m_image)
   set_size_request(m_image->get_width()/2, m_image->get_height()/2);
     */
}

/*bool cForm::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if (!m_image)
    return false;

  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  // Draw the image in the middle of the drawing area, or (if the image is
  // larger than the drawing area) draw the middle part of the image.
  Gdk::Cairo::set_source_pixbuf(cr, m_image,
    (width - m_image->get_width())/2, (height - m_image->get_height())/2);
  cr->paint();

  return true;
}
 */

