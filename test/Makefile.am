bin_PROGRAMS = test

test_SOURCES = ../src/FaceRecognizer.cpp ../src/FaceRecognizer.h t_recognizer.cpp

test_CXXFLAGS = @OPENCV_CFLAGS@ -Wall -pthread
test_LDADD = @OPENCV_LIBS@ -lm -lgtest_main -lgtest
