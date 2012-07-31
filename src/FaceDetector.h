
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

using namespace cv;

class cFaceDetector
{
public:
    cFaceDetector();
    bool Init();
    bool FindFace(const char * image);
    bool InFaceArrayRange();
    cv::Mat& GetFaces();
    virtual ~cFaceDetector();
    cv::Mat& GetImage(){ return _image; };
private:
    cv::Mat _image;
    cv::Mat _gray;
    cv::Mat _smallImg;
    cv::Mat _smallImgCopy;
    cv::Mat _retImage;
    cv::CascadeClassifier _cascade;
    cv::CascadeClassifier _nestedCascade;
    double _scale;
    std::vector<cv::Rect> _faces;
    std::vector<cv::Rect>::iterator _r;
    cv::Rect _rect;
    int _tmp;
    int _size;
    std::vector<int> _data;
    vector<cv::Rect> _nestedObjects;


    void _DetectFace();
    void _DrawFace();
    void _ConvertImage();
    std::string _haar_cascade_name[4];
    cv::Scalar _colors[8];
};