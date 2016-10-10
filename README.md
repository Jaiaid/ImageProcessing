This is some dirty implementation of things I have learned in Digital Image Processing course.

imageclass.h holds a Image class which has some attribute an method both virtual and implemented. This class is intended to be inherited by a class to handle and parse specific image format. Here I have made an derived class PPM to parse a .ppm file or save a .ppm from created pixel matrix.

imageprocessor.h holds DIP class which holds methods to process Image class(or class derived from it).

image_processing.cpp creates an interface to interact with a DIP class instantiated with a input file to manipulate it and creates new image.
