/* 
 * File:   Form.h
 * Author: morwin
 *
 * Created on 4 липня 2012, 10:02
 */

#pragma once
#include <gtkmm-3.0/gtkmm.h>
#include<gdkmm-3.0/gdkmm.h>

class cForm : public Gtk::Window
{
public:
    cForm();
    virtual ~cForm();
private:
    //Обработчик сигналов:
    virtual void _OnButtonClicked();
    void _OnButtonFileClicked();
    void _OnButtonFolderClicked();
    void _ShowImg();
    void _BuildButtonBox();
    void _BuildImageBoxes();

    //Child widgets:
    //    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

    Gtk::Image _m_image;
    Gtk::Image _m_face;
    //    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
    Gtk::Box _m_ButtonBox;
    Gtk::Box _m_ButtonBoxWrap;
    Gtk::Box _m_ImageOriginalBoxWrap;
    Gtk::Box _m_ImageFaceBoxWrap;
    Gtk::ButtonBox _m_MainBox;
    Gtk::Button _m_Button_File, _m_Button_Folder;
    Gtk::Fixed _m_fixed;
    Gtk::Label _m_original_image_label;
    Gtk::Label _m_face_image_label;
    Gtk::Box _m_ImageOriginalLabelBox;
    Gtk::Box _m_ImageFaceLabelBox;

    //Виджет объекта:
    Gtk::Button _m_button;
    Gtk::Button _m_find_face;
    std::string _filename;
};


