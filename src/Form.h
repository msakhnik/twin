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
    void on_button_file_clicked();
    void on_button_folder_clicked();
    void on_button_add_to_train_clicked();
    void on_button_train_clicked();

    void on_button_clicked();

    Gtk::VButtonBox m_button_box;
    Gtk::Button m_button_file, m_button_add_to_train, m_button_train, m_button_folder;

    Gtk::VBox m_left_box, m_center_box, m_right_box;

    Gtk::HBox m_main_box, m_hbox_button_left, m_hbox_wrap_left;

    Gtk::HBox m_hbox_button_center, m_hbox_wrap_center;
    Gtk::Frame m_frame_vertical_center;

    Gtk::HBox m_hbox_button_right, m_hbox_wrap_right;
    Gtk::Frame m_frame_vertical_left, m_frame_vertical_right;

    Gtk::Image m_image, m_list;
    Gtk::VPaned m_VPaned;
    Glib::RefPtr<Gtk::ListStore> m_refListStore; //The Tree Model.
    Gtk::TreeView m_TreeView; //The Tree View.
    Gtk::ScrolledWindow m_ScrolledWindow;

    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:

        ModelColumns()
        {
            add(m_col_num);
            add(m_col_text);
        }

        Gtk::TreeModelColumn<Glib::ustring> m_col_num;
        Gtk::TreeModelColumn<Glib::ustring> m_col_text;
    };
    ModelColumns m_Columns;
    Gtk::TreeModel::Row m_row;
    //        MessagesList m_MessagesList;;
private:
    std::string _filename;
    cFaceDetector & _face;
    cFaceRecognizer _recognizer;
    double _min_value;
    std::string _min_filename; 

    void _BuildLeftBox();
    void _BuildCenterBox();
    void _BuildRightBox();
    void _ShowOriginalImg();
    void _AddRow();
    void _RemoveRow();
    void _ResultProcess(std::string &);
};