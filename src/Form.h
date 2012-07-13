#include <gtkmm-2.4/gtkmm.h>
#include <string.h>
#include <FaceDetector.h>

class cFaceDetector;

class cForm : public Gtk::Window
{
public:
    cForm(cFaceDetector &);
    virtual ~cForm();

protected:
    void on_button_file_clicked();
    void on_button_folder_clicked();
    void on_button_clicked();
    Gtk::VButtonBox m_button_box;
    Gtk::Button m_button_file, m_button_folder, m_button_find_face;

    Gtk::VBox m_left_box, m_center_box, m_right_box;

    Gtk::HBox m_main_box, m_hbox_button_left, m_hbox_wrap_left;

    Gtk::HBox m_hbox_button_center, m_hbox_wrap_center;
    Gtk::Frame m_frame_vertical_center;

    Gtk::HBox m_hbox_button_right, m_hbox_wrap_right;
    Gtk::Frame m_frame_vertical_left, m_frame_vertical_right;

    Gtk::Image m_image, m_face;
private:
    std::string _filename;
    cFaceDetector* _face;
    void _BuildLeftBox();
    void _BuildCenterBox();
    void _BuildRightBox();
    void _ShowOriginalImg();
    void _Init();
};