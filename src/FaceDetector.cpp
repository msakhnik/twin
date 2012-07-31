
#include "FaceDetector.h"
#include <iterator>
#include <algorithm>
#include <ncurses.h>

using namespace std;
using namespace cv;

cFaceDetector::cFaceDetector() :
_scale(1),
_size(200)
{
    _haar_cascade_name[0] = "../data/haarcascades/haarcascade_frontalface_alt_tree.xml";
    _haar_cascade_name[1] = "../data/haarcascades/haarcascade_frontalface_default.xml";
    _haar_cascade_name[2] = "../data/haarcascades/haarcascade_frontalface_alt.xml";
    _haar_cascade_name[3] = "../data/haarcascades/haarcascade_frontalface_alt2.xml";
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
    if (!_cascade.load(_haar_cascade_name[0]) ||
            !_nestedCascade.load(_haar_cascade_name[0]))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Check you cascade files in a data directory\n" << endl;
        return false;
    }

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
    _r = _faces.begin();

    return true;
}

cFaceDetector::~cFaceDetector()
{
}

void cFaceDetector::_DetectFace()
{
    double t = 0;
    Mat smallImg(cvRound(_image.rows / _scale), cvRound(_image.cols / _scale),
                 CV_8UC1);
    _smallImg = smallImg;

    cvtColor(_image, _gray, CV_BGR2GRAY);
    resize(_gray, _smallImg, _smallImg.size(), 0, 0, INTER_LINEAR);
    equalizeHist(_smallImg, _smallImg);

    t = (double) cvGetTickCount();
    _cascade.detectMultiScale(_smallImg, _faces,
                              1.1, 2, 0
                              | CV_HAAR_SCALE_IMAGE,
                              Size(_size, _size));
    t = (double) cvGetTickCount() - t;
    cerr << "detection time = " << (t / ((double) cvGetTickFrequency()*1000.))
            << " ms" << endl;
}

bool cFaceDetector::InFaceArrayRange()
{
    if (_r == _faces.end())
        return false;
    int i = 0;
    do
    {
        if (!_nestedCascade.load(_haar_cascade_name[i]))
        {
            cerr << "ERROR: Could not load classifier cascade" << endl;
            cerr << "Check you cascade files in a data directory\n" << endl;
            return false;
        }
        _nestedCascade.detectMultiScale(_smallImg(*_r), _nestedObjects,
                                        1.1, 2, 0 | CV_HAAR_SCALE_IMAGE,
                                        Size(_size, _size));

        ++i;
    }
    while (_nestedObjects.size() == 0 && i <= 3);

    return true;
}

Mat& cFaceDetector::GetFaces()
{
    for (vector<Rect>::const_iterator nr = _nestedObjects.begin(); nr != _nestedObjects.end(); nr++)
    {
        _rect.x = cvRound((_r->x + nr->x) * _scale);
        _rect.y = cvRound((_r->y + nr->y) * _scale);
        _rect.width = cvRound((nr->width + nr->height) * 0.5 * _scale);
        _rect.height = cvRound((nr->width + nr->height)*0.5 * _scale);
        cerr << _rect.width << ";" << _rect.height << endl;

        _ConvertImage();
    }
    _r++;
    _nestedObjects.empty();

    return _retImage;
}

void cFaceDetector::_ConvertImage()
{
    Mat image = _image(_rect);
    cv::resize(image, _retImage, Size(_size, _size), 1, 1);
    cv::cvtColor(_retImage, _retImage, CV_BGR2GRAY);
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
                                        | CV_HAAR_DO_CANNY_PRUNING,
                                        Size(30, 30));
        for (vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
        {
            center.x = cvRound((r->x + nr->x + nr->width * 0.5) * _scale);
            center.y = cvRound((r->y + nr->y + nr->height * 0.5) * _scale);
            radius = cvRound((nr->width + nr->height)*0.25 * _scale);
            //                        circle( _image, center, radius, color, 3, 8, 0 );
        }
    }
}