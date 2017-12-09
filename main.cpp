#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

double xSolve(double a1, double b1, double c1, double a2, double b2, double c2);
double ySolve(double a1, double b1, double c1, double a2, double b2, double c2);

int main(int argc, char** argv) {
	Mat input;
	input = imread(argv[1], 1);
	if (!input.data) {
		return 1;
	}

	//----------Noise filter---------------
	Size size(3, 3);
	GaussianBlur(input, input, size, 0);

	//Convert to gray image
	Mat grayImg, grayFilterImg;
	cvtColor(input, grayImg, CV_BGR2GRAY);
	//bilateralFilter(grayImg, grayFilterImg, 10, 20, 5);

	//----------Edge dectect by Canny------
	int lowThreshold = 70;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	//imshow("Input", input);
	Mat edgeImg;
	Canny(grayImg, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);

	//----------Hough transform-------------
	vector<Vec4i> linesA, linesB;
	HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 60, 10);
	HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 60, 10);

	//----------Draw lines------------------
	Mat lineImg(input.size(), CV_8UC3, Scalar(0, 0, 0));
	Mat lineImgH(input.size(), CV_8UC3, Scalar(0, 0, 0));
	Mat lineImgV(input.size(), CV_8UC3, Scalar(0, 0, 0));
	vector<Vec2d> linesH, linesV;
	for (size_t i = 0; i < linesA.size(); i++) {
		Vec4i l = linesA[i];
		float w = abs(l[1] - l[3]);
		float h = abs(l[0] - l[2]);
		float tan = w / h;
		if (tan < 0.08) {
			line(lineImg, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), 5, CV_AA);
			line(lineImgH, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), 5, CV_AA);
			l[0] = l[0] - 50;
			l[2] = l[2] + 50;
			double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);
			linesH.push_back(Vec2d(a, b));
		}
		if (tan > 11) {
			line(lineImg, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			line(lineImgV, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			l[1] = l[1] + 50;
			l[3] = l[3] - 50;
			double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);
			linesV.push_back(Vec2d(a, b));
		}
	}
	for (size_t i = 0; i < linesB.size(); i++) {
		Vec4i l = linesB[i];
		float w = abs(l[1] - l[3]);
		float h = abs(l[0] - l[2]);
		float tan = w / h;
		if (tan < 0.08) {
			line(lineImg, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), 5, CV_AA);
			line(lineImgH, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), 5, CV_AA);
			l[0] = l[0] - 50;
			l[2] = l[2] + 50;
			double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);
			linesH.push_back(Vec2d(a, b));
		}
		if (tan > 11) {
			line(lineImg, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			line(lineImgV, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			l[1] = l[1] + 50;
			l[3] = l[3] - 50;
			double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);
			linesV.push_back(Vec2d(a, b));
		}
	}

	//Find intersections
	vector<Vec2i> iPoints;
	for (int i = 0; i < linesV.size(); i++) {
		for (int j = 0; j < linesH.size(); j++) {
			int x = (int)xSolve(linesV[i][0], -1, - linesV[i][1], linesH[j][0], -1, - linesH[j][1]);
			int y = (int)ySolve(linesV[i][0], -1, - linesV[i][1], linesH[j][0], -1, - linesH[j][1]);
			if (x < 10 || y < 10) continue;
			if (((int)lineImgH.at<cv::Vec3b>(y, x)[2] == 255) && ((int)lineImgV.at<cv::Vec3b>(y, x)[2] == 255)) {
				iPoints.push_back(Vec2i(x, y));
				line(lineImg, Point(x, y), Point(x, y), Scalar(0, 255, 0), 5, CV_AA);
			}
		}
	}
	
	//Crop the plate
	Mat plateImg;
	if (!iPoints.empty()) {
		for (int i = 0; i < iPoints.size() - 1; i++) {
			for (int j = 1; j < iPoints.size(); j++) {
				double w = abs(iPoints[i][0] - iPoints[j][0]);
				double h = abs(iPoints[i][1] - iPoints[j][1]);
				double l = w / h;
				//if (w < 200 || h < 200) continue;
				if (l > 1.3 && l < 1.4) {
					Mat plateTempImg, plateGrayImg, plateThresholdImg;
					int black = 0, white = 0;
					//Rect(Point2i(int(iPoints[i][0]), int(iPoints[i][1])), Point2i(int(iPoints[j][0]), int(iPoints[j][1])));
					//rectangle(lineImg, Rect(Point2i(int(iPoints[i][0]), int(iPoints[i][1])), Point2i(int(iPoints[j][0]), int(iPoints[j][1]))), Scalar(0, 255, 255), 3, 8, 0);
					plateTempImg = input(Rect(Point2i(int(iPoints[i][0]), int(iPoints[i][1])), Point2i(int(iPoints[j][0]), int(iPoints[j][1]))));
					cvtColor( plateTempImg, plateGrayImg, CV_BGR2GRAY );
					threshold( plateGrayImg, plateThresholdImg, 100, 255, 0 );
					for (int h = 0; h < plateThresholdImg.rows; h++) {
						for (int k = 0; k < plateThresholdImg.cols; k++){
							if (plateThresholdImg.at<int>(h, k) == 0) black++;
							else white++;
						}
					}
					if (white / black >= 2) plateImg = plateTempImg;
				}
			}
		}
	}

	imshow("Line", lineImg);
	//imshow("Edge", lineImg1);
	imshow("Plate", plateImg);

	waitKey(0);
	return 0;
}

double xSolve(double a1, double b1, double c1, double a2, double b2, double c2) {
	double d = a1*b2 - a2*b1;
	double dx = c1*b2 - c2*b1;
	double dy = c2*a1 - c1*a2;
	if (dx == 0 || dy == 0) return -1;
	return dx / d;
}

double ySolve(double a1, double b1, double c1, double a2, double b2, double c2) {
	double d = a1*b2 - a2*b1;
	double dx = c1*b2 - c2*b1;
	double dy = c2*a1 - c1*a2;
	if (dy == 0 || dy == 0) return -1;
	return dy / d;
}