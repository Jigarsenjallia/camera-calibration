cmake_minimum_required (VERSION 2.6)
PROJECT(calibration)
FIND_PACKAGE( OpenCV REQUIRED )
ADD_EXECUTABLE( calibration calibration.cpp )
TARGET_LINK_LIBRARIES( calibration ${OpenCV_LIBS} )






