#pragma once;

#include "opencv2/opencv.hpp"
#include <string.h>
#include <vector>

class cFaceRecognizer
{
public:
    cFaceRecognizer();
    virtual ~cFaceRecognizer();
    bool AddTrainImage(std::string filename, int label);
    bool RemoveLastTrainImage();
    void Clear(); 
    bool Init(){ return true; };
    void Train();
    int GetAnswer(std::string filename);
    void SetThreshold(double number) { _threshold = number; };
    double GetThreshold() { return _threshold; };
private:
    std::vector<cv::Mat> _images;
    std::vector<int> _labels;
    cv::Ptr<cv::FaceRecognizer> _model;
    double _threshold;
};