bin_PROGRAMS = twin

twin_SOURCES = FaceDetector.cpp FaceDetector.h FaceRecognizer.cpp FaceRecognizer.h Form.h Form.cpp main.cpp

twin_CXXFLAGS = @OPENCV_CFLAGS@ -Wall -g $(GTKMM_CFLAGS)
twin_LDADD = @OPENCV_LIBS@ -lm $(GTKMM_LIBS) -lboost_system -lboost_thread -lboost_filesystem
