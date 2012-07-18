#include <gtest/gtest.h>
#include "../src/FaceRecognizer.h"
#include <iostream>
#include <numeric>

using namespace std;

cFaceRecognizer recognizer;

TEST(cFaceRecognizer, Init)
{
    ASSERT_TRUE(recognizer.Init());
}

TEST(cFaceRecognizer, AddImage)
{
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/1.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/2.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/3.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/4.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/5.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/6.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/7.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/8.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/9.jpg", 0));
    ASSERT_TRUE(recognizer.AddTrainImage("../data/misha/12.jpg", 0));
    ASSERT_FALSE(recognizer.AddTrainImage("../data/misha/-1.jpg", 0));
}

TEST(cFaceRecognizer, PopImage)
{
    ASSERT_TRUE(recognizer.RemoveLastTrainImage());
}

TEST(cFaceRecognizer, Train)
{
    ASSERT_NO_THROW(recognizer.Train());
}

TEST(cFaceRecognizer, GetAnswer)
{
    ASSERT_EQ(0, recognizer.GetAnswer("../data/misha/1.jpg"));
    ASSERT_EQ(0, recognizer.GetAnswer("../data/misha/2.jpg"));
    ASSERT_EQ(0, recognizer.GetAnswer("../data/misha/7.jpg"));
    ASSERT_EQ(0, recognizer.GetAnswer("../data/misha/11.jpg"));
    ASSERT_EQ(-1, recognizer.GetAnswer("../data/meg1/2.jpg"));
}
