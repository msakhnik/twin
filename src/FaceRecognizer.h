#pragma once

#include "opencv2/opencv.hpp"
#include <string.h>
#include <vector>

class cFaceRecognizer
{
public:
    cFaceRecognizer();
    virtual ~cFaceRecognizer();
    bool AddTrainImage(cv::Mat& image, int label);
    bool RemoveLastTrainImage();
    void Clear(); 
    bool Init(){ return true; };
    void Train();
    int GetAnswer(std::string filename);
    void SetThreshold(double number) { _threshold = number; };
    double GetThreshold() { return _threshold; };
    bool ValidateImageArray(unsigned int image_number) { return (image_number >=0 && image_number < _images.size()); };
    cv::Mat GetImage(int image_number) { return _images.at(image_number); };
    unsigned int GetImageSize() { return _images.size(); };
private:
    std::vector<cv::Mat> _images;
    std::vector<int> _labels;
    cv::Ptr<cv::FaceRecognizer> _model;
    double _threshold;
};