#include <gtest/gtest.h>
#include "../src/FaceDetector.h"

cFaceDetector face;

TEST(cFaceDetector, Initialize)
{
    ASSERT_TRUE(face.Init());
}

TEST(cFaceDetector, FindFace)
{
    ASSERT_TRUE(face.FindFace("../data/test_img/1.jpg"));
}
