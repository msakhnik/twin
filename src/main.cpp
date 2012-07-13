/* 
 * File:   main.cpp
 * Author: morwin
 *
 * Created on 22 червня 2012, 21:09
 */

#include <cstdlib>
#include "AnnTrain.h"
#include "Form.h"
#include "FaceDetector.h"

//s#include <gtkmm/main.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    Gtk::Main kit(argc, argv);
    cFaceDetector face;
    
    cForm form(face);
    Gtk::Main::run(form);
    return 0;
}

