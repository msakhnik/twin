bin_PROGRAMS = test

test_SOURCES = ../src/FaceDetector.cpp ../src/FaceDetector.h t_facedetector.cpp

test_CXXFLAGS = @OPENCV_CFLAGS@ -Wall -pthread
test_LDADD = @OPENCV_LIBS@ -lm -lgtest_main -lgtest
