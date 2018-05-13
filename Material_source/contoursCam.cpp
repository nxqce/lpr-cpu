#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

int main() {
	//string name;
	//cin >> name;
	//name = "../../bs/" + name + ".jpg";
	//Mat input = imread(name);
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

		Mat input;
		resize(frame, input, Size(640, 480));

		Mat grayImg;
		cvtColor(input, grayImg, CV_BGR2GRAY);

		//imshow("gray", grayImg);

		/// Apply Histogram Equalization
		equalizeHist(grayImg, grayImg);

		imshow("equalized", grayImg);

		int lowThreshold = 70;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;
		//imshow("Input", input);
		Mat edgeImg;
		Canny(grayImg, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);

		//Mat binImg;
		//threshold(grayImg, binImg, 200, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
		dilate(edgeImg, edgeImg, element);
		dilate(edgeImg, edgeImg, element);

		//Mat element = getStructuringElement(MORPH_RECT, Size(17, 3));
		//morphologyEx(edgeImg, edgeImg, CV_MOP_CLOSE, element);

		bitwise_not(edgeImg, edgeImg);

		imshow("bin", edgeImg);

		CvMemStorage *contoursMem = cvCreateMemStorage(0);
		CvSeq *contours = 0;
		IplImage plate = edgeImg;

		int n = cvFindContours(&plate, contoursMem, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

		//Mat charImgArray[9];
		//Mat * charImg = charImgArray;
		//Point2i topLeft[9];
		//int index = 0;

		/// Approximate contours to polygons and get bounding rects
		vector<vector<Point> > contours_poly(n);
		vector<CvRect> boundRect(n);

		CvMemStorage *polyMem = cvCreateMemStorage(0);
		CvSeq *poly;

		poly = cvApproxPoly(contours, sizeof(CvContour), polyMem, CV_POLY_APPROX_DP, 3, 1);
		int totalPoly = 0;
		for (CvSeq *i = poly; i != NULL; i = i->h_next) {
			boundRect[totalPoly] = cvBoundingRect((CvContour*)i, 1);
			totalPoly++;
		}

		/// Draw bonding rects
		// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
		// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

		for (int i = 0; i < totalPoly; i++) {
			Rect r = boundRect[i];
			double rate = r.width / (double)r.height;
			if ((rate > 1.0f) && r.height > 50) {
				rectangle(input, r, Scalar(0, 0, 255), 1, 8, 0);
			}
		}

		cvClearMemStorage(contoursMem);
		cvClearMemStorage(polyMem);

		cvReleaseMemStorage(&contoursMem);
		cvReleaseMemStorage(&polyMem);

		imshow("contours", input);

		if (waitKey(30) >= 0) break;
	}
	
	return 0;
}