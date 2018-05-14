#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

void findCharacter(Mat input, Mat *charImg, int &index, Point2i *topLeft) {
	//cout << "Debug: findCharacter()" << endl;
	//----------Extract characters------------
	Mat plateImg = input;
	//resize(plateImg, plateImg, Size(640, 480)); //270;200
	resize(plateImg, plateImg, Size(270, 200));
	//imshow("Plate", plateImg);

	Mat plateBlurImg, plateEdgeImg, plateBinImg, plateGrayImg, plateContourImg;

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));

	cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);
	threshold(plateGrayImg, plateBinImg, 80, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	//adaptiveThreshold(plateGrayImg, plateBinImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 1);

	//dilate(plateBinImg, plateBinImg, element, Point(-1,-1), 2);
	erode(plateBinImg, plateBinImg, element);

	plateContourImg = plateBinImg.clone();
	//imshow("Bin", plateBinImg);

	dilate(plateBinImg, plateBinImg, element, Point(-1, -1), 2);

	//vector < vector<Point> > contours;
	//findContours(plateEdgeImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	CvMemStorage *contoursMem = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	IplImage plate = plateContourImg;

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
		if (index > 8) break;
		Rect r = boundRect[i];
		double rate = r.width / (double)r.height;
		if ((rate > 0.2f && rate < 0.6f) && (r.height < 100 && r.height > 40)) {
			topLeft[index] = r.tl();
			rectangle(plateImg, r, Scalar(0, 0, 255), 1, 8, 0);
			charImg[index] = plateBinImg(r);
			index++;
		}
	}
	//imshow("plateBin", plateBinImg);
	if (index > 7 && index < 10) imshow("plate", plateImg);
	//waitKey(0);

	cvClearMemStorage(contoursMem);
	cvClearMemStorage(polyMem);

	cvReleaseMemStorage(&contoursMem);
	cvReleaseMemStorage(&polyMem);

	return;
}

