
#include "FaceDetector.h"
#include <iterator>
#include <algorithm>
#include <ncurses.h>

using namespace std;
using namespace cv;

cFaceDetector::cFaceDetector() :
    _capture(0),
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
    if (!_cascade.load(_haar_cascade_name[0]) || !_nestedCascade.load(_haar_cascade_name[0]))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Check you cascade files in a data directory\n" << endl;
        return false;
    }

//    cvNamedWindow("result", 1);

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
    cvNamedWindow("result", 1);
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
                              Size(_size, _size));
    t = (double) cvGetTickCount() - t;
    cerr << "detection time = " << (t / ((double) cvGetTickFrequency()*1000.)) << " ms" << endl;
}

bool cFaceDetector::InFaceArrayRange()
{
    if (_r == _faces.end())
        return false;
    int i  = 1;
    while (true)
    {
        _nestedCascade.detectMultiScale(_smallImg(*_r), _nestedObjects,
                                        1.1, 2, 0 | CV_HAAR_SCALE_IMAGE,
                                        Size(_size, _size));
        if (_nestedObjects.size() == 0)
            if (i <= 3)
                if (!_nestedCascade.load(_haar_cascade_name[i]))
                {
                    cerr << "ERROR: Could not load classifier cascade" << endl;
                    cerr << "Check you cascade files in a data directory\n" << endl;
                    return false;
                }
                else
                    continue;
            else
                return false;
        else
            return true;
    }
}

Mat& cFaceDetector::GetFaces()
{
//    cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
    
    
    for (vector<Rect>::const_iterator nr = _nestedObjects.begin(); nr != _nestedObjects.end(); nr++)
    {
        _rect.x = cvRound((_r->x + nr->x) * _scale);
        _rect.y = cvRound((_r->y + nr->y) * _scale);
        _rect.width = cvRound((nr->width + nr->height) * 0.5 * _scale);
        _rect.height = cvRound((nr->width + nr->height)*0.5 * _scale);
        cerr << _rect.width << ";" << _rect.height << endl;
        
//        rectangle(_image, _rect, _colors[2]);
        _ConvertImage();
//        _FillDataArray();
        cerr << "1" << endl;
    }
    cerr << "2" << endl;
    _r++;
//    waitKey(0);
    _nestedObjects.empty();
    cerr << "3" << endl;
    return _retImage;
}

void cFaceDetector::_ConvertImage()
{
    Mat image = _image(_rect);
    cv::resize(image, _retImage, Size(_size, _size), 1, 1);
//    Mat image_gray;
//    cvtColor( image, image_gray, CV_RGB2GRAY );
//    Mat sobel = image_gray;
//    Sobel(image_gray, sobel, image_gray.depth(), 1, 0, 3, 1, 0, BORDER_DEFAULT);
//    Sobel(sobel, sobel, sobel.depth(), 1, 0, 3, 1, 0, BORDER_DEFAULT);
//    adaptiveThreshold(sobel, _smallImgCopy, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 25, 0);
//    threshold(sobel, _smallImgCopy, 128, 255, CV_ADAPTIVE_THRESH_MEAN_C);
//    adaptiveThreshold(sobel, _smallImgCopy, 8)
//    Canny(_smallImgCopy, _smallImgCopy, 10, 100);
//    imshow("face", sobel);
//    
//    cv::resize(image, image, Size(_size, _size), 1, 1);
//    Mat image_gray;
//    GaussianBlur( image, image, Size(3,3), 0, 0, BORDER_DEFAULT );
//    cvtColor( image, image_gray, CV_RGB2GRAY );
//    Mat sobel = image;
//    Sobel(image_gray, sobel, image_gray.depth(), 1, 0, 3, 1, 0, BORDER_DEFAULT);
//    Mat grad_x, grad_y;
//    Mat abs_grad_x, abs_grad_y;
//    int ddepth = CV_16S;
//    int scale = 2;
//    int delta = 1;
//    //finding max gradient
//    Sobel( image_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
//    Sobel( image_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
//    convertScaleAbs( grad_x, abs_grad_x );
//    convertScaleAbs( grad_y, abs_grad_y );
//    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel );
//    Mat canny(image.rows, image.cols, CV_8UC1);
////    cvtColor( image, canny, CV_8UC1 );
//    Canny(sobel, canny, 10, 100);
////    subtract(canny, sobel, sobel);
////    cv::resize(sobel, _smallImgCopy, Size(_size, _size), 1, 1);
//    adaptiveThreshold(sobel, _smallImgCopy, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 7);
////    _smallImg.release();
////    _smallImgCopy.release();
//    imshow("face", _smallImgCopy);
}

void cFaceDetector::_FillDataArray()
{
    _data.empty();
    for (int i = 0; i < _smallImgCopy.rows; ++i)
    {
        for (int j = 0; j < _smallImgCopy.cols; ++j)
            if (_smallImgCopy.at<bool>(i, j))
               _data.push_back(1);
            else
                _data.push_back(0);
    }
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

 cv::Mat& cFaceDetector::GetImage()
 {
     return _image;
 }