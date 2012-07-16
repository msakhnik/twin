#include "Form.h"
#include <iostream>
#include <gtkmm-2.4/gtkmm.h>
#include <gtkmm-3.0/gtkmm/enums.h>
#include <gdkmm-2.4/gdkmm/pixbuf.h>

using namespace std;

cForm::cForm(cFaceDetector & face) :
//    m_button("Hello World"), // creates a new button with label "Hello World".
m_button_file("Choose Photo"),
m_button_folder("Choose Folder"),
m_button_find_face("Find Face"),
m_frame_vertical_center("Original Image"),
m_frame_vertical_left("Controls"),
m_frame_vertical_right("Face"),
_filename(""),
_face(face)
{
    set_title("Face detector");
    
    add(m_main_box);
    m_main_box.set_size_request(650, 350);
    m_main_box.add(m_left_box);
    m_main_box.add(m_center_box);
    m_main_box.add(m_right_box);

    _BuildLeftBox();
    _BuildCenterBox();
    _BuildRightBox();

    m_button_folder.set_sensitive(false);
    m_button_find_face.set_sensitive(false);

    m_button_file.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &cForm::on_button_file_clicked));
    m_button_folder.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &cForm::on_button_folder_clicked));
    m_button_find_face.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &cForm::on_button_find_face_clicked));

    show_all_children();
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
    dialog.set_current_folder("../data/test_img");

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    Gtk::FileFilter filter_image;
    filter_image.set_name("Images");
    filter_image.add_mime_type("image/jpg");
    filter_image.add_pattern("*.jpg");
    dialog.add_filter(filter_image);

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
        _ShowOriginalImg();
        m_button_find_face.set_sensitive(true);
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

void cForm::on_button_find_face_clicked()
{
    if (_filename.size())
    {
        _face.FindFace(_filename.c_str());
        vector<int> temp_vector;
        temp_vector = _face.GetFaces();

    }
    else
        Gtk::MessageDialog dialog(*this, "Image not found",
                                  false /* use_markup */, Gtk::MESSAGE_ERROR,
                                  Gtk::BUTTONS_OK);
}

void cForm::_BuildLeftBox()
{
    m_left_box.pack_start(m_hbox_wrap_left, Gtk::PACK_SHRINK, 10);
    m_hbox_wrap_left.pack_start(m_frame_vertical_left, Gtk::PACK_SHRINK, 10);

    m_frame_vertical_left.set_size_request(140, 150);
    m_frame_vertical_left.set_tooltip_text("Controls");
    m_hbox_button_left.set_border_width(10);
    m_frame_vertical_left.add(m_hbox_button_left);
    m_button_box.add(m_button_file);
    m_button_box.add(m_button_find_face);
    m_button_box.add(m_button_folder);
    m_hbox_button_left.pack_start(m_button_box,
                                  Gtk::PACK_EXPAND_WIDGET, 5);
}

void cForm::_BuildCenterBox()
{
    m_center_box.pack_start(m_hbox_wrap_center, Gtk::PACK_SHRINK, 10);
    m_hbox_wrap_center.pack_start(m_frame_vertical_center, Gtk::PACK_SHRINK, 10);

    m_frame_vertical_center.set_size_request(300, 300);
    m_frame_vertical_center.add(m_image);
}

void cForm::_BuildRightBox()
{
    m_right_box.pack_start(m_hbox_wrap_right, Gtk::PACK_SHRINK, 10);
    m_hbox_wrap_right.pack_start(m_frame_vertical_right, Gtk::PACK_SHRINK, 10);

    m_frame_vertical_right.set_size_request(150, 150);
    m_frame_vertical_right.add(m_face);
    //    m_frame_vertical_right.add(m_image);
}

void cForm::_ShowOriginalImg()
{
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    pixbuf = Gdk::Pixbuf::create_from_file(_filename, 300, 300);
    m_image.set(pixbuf);
    m_image.show();
}

