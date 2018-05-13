#include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

class WatershedSegmenter {
private:
	cv::Mat markers;
public:
	void setMarkers(const cv::Mat& markerImage) {
		//Convert to image of ints
		markerImage.convertTo(markers, CV_32S);
	}
	cv::Mat process(const cv::Mat &image) {
		//Apply watersheds
		cv::watershed(image, markers);
		return markers;
	}
	void showMarkers() {
		cv::imshow("Watershed", markers);
	}
	// Return result in the form of an image
	cv::Mat getSegmentation() {

		cv::Mat tmp;
		// all segment with label higher than 255
		// will be assigned value 255
		markers.convertTo(tmp, CV_8U);

		return tmp;
	}

	// Return watershed in the form of an image
	cv::Mat getWatersheds() {

		cv::Mat tmp;
		markers.convertTo(tmp, CV_8U, 255, 255);

		return tmp;
	}

};

int main() {
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	//cap.set(CV_CAP_PROP_FPS, 15);

	//Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
	namedWindow("Direction Histogram", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		//cv::imshow("Image", image);

		Mat image;
		cv::resize(frame, image, cv::Size(640, 480));

		cv::Mat binary;
		////binary = cv::imread("binary.bmp");
		cv::cvtColor(image, binary, CV_BGR2GRAY);
		cv::threshold(binary, binary, 128, 255, CV_THRESH_BINARY);


		//cv::imshow("Binary", binary);

		//Eliminate noise and smaller object
		cv::Mat fg;
		cv::erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 6);

		//cv::imshow("Foreground", fg);

		// Identify image pixels without objects
		cv::Mat bg;
		cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 6);
		cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);

		//cv::imshow("Background", bg);

		// Create markers image
		cv::Mat markers(binary.size(), CV_8U, cv::Scalar(0));
		markers = fg + bg;

		cv::imshow("Combine", markers);

		// Create watershed segmentation object
		WatershedSegmenter segmenter;
		// Set markers and process
		segmenter.setMarkers(markers);
		segmenter.process(image);

		//cv::imshow("Watersheds", markers);
		//segmenter.showMarkers();

		cv::rectangle(image, cv::Point(5, 5), cv::Point(image.cols - 5, image.rows - 5), cv::Scalar(255, 255, 255), 3);
		cv::rectangle(image, cv::Point(image.cols / 2 - 10, image.rows / 2 - 10),
			cv::Point(image.cols / 2 + 10, image.rows / 2 + 10), cv::Scalar(1, 1, 1), 10);
		cv::namedWindow("Image with marker");
		cv::imshow("Image with marker", image);

		// Display watersheds
		cv::namedWindow("Watershed");
		cv::imshow("Watershed", segmenter.getWatersheds());

		if (cv::waitKey(30) >=0) break;
	}
	return 0;
}