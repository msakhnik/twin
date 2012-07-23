#include "FaceRecognizer.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include <string.h>
#include <vector>

using namespace std;
using namespace cv;

cFaceRecognizer::cFaceRecognizer() :
_threshold(19000.0)
{
    cerr << "In cFaceRecognizer" << endl;
}

cFaceRecognizer::~cFaceRecognizer()
{
}

bool cFaceRecognizer::RemoveLastTrainImage()
{
    if (_images.size() == 0)
    {
        cerr << "Data array is empty" << endl;
        return false;
    }

    _images.pop_back();
    _labels.pop_back();

    return true;
}

bool cFaceRecognizer::AddTrainImage(Mat& img, int label)
{
    cerr << "Add image " << endl;
    if (img.empty())
    {
        cerr << "Image not found" << endl;
        return false;
    }
    _images.push_back(img);
    _labels.push_back(label);

    return true;
}

void cFaceRecognizer::Clear()
{
    _images.clear();
    _labels.clear();
}

void cFaceRecognizer::Train()
{
//    _model = createEigenFaceRecognizer(_images.size(), _threshold);
        _model = createEigenFaceRecognizer();
    _model->train(_images, _labels);
}

double cFaceRecognizer::GetAnswer(Mat& testImage)
{
//    Mat testImage = imread(filename.c_str(), 0);

    if (testImage.empty())
    {
        cerr << "Image is empty" << endl;
        return -1;
    }
//    cerr << testImage.cols << endl;

        int predictedLabel = 1;
        double confidence = 0.0;
        
        predictedLabel = 0;
        confidence = 0.0;
        _model->predict(testImage, predictedLabel, confidence);
    //    cerr << confidence << endl;

    return confidence;
}
