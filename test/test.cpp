#include <gtest/gtest.h>
#include "../src/FaceDetector.h"
#include "../src/AnnTrain.h"
#include <iostream>
#include <numeric>

using namespace std;

cFaceDetector face;
unsigned int size = 150 * 150;
unsigned int output = 1;
cAnnTrain ann(output, size);

TEST(cFaceDetector, Initialize)
{
    ASSERT_TRUE(face.Init());
}
/*
TEST(cAnnTrain, Init)
{
    ann.TrainNeiro();
}
 */

TEST(cFaceDetector, FindFace)
{
    ASSERT_TRUE(face.FindFace("../data/test_img/2.jpg"));
}

TEST(cFaceDetector, InFaceArrayRange)
{
    ASSERT_TRUE(face.InFaceArrayRange());
}

vector<int> temp_vector;

TEST(cFaceDetector, GetFaces)
{
    while (face.InFaceArrayRange())
    {
        temp_vector = face.GetFaces();
    }
    cerr << temp_vector.size() << endl;
}

/*
TEST(cAnnTrain, SaveData)
{
    ann.SaveData(temp_vector, 1);
}
*/

//
//TEST (cAnnTrain, GetFloatAnswer)
//{
//    int sum =  accumulate(temp_vector.begin(), temp_vector.end(), 0);
//    printf("%f/n", ann.GetFloatAnswer(temp_vector));
//    cerr << endl << "sum is:" << sum << endl;
//}