#include <cv.h>
#include <highgui.h>
#include <iostream>

#include <frangi_filter.hpp>

using namespace cv;
string type2str(int type);

int main( int argc, char** argv ) {

	// filename of the video to process
	VideoCapture video_file("./../movies/IMG_0052.mov");

	Mat image;
	std::vector<Mat> input_images; // container for our images (not using 3D openCV arrays for now)
	int n_frames = 0;
	bool display_video = false; // option to play the video
	bool display_steps = false;
	// read in the red channel of the video
	while ( video_file.read(image) ) {
		Mat channels[3];
		split(image, channels); 	
		input_images.push_back(channels[0]); // this is the red channel
		std::ostringstream image_filename;  
		if (display_video){
			cv::imshow("window", image);

			char key = cvWaitKey(20);
        if (key == 27) // ESC
        	break; 		
	    }
	    n_frames++;
	}
	if (n_frames == 0) {
		std::cout << "Could not read in video file, exiting" << std::endl;
		return 1;
	}

	int n_rows = input_images[0].rows;
	int n_cols = input_images[0].cols;
	int image_type = input_images[0].type();
	std::cout << "Video dimensions = " << n_rows << "x" << n_cols << "x" << n_frames << std::endl;
	std::cout << "Video type = " << type2str(image_type) << std::endl;

	// calculate the median image
	Mat median_image = Mat::zeros(n_rows,n_cols,CV_8U);

	for (int i = 0; i < n_rows; ++i) {
		for (int j = 0; j < n_cols; ++j) {
			std::vector<uint8_t> v;
			for (int k = 0; k < n_frames; ++k) {
				v.push_back(input_images[k].at<uint8_t>(i,j));
			}
			size_t n = v.size() / 2;
			nth_element(v.begin(), v.begin()+n, v.end());
			median_image.at<uint8_t>(i,j) = v[n];
		}
	}
	// subtract the median image from the input images
	std::vector<Mat> output_images;
	for (int k = 0; k < n_frames; ++k) {
		Mat subtracted_image = Mat::zeros(n_rows,n_cols,CV_8S);
		for (int i = 0; i < n_rows; ++i) {
			for (int j = 0; j < n_cols; ++j) {
				subtracted_image.at<int8_t>(i,j) = int8_t(input_images[k].at<uint8_t>(i,j)) - int8_t(median_image.at<uint8_t>(i,j));
			}
		}
		output_images.push_back(subtracted_image);
		if (display_steps){
			cv::imshow("window", subtracted_image);

			char key = cvWaitKey(20);
	        if (key == 27) // ESC
	        	break; 	
	    }	
	}
	std::cout << "Writing out median image" << std::endl;
	imwrite("median.png",median_image);
	std::vector<float> sigmas;
	sigmas.push_back(1.0f);
	frangi_filter(output_images[0],sigmas);

	return 0;
	
}


// small helper function to get meaningful type names from OpenCV enums
string type2str(int type) {

	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch ( depth ) {
		case CV_8U:  r = "8U"; break;
		case CV_8S:  r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
		default:     r = "User"; break;
	}

	r += "C";
	r += (chans+'0');

	return r;

}

