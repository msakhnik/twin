#include "Form.h"
#include <iostream>
#include <gtkmm-2.4/gtkmm.h>
#include <gtkmm-3.0/gtkmm/enums.h>
#include <gdkmm-2.4/gdkmm/pixbuf.h>

using namespace std;
using Glib::ustring;

cForm::cForm(cFaceDetector & face) :
m_button_file("Load Image"),
m_button_add_to_train("Add To Train"),
m_button_train("Train"),
m_button_folder("Force Folder"),
m_frame_vertical_center("Original Image"),
m_frame_vertical_left("Controls"),
m_frame_vertical_right("Image in train"),
_filename(""),
_face(face),
_min_value(0.0),
    _min_filename("")
{
    set_title("Face detector");

    add(m_main_box);
    m_main_box.set_size_request(750, 350);
    m_main_box.add(m_left_box);
    m_main_box.add(m_center_box);
    m_main_box.add(m_right_box);

    _BuildLeftBox();
    _BuildCenterBox();
    _BuildRightBox();

    m_button_file.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &cForm::on_button_file_clicked));

    m_button_add_to_train.signal_clicked().connect(sigc::mem_fun(*this,
                                                                 &cForm::on_button_add_to_train_clicked));

    m_button_train.signal_clicked().connect(sigc::mem_fun(*this,
                                                          &cForm::on_button_train_clicked));

    m_button_folder.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &cForm::on_button_folder_clicked));
    //    m_button_find_face.signal_clicked().connect(sigc::mem_fun(*this,
    //                                                              &cForm::on_button_find_face_clicked));
    //    m_button_getanswer.signal_clicked().connect(sigc::mem_fun(*this,
    //                                                              &cForm::on_button_getanswer_clicked));

    show_all_children();
}

cForm::~cForm()
{
}

void cForm::on_button_add_to_train_clicked()
{
    if (!_filename.empty())
    {
        _face.FindFace(_filename.c_str());
        while (_face.InFaceArrayRange())
        {
            _recognizer.AddTrainImage(_face.GetFaces(), 0);
        }
        _AddRow();
    }
    _filename.clear();
}

void cForm::on_button_train_clicked()
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

void cForm::on_button_folder_clicked()
{
    Gtk::FileChooserDialog dialog("Please choose a folder",
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*this);
    dialog.set_current_folder("../data");

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
        string filename = dialog.get_filename();
        _ResultProcess(filename);
        _filename = _min_filename;
        cerr << _min_filename << endl;
        _ShowOriginalImg();
         Gtk::MessageDialog message(*this, "Thresold:",
                                  false /* use_markup */, Gtk::MESSAGE_INFO,
                                  Gtk::BUTTONS_OK);
         Glib::ustring text = ustring::format(std::fixed, std::setprecision(2), _min_value);
         message.set_secondary_text(text) ;
       message.run();
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
    dialog.set_current_folder("../data");

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    Gtk::FileFilter filter_image;
    filter_image.set_name("Images");
    filter_image.add_mime_type("image/jpg");
    filter_image.add_pattern("*.jpg");
    dialog.add_filter(filter_image);

    int result = dialog.run();

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

void cForm::_BuildLeftBox()
{
    m_left_box.pack_start(m_hbox_wrap_left, Gtk::PACK_SHRINK, 10);
    m_hbox_wrap_left.pack_start(m_frame_vertical_left, Gtk::PACK_SHRINK, 10);

    m_frame_vertical_left.set_size_request(160, 250);
    m_frame_vertical_left.set_tooltip_text("Controls");
    m_hbox_button_left.set_border_width(10);
    m_frame_vertical_left.add(m_hbox_button_left);
    m_button_box.add(m_button_file);
    m_button_box.add(m_button_add_to_train);
    m_button_box.add(m_button_train);
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

    m_frame_vertical_right.set_size_request(150, 220);
    m_frame_vertical_right.add(m_VPaned);
    m_VPaned.add(m_ScrolledWindow);

    m_refListStore = Gtk::ListStore::create(m_Columns);

    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_ScrolledWindow.add(m_TreeView);
    m_TreeView.set_model(m_refListStore);
    m_TreeView.append_column("№", m_Columns.m_col_num);
    m_TreeView.append_column("Image", m_Columns.m_col_text);

    show_all_children();
}

void cForm::_ShowOriginalImg()
{
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    pixbuf = Gdk::Pixbuf::create_from_file(_filename, 300, 300);
    m_image.set(pixbuf);
    m_image.show();
}

void cForm::_AddRow()
{
    static int i = 0;
    string number;
    std::stringstream ss;
    ss << i;
    ss >> number;
    m_row = *(m_refListStore->append());
    m_row[m_Columns.m_col_num] = number;
    m_row[m_Columns.m_col_text] = _filename;
    ++i;
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