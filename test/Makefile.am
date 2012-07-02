bin_PROGRAMS = test

test_SOURCES = ../src/FaceDetector.cpp ../src/FaceDetector.h ../src/AnnTrain.h ../src/AnnTrain.cpp test.cpp

test_CXXFLAGS = @OPENCV_CFLAGS@ -Wall -pthread
test_LDADD = @OPENCV_LIBS@ -lm -lfann -lgtest_main -lgtest