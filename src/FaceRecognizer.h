/* 
 * File:   FaceRecognizer.h
 * Author: morwin
 *
 * Created on 18 липня 2012, 17:09
 */

#pragma once;

class cFaceRecognizer
{
public:
    cFaceRecognizer();
    cFaceRecognizer(const cFaceRecognizer& orig);
    virtual ~cFaceRecognizer();
    bool Init(){ return true; };
private:

};