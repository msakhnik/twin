#include "Form.h"
#include <iostream>
#include <gtkmm-2.4/gtkmm.h>
#include <gtkmm-3.0/gtkmm/enums.h>
#include <gdkmm-2.4/gdkmm/pixbuf.h>

using namespace std;
using Glib::ustring;

cForm::cForm(cFaceDetector & face) :
m_button_add_to_train("Train Files"),
m_button_folder("Force Folder"),
m_frame_vertical_center("Original Image"),
m_frame_vertical_left("Controls"),
_face(face),
_min_value(0.0),
_min_filename(""),
        _directory_name("") 
{
    set_title("Twin");

    add(m_main_box);
    m_main_box.set_size_request(550, 350);
    m_main_box.add(m_left_box);
    m_main_box.add(m_center_box);

    _BuildLeftBox();
    _BuildCenterBox();

    m_button_add_to_train.signal_clicked().connect(sigc::mem_fun(*this,
                                                                 &cForm::on_button_choose_dialog_cliked));
    m_button_add_to_train.signal_released().connect(sigc::mem_fun(*this,
                                                                 &cForm::on_train));

    m_button_folder.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &cForm::on_button_choose_dialog_cliked));
    m_button_folder.signal_released().connect(sigc::mem_fun(*this,
                                                                 &cForm::on_get_answer));

    show_all_children();
}

cForm::~cForm()
{
}

void cForm::on_button_choose_dialog_cliked()
{
    Gtk::FileChooserDialog dialog("Please choose a folder",
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*this);
    dialog.set_current_folder("../data");

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);
    int result = dialog.run();
    switch (result)
    {
    case(Gtk::RESPONSE_OK):
    {
        cerr << "Folder selected: " << dialog.get_filename()
                << endl;
        _directory_name = dialog.get_filename();
        break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
        cerr << "Cancel clicked." << endl;
        break;
    }
    default:
    {
        cerr << "Unexpected button clicked." << endl;
        break;
    }
    }
    dialog.hide_all();
}

void cForm::_Train()
{
    try
    {
        _recognizer.Train();
        Gtk::MessageDialog dialog(*this, "Train Saccesfully",
                                  false /* use_markup */, Gtk::MESSAGE_INFO,
                                  Gtk::BUTTONS_OK);
        dialog.run();
    }
    catch (cv::Exception& e)
    {
        cout << endl << e.what() << endl;
        Gtk::MessageDialog dialog(*this, "Train Failed",
                                  false /* use_markup */, Gtk::MESSAGE_ERROR,
                                  Gtk::BUTTONS_OK);
        dialog.set_secondary_text(
                                  e.what());
        dialog.run();
    }
}

void cForm::on_train()
{
    if (!_directory_name.empty())
    {
        _AddTrainFiles(_directory_name);
        _Train();
        _directory_name.clear();
    }
}

void cForm::on_get_answer()
{
    if (!_directory_name.empty())
    {
        _ResultProcess(_directory_name);
        cerr << _min_filename << endl;
        _ShowOriginalImg();
        Gtk::MessageDialog message(*this, "Thresold:",
                                   false /* use_markup */, Gtk::MESSAGE_INFO,
                                   Gtk::BUTTONS_OK);
        Glib::ustring text = ustring::format(std::fixed, std::setprecision(2), _min_value);
        message.set_secondary_text(text);
        message.run();
    }
}

void cForm::_BuildLeftBox()
{
    m_left_box.pack_start(m_hbox_wrap_left, Gtk::PACK_SHRINK, 10);
    m_hbox_wrap_left.pack_start(m_frame_vertical_left, Gtk::PACK_SHRINK, 10);

    m_frame_vertical_left.set_size_request(160, 120);
    m_frame_vertical_left.set_tooltip_text("Controls");
    m_hbox_button_left.set_border_width(10);
    m_frame_vertical_left.add(m_hbox_button_left);
    m_button_box.add(m_button_add_to_train);
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

void cForm::_ShowOriginalImg()
{
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    pixbuf = Gdk::Pixbuf::create_from_file(_min_filename, 300, 300);
    m_image.set(pixbuf);
    m_image.show();
}

void cForm::_ResultProcess(string & filename)
{
    boost::filesystem::path dir(filename.c_str());
    bool first_input = true;
    for (boost::filesystem::directory_iterator it(dir), end; it != end; ++it)
    {
        if (it->path().extension() == ".jpg")
        {
            cerr << "File in process - " << *it << endl;
            _face.FindFace(it->path().string().c_str());
            while (_face.InFaceArrayRange())
            {
                double op = _recognizer.GetAnswer(_face.GetFaces());
                if (first_input || _min_value > op)
                {
                    _min_value = op;
                    _min_filename = it->path().string().c_str();
                    first_input = false;
                }
            }
        }
    }
}

void cForm::_AddTrainFiles(string& filename)
{
    boost::filesystem::path dir(filename.c_str());
    try
    {
        for (boost::filesystem::directory_iterator it(dir), end; it != end; ++it)
        {
            if (it->path().extension() == ".jpg")
            {
                cerr << "File in process - " << *it << endl;
                _face.FindFace(it->path().string().c_str());
                while (_face.InFaceArrayRange())
                {
                    _recognizer.AddTrainImage(_face.GetFaces(), 0);
                }
            }
        }
    }
    catch (cv::Exception& e)
    {
        cout << endl << e.what() << endl;
        Gtk::MessageDialog dialog(*this, "Train Failed",
                                  false /* use_markup */, Gtk::MESSAGE_ERROR,
                                  Gtk::BUTTONS_OK);
        dialog.set_secondary_text(
                                  e.what());
        dialog.run();
    }
}