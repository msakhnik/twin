#include <gtkmm-2.4/gtkmm.h>
#include <string.h>
#include "FaceDetector.h"
#include "FaceRecognizer.h"
#include <boost/filesystem.hpp>

class cFaceDetector;
class cFaceRecognizer;

class cForm : public Gtk::Window
{
public:
    cForm(cFaceDetector &);
    virtual ~cForm();

protected:
    void on_train();
    void on_get_answer();
    void on_button_choose_dialog_cliked();

    Gtk::VButtonBox m_button_box;
    Gtk::Button m_button_add_to_train, m_button_folder;

    Gtk::VBox m_left_box, m_center_box;

    Gtk::HBox m_main_box, m_hbox_button_left, m_hbox_wrap_left;

    Gtk::HBox m_hbox_button_center, m_hbox_wrap_center;
    Gtk::Frame m_frame_vertical_center;

    Gtk::Frame m_frame_vertical_left;

    Gtk::Image m_image, m_list;
    Gtk::VPaned m_VPaned;
    Gtk::Label m_wait;
    Gtk::Window m_modal;
    

private:
    cFaceDetector & _face;
    cFaceRecognizer _recognizer;
    double _min_value;
    std::string _min_filename;
    std::string _directory_name;

    void _BuildLeftBox();
    void _BuildCenterBox();
    void _ShowOriginalImg();
    void _ResultProcess(std::string &);
    void _AddTrainFiles(std::string &);
    void _Train();
    void _OpenModalWindow();
};