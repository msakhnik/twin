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
    virtual void on_button_clicked();
    void on_button_file_clicked();
    void on_button_folder_clicked();
    void show_img();

    //Child widgets:
    //    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

    Gtk::Image m_image;
    //    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
    Gtk::Box m_ButtonBox;
    Gtk::Box m_ImageBox;
    Gtk::ButtonBox m_MainBox;
    Gtk::Button m_Button_File, m_Button_Folder;
    Gtk::Fixed m_fixed;

    //Виджет объекта:
    Gtk::Button m_button;
    std::string _filename;
};


