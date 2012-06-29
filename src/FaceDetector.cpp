/* 
 * File:   FaceDetector.cpp
 * Author: morwin
 * 
 * Created on 23 червня 2012, 0:12
 */

#include "FaceDetector.h"
#include <iterator>
#include <algorithm>

using namespace std;
using namespace cv;

cFaceDetector::cFaceDetector() :
_cascadeName("../data/haarcascades/haarcascade_frontalface_alt.xml"),
_nestedCascadeName("../data/haarcascades/haarcascade_frontalface_alt.xml"),
_capture(0),
_scale(1)
{
    _colors[1] = CV_RGB(0, 0, 255);
    _colors[2] = CV_RGB(0, 128, 255);
    _colors[3] = CV_RGB(0, 255, 255);
    _colors[4] = CV_RGB(0, 255, 0);
    _colors[5] = CV_RGB(255, 128, 0);
    _colors[6] = CV_RGB(255, 255, 0);
    _colors[7] = CV_RGB(255, 0, 0);
    _colors[8] = CV_RGB(255, 0, 255);
}

bool cFaceDetector::Init()
{
    if (!_cascade.load(_cascadeName) || !_nestedCascade.load(_nestedCascadeName))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Check you cascade files in a data directory\n" << endl;
        return false;
    }

    cvNamedWindow("result", 1);

    return true;
}

bool cFaceDetector::FindFace(const char * image)
{
    _image = imread(image, 1);
    if (_image.empty())
    {
        cerr << "Image: <" << image << " >not found" << endl;
        return false;
    }

    _DetectFace();
    //    _DrawFace();
    _r = _faces.begin();
    imshow("result", _image);
    waitKey(0);

    return true;
}

cFaceDetector::~cFaceDetector()
{
}

void cFaceDetector::_DetectFace()
{
    double t = 0;
    Mat smallImg(cvRound(_image.rows / _scale), cvRound(_image.cols / _scale), CV_8UC1);
    _smallImg = smallImg;

    cvtColor(_image, _gray, CV_BGR2GRAY);
    resize(_gray, _smallImg, _smallImg.size(), 0, 0, INTER_LINEAR);
    equalizeHist(_smallImg, _smallImg);

    t = (double) cvGetTickCount();
    _cascade.detectMultiScale(_smallImg, _faces,
                              1.1, 2, 0
                              //|CV_HAAR_FIND_BIGGEST_OBJECT
                              //|CV_HAAR_DO_ROUGH_SEARCH
                              | CV_HAAR_SCALE_IMAGE
                              ,
                              Size(30, 30));
    t = (double) cvGetTickCount() - t;
    cerr << "detection time = " << (t / ((double) cvGetTickFrequency()*1000.)) << " ms" << endl;
}

bool cFaceDetector::InFaceArrayRange()
{
    return _r != _faces.end();
}

vector<int> cFaceDetector::GetFaces()
{
    cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
    vector<int> a(5, 1);
    vector<Rect> nestedObjects;
    _nestedCascade.detectMultiScale(_smallImg(*_r), nestedObjects,
                                    1.1, 2, 0 | CV_HAAR_SCALE_IMAGE,
                                    Size(30, 30));
    for (vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
    {
        _rect.x = cvRound((_r->x + nr->x) * _scale);
        _rect.y = cvRound((_r->y + nr->y) * _scale);
        _rect.width = cvRound((nr->width + nr->height) * 0.5 * _scale);
        _rect.height = cvRound((nr->width + nr->height)*0.5 * _scale);
        
//        rectangle(_image, _rect, _colors[2]);
        _ConvertImage();
//        _FillDataArray();
    }
    _r++;
    waitKey(0);
    return a;
}

void cFaceDetector::_ConvertImage()
{
    Mat image = _image(_rect);
    Mat image_gray;
    GaussianBlur( image, image, Size(3,3), 0, 0, BORDER_DEFAULT );
    cvtColor( image, image_gray, CV_RGB2GRAY );
    Mat sobel = image;
    Sobel(image_gray, sobel, image_gray.depth(), 1, 0, 3, 1, 0, BORDER_DEFAULT);
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    int ddepth = CV_16S;
    int scale = 1;
    int delta = 0;
    //finding max gradient
    Sobel( image_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    Sobel( image_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    convertScaleAbs( grad_y, abs_grad_y );
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel );
    Mat canny(image.rows, image.cols, CV_8UC1);
//    cvtColor( image, canny, CV_8UC1 );
    Canny(sobel, canny, 10, 100);
//    subtract(canny, sobel, sobel);
    cv::resize(sobel, _smallImgCopy, Size(150, 150), 1, 1);
    adaptiveThreshold(_smallImgCopy, _smallImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 3);
    imshow("face", _smallImg);
}


void cFaceDetector::_FillDataArray()
{
//    Mat image = _image(_rect);
    for (int i = 0; i < _smallImg.rows; ++i)
        for (int j = 0; j < _smallImg.cols; ++j)
            cerr << _smallImg.at<int>(i, j) << endl;
}

void cFaceDetector::_DrawFace()
{
    int i = 0;
    for (vector<Rect>::const_iterator r = _faces.begin(); r != _faces.end(); r++, i++)
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = _colors[i % 8];
        int radius;
        center.x = cvRound((r->x + r->width * 0.5) * _scale);
        center.y = cvRound((r->y + r->height * 0.5) * _scale);
        radius = cvRound((r->width + r->height)*0.25 * _scale);
        circle(_image, center, radius, color, 3, 8, 0);
        if (_nestedCascade.empty())
            continue;
        smallImgROI = _smallImg(*r);
        _nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
                                        1.1, 2, 0
                                        //|CV_HAAR_FIND_BIGGEST_OBJECT
                                        //|CV_HAAR_DO_ROUGH_SEARCH
                                        //|CV_HAAR_DO_CANNY_PRUNING
//                                        | CV_HAAR_SCALE_IMAGE
                                        |CV_HAAR_DO_CANNY_PRUNING
                                        ,
                                        Size(30, 30));
        for (vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
        {
            center.x = cvRound((r->x + nr->x + nr->width * 0.5) * _scale);
            center.y = cvRound((r->y + nr->y + nr->height * 0.5) * _scale);
            radius = cvRound((nr->width + nr->height)*0.25 * _scale);
            //            circle( _image, center, radius, color, 3, 8, 0 );
        }
    }
}