int main() {
	string name;
	cin >> name;
	name = "../../bs/" + name + ".jpg";
	Mat input = imread(name);
	//VideoCapture cap(0); // open the default camera
	//if (!cap.isOpened())  // check if we succeeded
	//	return -1;
	//cap.set(CV_CAP_PROP_FPS, 15);

	//Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
	//for (;;)
	//{
		//Mat frame;
		//cap >> frame; // get a new frame from camera

		//Mat input;
		resize(input, input, Size(640, 480));

		Mat img = input.clone();

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

		Mat diEdgeImg;
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
		dilate(edgeImg, diEdgeImg, element);
		dilate(diEdgeImg, diEdgeImg, element);

		//Mat element = getStructuringElement(MORPH_RECT, Size(17, 3));
		//morphologyEx(edgeImg, edgeImg, CV_MOP_CLOSE, element);

		bitwise_not(diEdgeImg, diEdgeImg);

		imshow("bin", diEdgeImg);

		CvMemStorage *contoursMem = cvCreateMemStorage(0);
		CvSeq *contours = 0;
		IplImage plate = diEdgeImg;

		int n = cvFindContours(&plate, contoursMem, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

		/// Approximate contours to polygons and get bounding rects
		vector<vector<Point> > contours_poly(n);
		vector<CvRect> boundRect(n);
		vector<CvBox2D> boundBox(n);

		CvMemStorage *polyMem = cvCreateMemStorage(0);
		CvSeq *poly;

		poly = cvApproxPoly(contours, sizeof(CvContour), polyMem, CV_POLY_APPROX_DP, 3, 1);
		int totalPoly = 0;
		for (CvSeq *i = poly; i != NULL; i = i->h_next) {
			boundRect[totalPoly] = cvBoundingRect((CvContour*)i, 1);
			boundBox[totalPoly] = cvMinAreaRect2((CvContour*)i);
			totalPoly++;
		}

		/// Draw bonding rects
		// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
		// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

		vector<Mat> plates, edgePlates;
		int j = 0;
		for (int i = 0; i < totalPoly; i++) {
			Rect r = boundRect[i];
			CvBox2D b = boundBox[i];
			double rate = r.width / (double)r.height;
			if (rate > 0.5 && rate < 2 && r.height > 50) {
				rectangle(img, r, Scalar(0, 0, 255), 1, 8, 0);
				if (r.tl().x - 20 < 0 || r.tl().y - 20 < 0 || r.br().x + 20 > input.cols || r.br().y + 20 > input.rows) continue;
				Rect sr = Rect(Point(r.tl().x - 20, r.tl().y - 20), Point(r.br().x + 20, r.br().y + 20));
				plates.push_back(input(sr));
				edgePlates.push_back(edgeImg(sr));

				if (b.size.height < 100) continue;
				RotatedRect rRect = RotatedRect(b);
				Point2f vertices[4];
				rRect.points(vertices);
				for (int i = 0; i < 4; i++)
					line(img, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0),3);

				j++;
			}
		}

		int index = 0;
		Mat charImgArray[9];
		Point2i topLeftArray[9];
		for (int i = 0; i < edgePlates.size(); i++) {
			CvMemStorage* linesMem = cvCreateMemStorage(0);
			IplImage plate = edgePlates[i];
			CvSeq* linesA = cvHoughLines2(&plate, linesMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 70, 30, 50);

			if (linesA->total < 1) {
				cvClearMemStorage(linesMem);
				cvReleaseMemStorage(&linesMem);
				continue;
			}

			float avgA = 0;
			for (size_t j = 0; j < linesA->total; j++) {
				Point* lP = (Point*)cvGetSeqElem(linesA, j);
				float w = abs(lP[0].x - lP[1].x);
				float h = lP[0].y - lP[1].y;
				float tan = h / w;
				avgA += atan(tan);
				
				//line(plates[i], lP[0], lP[1], Scalar(255,0,0), 1, CV_AA);
				//line(plates[i], lP[0], lP[0], Scalar(255, 255, 0), 3, CV_AA);
				//line(plates[i], lP[1], lP[1], Scalar(0, 0, 255), 3, CV_AA);
			}
			avgA = avgA / linesA->total;

			cvClearMemStorage(linesMem);
			cvReleaseMemStorage(&linesMem);

			double angle = avgA * 180 / CV_PI;
			if (angle > 0) angle = 40 - angle;
			else angle = - angle - 50;

			Point2f center((plates[i].cols - 1) / 2.0, (plates[i].rows - 1) / 2.0);
			Mat rot = getRotationMatrix2D(center, angle, 1.0);
			// determine bounding rectangle, center not relevant
			Rect bbox = RotatedRect(cv::Point2f(), plates[i].size(), angle).boundingRect();
			// adjust transformation matrix
			rot.at<double>(0, 2) += bbox.width / 2.0 - plates[i].cols / 2.0;
			rot.at<double>(1, 2) += bbox.height / 2.0 - plates[i].rows / 2.0;
			warpAffine(plates[i], plates[i], rot, bbox.size());

			imshow(to_string(i), plates[i]);

			//Mat binPlate;
			//threshold(plates[i], binPlate, 100, 255, CV_THRESH_BINARY);

			//imshow("sdfks", binPlate);

			//CvMemStorage *contoursMem2 = cvCreateMemStorage(0);
			//CvSeq *contours2 = 0;
			//IplImage plate2 = binPlate;

			//int n = cvFindContours(&plate2, contoursMem2, &contours2, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
			//
			///// Approximate contours to polygons and get bounding rects
			//vector<vector<Point> > contours_poly2(n);
			//vector<CvRect> boundRect2(n);

			//CvMemStorage *polyMem2 = cvCreateMemStorage(0);
			//CvSeq *poly2;

			//poly2 = cvApproxPoly(contours2, sizeof(CvContour), polyMem2, CV_POLY_APPROX_DP, 3, 1);
			//int totalPoly2 = 0;
			//for (CvSeq *j = poly2; j != NULL; j = j->h_next) {
			//	boundRect2[totalPoly2] = cvBoundingRect((CvContour*)j, 1);
			//	totalPoly2++;
			//}
			//
			///// Draw bonding rects
			//// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
			//// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

			//Mat plates2 = plates[i].clone();
			//int j = 0;
			//for (int j = 0; j < totalPoly2; j++) {
			//	Rect r = boundRect2[j];
			//	double rate = r.width / (double)r.height;
			//	if (r.height > 50) {
			//		rectangle(plates[i], r, Scalar(0, 0, 255), 1, 8, 0);
			//		plates2 = plates[i](r);
			//	}
			//}

			//imshow(to_string(i), plates[i]);


			//int tempIndex = 0;
			//findCharacter(plates2, charImgArray, tempIndex, topLeftArray);
			//cout << "debug" << endl;
			//if (tempIndex > 7 && tempIndex < 10) {
			//	index = tempIndex;
			//	imshow("Plate", plates2);
			//	break;
			//}
		}

		cvClearMemStorage(contoursMem);
		cvClearMemStorage(polyMem);

		cvReleaseMemStorage(&contoursMem);
		cvReleaseMemStorage(&polyMem);

		imshow("contours", img);

		waitKey(0);

	//	if (waitKey(30) >= 0) break;
	//}
	
	return 0;
}