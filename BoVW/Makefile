CFLAGS = -std=c++11

#######################################
# Scheme Two:

LIBS =  -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videostab -lopencv_calib3d -lopencv_contrib
#INCPATH = -I/usr/local/Cellar/opencv/2.4.11_1/include
#LIBPATH = -L/usr/local/Cellar/opencv/2.4.11_1/lib

INCPATH = -I/usr/local/include -I/Users/willard/codes/cpp/opencv-computer-vision/cpp/BoVW/cppsugar
LIBPATH = -L/usr/local/lib

all:    index search
index:    index.cpp BoWBuilder.h
	g++ $(CFLAGS) $(INCPATH) $(LIBPATH) -o index index.cpp $(LIBS)
search:    search.cpp BoWBuilder.h
	g++ $(CFLAGS) $(INCPATH) $(LIBPATH) -o search search.cpp $(LIBS)

clean:
	rm -f index search
