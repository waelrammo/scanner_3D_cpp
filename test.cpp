#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;

int main( int argc, char** argv ) {

	char* imageName = argv[1];

	Mat image = Mat(3,3,CV_64F,double(0));
	std::cout << "Hello world from OpenCV" << std::endl;
	std::cout << image << std::endl;
	
	return 0;

}
