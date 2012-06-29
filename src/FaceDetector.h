/* 
 * File:   FaceDetector.h
 * Author: morwin
 *
 * Created on 23 червня 2012, 0:12
 */

#pragma once

#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>

class cFaceDetector
{
public:
    cFaceDetector();
    bool Init();
    bool FindFace(const char * image);
    bool InFaceArrayRange();
    std::vector<int> GetFaces();
    virtual ~cFaceDetector();
private:
    std::string _cascadeName;
    std::string _nestedCascadeName;
    CvCapture* _capture;
    cv::Mat _image;
    cv::Mat _gray;
    cv::Mat _smallImg;
    cv::Mat _smallImgCopy;
    cv::CascadeClassifier _cascade;
    cv::CascadeClassifier _nestedCascade;
    double _scale;
    std::vector<cv::Rect> _faces;
    std::vector<cv::Rect>::iterator _r;
    cv::Rect _rect;
    int _tmp;
    int _size;


    void _DetectFace();
    void _DrawFace();
    void _ConvertImage();
    void _FillDataArray();

    cv::Scalar _colors[8];
};

