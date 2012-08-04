#include <gtest/gtest.h>
#include "../src/FaceRecognizer.h"
#include "../src/FaceDetector.h"
#include <iostream>
#include <numeric>

using namespace std;

cFaceRecognizer recognizer;
cFaceDetector face;

TEST(cFaceRecognizer, AddImage)
{
    face.FindFace("../data/misha/1.jpg");
    while(face.InFaceArrayRange())
    {
        cv::Mat img;
        img = face.GetFaces();
        recognizer.AddTrainImage(img, 0);
//        cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
//        imshow("face", img);
//        cv::waitKey();
    }
    face.FindFace("../data/misha/2.jpg");
    while(face.InFaceArrayRange())
    {
        cv::Mat img;
        img = face.GetFaces();
        recognizer.AddTrainImage(img, 0);
//        cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
//        imshow("face", img);
//        cv::waitKey();
    }
    face.FindFace("../data/misha/3.jpg");
    while(face.InFaceArrayRange())
    {
        cv::Mat img;
        img = face.GetFaces();
        recognizer.AddTrainImage(img, 0);
//        cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
//        imshow("face", img);
//        cv::waitKey();
    }
    face.FindFace("../data/misha/4.jpg");
    while(face.InFaceArrayRange())
    {
        cv::Mat img;
        img = face.GetFaces();
        recognizer.AddTrainImage(img, 0);
//        cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
//        imshow("face", img);
//        cv::waitKey();
    }
    face.FindFace("../data/misha/5.jpg");
    while(face.InFaceArrayRange())
    {
        cv::Mat img;
        img = face.GetFaces();
        recognizer.AddTrainImage(img, 0);
//        cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
//        imshow("face", img);
//        cv::waitKey();
    }
}

TEST(cFaceRecognizer, Train)
{
    ASSERT_NO_THROW(recognizer.Train());
}

TEST(cFaceRecognizer, GetAnswer)
{
    face.FindFace("../data/test_img/dfgswef.jpg");
    while(face.InFaceArrayRange())
    {
        cv::Mat img;
        img = face.GetFaces();
        cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
        imshow("face", img);
        cv::waitKey();
        int op = recognizer.GetAnswer(img);
        cerr << op << endl;
    }
}
