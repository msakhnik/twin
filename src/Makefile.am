bin_PROGRAMS = twin

twin_SOURCES = AnnTrain.cpp AnnTrain.h Form.h Form.cpp main.cpp

twin_CXXFLAGS = @OPENCV_CFLAGS@ -Wall $(GTKMM_CFLAGS)
twin_LDADD = @OPENCV_LIBS@ -lm -lfann $(GTKMM_LIBS)
