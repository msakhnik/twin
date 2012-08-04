/* 
 * File:   main.cpp
 * Author: morwin
 *
 * Created on 22 червня 2012, 21:09
 */

#include <cstdlib>
#include "Form.h"
#include "FaceDetector.h"

//s#include <gtkmm/main.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    gdk_threads_enter();
    Gtk::Main kit(argc, argv);
    cFaceDetector face;
    cForm form(face);
    if (!face.Init())
    {
        Gtk::MessageDialog dialog(form, "Init error",
                                  false /* use_markup */, Gtk::MESSAGE_ERROR,
                                  Gtk::BUTTONS_OK);
        dialog.set_secondary_text(
                                  "Cannot init cFaceDetector class.");

        dialog.run();
    }
    else
    {
        Gtk::Main::run(form);
    }
    gdk_threads_leave();
    return 0;
}