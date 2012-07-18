#include <gtest/gtest.h>
#include "../src/FaceRecognizer.h"
#include <iostream>
#include <numeric>

using namespace std;

cFaceRecognizer recognizer;

TEST(cFaceRecognizer, Recognizer)
{
    ASSERT_TRUE(recognizer.Init());
}
