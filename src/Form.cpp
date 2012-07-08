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
_m_image("Big picture"),
_m_ButtonBox(Gtk::ORIENTATION_VERTICAL),
_m_ImageOriginalBoxWrap(Gtk::ORIENTATION_VERTICAL),
_m_ImageFaceBoxWrap(Gtk::ORIENTATION_VERTICAL),
_m_MainBox(Gtk::ORIENTATION_HORIZONTAL),
_m_Button_File("Load Image"),
_m_Button_Folder("Choose Folder"),
_m_original_image_label("Original image"),
_m_face_image_label("Face"),
_m_button("Hello World"),
_m_find_face("Find Face")
// Создает новую кнопку с надписью "Hello World". 
{
    //Устанавливает рамку окна
    set_title("Face detection");

    add(_m_MainBox);
    _m_MainBox.set_size_request(500, 50);
    _m_MainBox.pack_start(_m_ButtonBoxWrap, true, false, 0);
    _m_MainBox.pack_start(_m_ImageOriginalBoxWrap,  true, false, 0);
    _m_MainBox.pack_start(_m_ImageFaceBoxWrap,  true, false, 0);
    _m_MainBox.pack_start(_m_ImageFaceBoxWrap,  true, false, 0);
    _BuildButtonBox();
    _BuildImageBoxes();
    show_all_children();
}

cForm::~cForm()
{
}

void cForm::_OnButtonClicked()
{
    std::cout << "Hello World" << std::endl;
}

void cForm::_OnButtonFolderClicked()
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

void cForm::_OnButtonFileClicked()
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
         _ShowImg();
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

void cForm:: _ShowImg()
{
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    pixbuf = Gdk::Pixbuf::create_from_file(_filename, 300, 300);
    _m_image.set(pixbuf);
    _m_image.show();
}

void cForm::_BuildButtonBox()
{
    _m_ButtonBoxWrap.set_size_request(110, 300);
    _m_ButtonBoxWrap.pack_start(_m_ButtonBox);
    _m_ButtonBox.set_border_width(10);
    _m_ButtonBox.set_size_request(110, 100);
    _m_ButtonBox.set_halign(Gtk::ALIGN_START);
    _m_ButtonBox.set_valign(Gtk::ALIGN_START);
    
    _m_Button_File.set_size_request(110, 20);
    _m_Button_File.set_border_width(0);
    _m_Button_File.set_halign(Gtk::ALIGN_START);
    _m_Button_File.set_valign(Gtk::ALIGN_START);
    _m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &cForm::_OnButtonFileClicked));
    _m_ButtonBox.pack_start(_m_Button_File);

    _m_find_face.set_size_request(110, 20);
    _m_find_face.set_border_width(0);
    _m_find_face.set_halign(Gtk::ALIGN_START);
    _m_find_face.set_valign(Gtk::ALIGN_START);
    _m_find_face.signal_clicked().connect(sigc::mem_fun(*this, &cForm::_OnButtonClicked));
    _m_ButtonBox.pack_start(_m_find_face);

    _m_Button_Folder.set_size_request(110, 20);
    _m_Button_Folder.set_border_width(0);
    _m_Button_Folder.set_halign(Gtk::ALIGN_START);
    _m_Button_Folder.set_valign(Gtk::ALIGN_START);
    _m_Button_Folder.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &cForm::_OnButtonFolderClicked));
    _m_Button_Folder.set_sensitive(false);
    _m_ButtonBox.pack_start(_m_Button_Folder);
    
    _m_button.set_size_request(110, 20);
    _m_button.set_border_width(0);
    _m_button.set_halign(Gtk::ALIGN_START);
    _m_button.set_valign(Gtk::ALIGN_START);
    _m_button.signal_clicked().connect(sigc::mem_fun(*this, &cForm::_OnButtonClicked));
    _m_ButtonBox.pack_start(_m_button);

}

void cForm::_BuildImageBoxes()
{
    _m_ImageOriginalBoxWrap.set_size_request(300, 300);
    _m_ImageOriginalBoxWrap.pack_start(_m_ImageOriginalLabelBox);

    _m_ImageOriginalLabelBox.set_border_width(10);
    _m_ImageOriginalLabelBox.set_size_request(300, 20);
    _m_ImageOriginalLabelBox.set_halign(Gtk::ALIGN_START);
    _m_ImageOriginalLabelBox.set_valign(Gtk::ALIGN_START);
    _m_ImageOriginalLabelBox.pack_start(_m_original_image_label);
    _m_ImageOriginalBoxWrap.pack_start(_m_image, true, false, 0);

    _m_ImageFaceBoxWrap.pack_start(_m_ImageFaceLabelBox);

    _m_ImageFaceLabelBox.set_border_width(10);
//    _m_ImageFaceLabelBox.set_size_request(300, 20);
    _m_ImageFaceLabelBox.set_halign(Gtk::ALIGN_CENTER);
    _m_ImageFaceLabelBox.set_valign(Gtk::ALIGN_START);
    _m_ImageFaceLabelBox.pack_start(_m_face_image_label);
    _m_ImageFaceBoxWrap.pack_start(_m_image, true, false, 0);
}