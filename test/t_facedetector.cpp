#include <gtest/gtest.h>
#include "../src/FaceDetector.h"
#include <iostream>
#include <numeric>

using namespace std;

cFaceDetector face;

TEST(cFaceDetector, Init)
{
    ASSERT_TRUE(face.Init());
}

TEST(cFaceDetector, FindFace)
{
    ASSERT_TRUE(face.FindFace("../data/misha/2.jpg"));
}

TEST(cFaceDetector, GetFace)
{
    while(face.InFaceArrayRange())
    {
        cv::Mat img;
        img = face.GetFaces();
        cvNamedWindow("face", CV_WINDOW_AUTOSIZE);
        imshow("face", img);
        cv::waitKey();
    }
}