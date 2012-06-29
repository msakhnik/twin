#include <gtest/gtest.h>
#include "../src/FaceDetector.h"
#include <iostream>

using namespace std;

cFaceDetector face;


TEST(cFaceDetector, Initialize)
{
    ASSERT_TRUE(face.Init());
}

TEST(cFaceDetector, FindFace)
{
    ASSERT_TRUE(face.FindFace("../data/test_img/4.jpg"));
}

TEST(cFaceDetector, InFaceArrayRange)
{
    ASSERT_TRUE(face.InFaceArrayRange());
}

TEST(cFaceDetector, GetFaces)
{
    vector<int> temp_vector;
    while (face.InFaceArrayRange())
    {
        temp_vector = face.GetFaces();
    }
}
