//#include "stdafx.h"
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//
//using namespace std;
//using namespace cv;
//
//double xSolve(double a1, double b1, double c1, double a2, double b2, double c2);
//double ySolve(double a1, double b1, double c1, double a2, double b2, double c2);
//Vec3d linearEquation(double x1, double y1, double x2, double y2);
//Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2);
//int dec2bin(int num);
//char getChar(int code);
//String checkCorner(Mat input);
//String checkRightEdge(Mat input);
//String checkTopEdge(Mat input, String type);
//String checkMidEdgeRight(Mat input);
//String checkInner(Mat input);
//String checkMN(Mat input);
//String checkMidEdge(Mat input);
//String detectChar(Mat input);
//
//int main(int argc, char** argv) {
//	Mat input;
//	input = imread("../InputImage/input.jpg", 1);
//	if (!input.data) {
//		return 1;
//	}
//
//	//----------Noise filter---------------
//	Mat Gauss_Step;
//	Size size(7, 7);
//	GaussianBlur(input, Gauss_Step, size, 0);
//
//	//Convert to gray image
//	Mat grayImg, grayFilterImg, img;
//	cvtColor(Gauss_Step, grayImg, CV_BGR2GRAY);
//	//threshold(grayImg, img, 110, 255, CV_THRESH_BINARY);
//	//imshow("anh nhi phan", img);
//	//bilateralFilter(grayImg, grayFilterImg, 10, 20, 5);
//
//	//----------Edge dectect by Canny------
//	int lowThreshold = 70;
//	int const max_lowThreshold = 100;
//	int ratio = 3;
//	int kernel_size = 3;
//	//imshow("Input", input);
//	Mat edgeImg;
//	Canny(grayImg, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);
//	imshow("Canny", edgeImg);
//	//----------Hough transform-------------
//	vector<Vec4i> linesA, linesB;
//	HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 60, 10);
//	HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 30, 10);
//
//	Mat Lines_StepA, Lines_StepB;
//	cvtColor(edgeImg, Lines_StepA, COLOR_GRAY2BGR);
//	cvtColor(edgeImg, Lines_StepB, COLOR_GRAY2BGR);
//	cout << endl << " Endline " << linesA.size();
//	for (size_t i = 0; i < linesA.size(); i++)
//	{
//		Vec4i l = linesA[i];
//		line(Lines_StepA, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
//	}
//	for (size_t i = 0; i < linesB.size(); i++)
//	{
//		Vec4i l = linesB[i];
//		line(Lines_StepB, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
//	}
//	imshow("Line_StepA", Lines_StepA);
//	imshow("Line_StepB", Lines_StepB);
//
//	//----------Draw lines------------------
//	int intensity = 3;
//	Mat lineImg(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	Mat lineImgH(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	Mat lineImgV(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	vector<Vec3d> linesH, linesV;
//	for (size_t i = 0; i < linesA.size(); i++) {
//		Vec4i l = linesA[i];
//		float w = abs(l[1] - l[3]);
//		float h = abs(l[0] - l[2]);
//		float tan = w / h;
//		// tan 7 = 0.12
//		if (tan < 0.12) {
//			line(lineImg, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), intensity, CV_AA);
//			line(lineImgH, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), intensity, CV_AA);
//			l[0] = l[0] - 50;
//			l[2] = l[2] + 50;
//			//double a = xSolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			//double b = ySolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			//if (a == 0 || b == 0 || a < 0 || b < 0) cout << endl << "Duong song song truc a : " << a << " b: " << b << endl;
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			linesH.push_back(constABC);
//		}
//		// tan 83 = 8.14
//		if (tan > 8.14) {
//			line(lineImg, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), intensity, CV_AA);
//			line(lineImgV, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), intensity, CV_AA);
//			l[1] = l[1] + 50;
//			l[3] = l[3] - 50;
//			//double a = xSolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			//double b = ySolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			//if (a == 0 || b == 0 || a < 0 || b < 0) cout << endl << "Duong song song truc a : " << a << " b: " << b << endl;
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			linesV.push_back(constABC);
//		}
//	}
//	for (size_t i = 0; i < linesB.size(); i++) {
//		Vec4i l = linesB[i];
//		float w = abs(l[1] - l[3]);
//		float h = abs(l[0] - l[2]);
//		float tan = w / h;
//		if (tan < 0.08) {
//			line(lineImg, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), intensity, CV_AA);
//			line(lineImgH, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), intensity, CV_AA);
//			l[0] = l[0] - 50;
//			l[2] = l[2] + 50;
//			/*double a = xSolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			double b = ySolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			if (a == 0 || b == 0 || a < 0 || b < 0) cout << endl << "Duong song song truc a : " << a << " b: " << b << endl;*/
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			linesH.push_back(constABC);
//		}
//		if (tan > 11) {
//			line(lineImg, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), intensity, CV_AA);
//			line(lineImgV, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), intensity, CV_AA);
//			l[1] = l[1] + 50;
//			l[3] = l[3] - 50;
//			/*double a = xSolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			double b = ySolve(l[0], 1.0, l[1], l[2], 1.0, l[3]);
//			if (a == 0 || b == 0 || a < 0 || b < 0) cout << "Duong song song truc a : " << a << " b: " << b << endl;*/
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			linesV.push_back(constABC);
//		}
//	}
//	imshow("Vertical ", lineImgV);
//	imshow("Horizontal ", lineImgH);
//	imshow("V + H", lineImg);
//
//	//----------Find intersections---------------
//	cout << "\n Tong so duong ngang: " << linesH.size() << " va doc: " << linesV.size() << endl;
//	cout << "\n => So giao diem ki vong: " << linesH.size() * linesV.size() << endl;
//	int trackInter = 0;
//	vector<Vec2i> iPoints;
//	for (int i = 0; i < linesV.size(); i++) {
//		for (int j = 0; j < linesH.size(); j++) {
//			//double x = (double)xSolve(linesV[i][0], -1, -linesV[i][1], linesH[j][0], -1, -linesH[j][1]);
//			//double y = (double)ySolve(linesV[i][0], -1, -linesV[i][1], linesH[j][0], -1, -linesH[j][1]);
//			Vec2d interXY;
//			interXY = interPoint(linesV[i][0], linesV[i][1], linesV[i][2], linesH[j][0], linesH[j][1], linesH[j][2]);
//			int x = (int)interXY[0];
//			int y = (int)interXY[1];
//			if (x < 0 || y < 0) continue;
//			if (x > input.cols || y > input.rows) continue;
//			if (((int)lineImgH.at<cv::Vec3b>(y, x)[2] == 255) && ((int)lineImgV.at<cv::Vec3b>(y, x)[2] == 255)) {
//				iPoints.push_back(Vec2i(x, y));
//				line(lineImg, Point(x, y), Point(x, y), Scalar(0, 255, 0), 5, CV_AA);
//				cout << "\nHeocon xinhxan" << endl;
//			}
//			//if (x == 0 || y == 0 || x < 0 || y < 0) cout << endl << "Giao diem nam tren truc x: " << x << " y: " << y << endl;
//			//else line(lineImg, Point(x, y), Point(x, y), Scalar(255, 0, 0), 5, CV_AA);
//			//iPoints.push_back(XY);
//		}
//	}
//	imshow("Intersections", lineImg);
//
//	//----------Crop the plate-------------------
//	Mat plateImg;
//	int run = 0;
//	cout << "\n Tong so giao diem cua hinh la: " << iPoints.size() << " va size cua Vec2i " << sizeof(Vec2i) << endl;
//	if (!iPoints.empty()) {
//		for (int i = 0; i < iPoints.size() - 1; i++) {
//			for (int j = 1; j < iPoints.size(); j++) {
//				double w = abs(iPoints[i][0] - iPoints[j][0]);
//				double h = abs(iPoints[i][1] - iPoints[j][1]);
//				double l = w / h;
//				//if (w < 200 || h < 200) continue;
//				if (l > 1.3 && l < 1.4) {
//					Mat plateTempImg, plateGrayImg, plateThresholdImg;
//					int black = 0, white = 0;
//
//					plateTempImg = input(Rect(Point2i(int(iPoints[i][0]), int(iPoints[i][1])), Point2i(int(iPoints[j][0]), int(iPoints[j][1]))));
//					cvtColor(plateTempImg, plateGrayImg, CV_BGR2GRAY);
//					threshold(plateGrayImg, plateThresholdImg, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//					for (int m = 0; m < plateThresholdImg.cols; m++) {
//						for (int n = 0; n < plateThresholdImg.rows; n++) {
//							if ((int)plateThresholdImg.at<uchar>(Point(m, n)) == 0) black++;
//							else white++;
//						}
//					}
//
//					if (black > 0 && white / black >= 2 && plateTempImg.rows > 100) {
//						plateImg = plateTempImg;
//						run++;
//					}	
//				}
//			}
//		}
//	}
//
//	//for (int i = 0; i < run; i++) {
//		try {
//			cout << "\n So hinh thuc te crop duoc: " << run << endl;
//			imshow("Plate: ", plateImg);
//		}
//		catch (Exception e) {
//			cout << "Cannot find the plate" << endl;
//		}
//	//}
//	
//	//----------Extract characters------------
//	//Mat plateGrayImg, plateBinImg;
//	//cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);
//	//threshold(plateGrayImg, plateBinImg, 120, 255, CV_THRESH_BINARY);
//	resize(plateImg, plateImg, Size(270, 200));
//	Mat plateBlurImg, plateEdgeImg, plateBinImg, plateGrayImg, plateContourImg;
//
//	//GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
//	//Canny(plateImg, plateEdgeImg, 50, 300, 3) ;
//	//Mat morpho;
//	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
//	//erode(plateImg, morpho, element, Point(-1, -1), 3);
//	//imshow("Erode", morpho);
//	//cvtColor(morpho, morpho, CV_BGR2GRAY);
//	//threshold(morpho, morpho, 110, 255, CV_THRESH_BINARY);
//
//	cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);
//	threshold(plateGrayImg, plateBinImg, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//	//adaptiveThreshold(plateGrayImg, plateBinImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 1);
//
//	//dilate(plateBinImg, plateBinImg, element, Point(-1,-1), 2);
//	erode(plateBinImg, plateBinImg, element);
//
//	plateContourImg = plateBinImg.clone();
//	imshow("Bin", plateBinImg);
//
//	dilate(plateBinImg, plateBinImg, element, Point(-1, -1), 2);
//
//	vector < vector<Point> > contours;
//	findContours(plateContourImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
//	imshow("contours", plateContourImg);
//
//	Mat charImg[9];
//	Point2i topLeft[9];
//	int index = 0;
//
//	/// Approximate contours to polygons and get bounding rects
//	vector<vector<Point> > contours_poly(contours.size());
//	vector<Rect> boundRect(contours.size());
//
//	for (int i = 0; i < contours.size(); i++) {
//		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
//		boundRect[i] = boundingRect(Mat(contours_poly[i]));
//	}
//
//	///// Draw bonding rects
//	//// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
//	//// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);
//
//	for (int i = 0; i< contours.size(); i++) {
//		Rect r = boundRect[i];
//		if ((r.width / (double)r.height > 0.2f && r.width / (double)r.height < 0.6f)
//			&& (r.height < 100 && r.height > 70)) {
//			topLeft[index] = r.tl();
//			rectangle(plateImg, r, Scalar(0, 0, 255), 1, 8, 0);
//			charImg[index] = plateBinImg(r);
//			index++;
//		}
//	}
//
//	////Sort the charImg array
//	//int top = 0;
//	//for (int i = 0; i < index; i++) {
//	//	if (topLeft[i].y < 50) {
//	//		top = i;
//	//		break;
//	//	}
//	//}
//
//	//for (int i = 0; i < top - 1; i++) {
//	//	for (int j = i; j < top; j++) {
//	//		if (topLeft[i].x < topLeft[j].x) {
//	//			Point2i tempPoint = topLeft[i];
//	//			topLeft[i] = topLeft[j];
//	//			topLeft[j] = tempPoint;
//
//	//			Mat tempImg = charImg[i];
//	//			charImg[i] = charImg[j];
//	//			charImg[j] = tempImg;
//	//		}
//	//	}
//	//}
//
//	//for (int i = top; i < index - 1; i++) {
//	//	for (int j = i; j < index; j++) {
//	//		if (topLeft[i].x < topLeft[j].x) {
//	//			Point2i tempPoint = topLeft[i];
//	//			topLeft[i] = topLeft[j];
//	//			topLeft[j] = tempPoint;
//
//	//			Mat tempImg = charImg[i];
//	//			charImg[i] = charImg[j];
//	//			charImg[j] = tempImg;
//	//		}
//	//	}
//	//}
//
//	//cout << "So ky tu: " << index << endl;
//
//	//int im = index - 1;
//	//while (im >= 0) {
//	//	// int im = 8;
//	//	//cout << charImg[im].cols << " : " << charImg[im].rows << endl;
//	//	// Scalar color = Scalar(255,255,255);
//	//	// line(charImg[im], Point(charImg[im].cols/3, 0), Point(charImg[im].cols/3, charImg[im].rows), color, 1, CV_AA);
//	//	// line(charImg[im], Point(charImg[im].cols/3*2, 0), Point(charImg[im].cols/3*2, charImg[im].rows), color, 1, CV_AA);
//	//	// line(charImg[im], Point(0, charImg[im].rows/6), Point(charImg[im].cols, charImg[im].rows/6), color, 1, CV_AA);
//	//	// line(charImg[im], Point(0, charImg[im].rows/6*2), Point(charImg[im].cols, charImg[im].rows/6*2), color, 1, CV_AA);
//	//	// line(charImg[im], Point(0, charImg[im].rows/6*3), Point(charImg[im].cols, charImg[im].rows/6*3), color, 1, CV_AA);
//	//	// line(charImg[im], Point(0, charImg[im].rows/6*4), Point(charImg[im].cols, charImg[im].rows/6*4), color, 1, CV_AA);
//	//	// line(charImg[im], Point(0, charImg[im].rows/6*5), Point(charImg[im].cols, charImg[im].rows/6*5), color, 1, CV_AA);
//	//	imshow("num " + char(im), charImg[im]);
//
//	//	int charSquare[3][6] = { { 0 } };
//	//	int charBit = 0;
//	//	char regChar = '-';
//
//	//	if (im == 6) {
//	//		cout << detectChar(charImg[im]);
//	//	}
//	//	else {
//	//		int black = 0, white = 0;
//	//		for (int l = 0; l < 6; l++) {
//	//			for (int c = 0; c < 3; c++) {
//	//				white = 0;
//	//				black = 0;
//	//				for (int i = 0 + c*charImg[im].cols / 3; i < (c + 1)*charImg[im].cols / 3; i++) {
//	//					for (int j = 0 + l*charImg[im].rows / 6; j < (l + 1)*charImg[im].rows / 6; j++) {
//	//						//cout << (int)charImg[0].at<uchar>(Point(i, j)) << " ";
//	//						if ((int)charImg[im].at<uchar>(Point(i, j)) == 0) black++;
//	//						else white++;
//	//					}
//	//					//cout << endl;
//	//				}
//	//				// cout << "[" << l << "," << c << "]" << endl;
//	//				// cout << "black: " << black << " - white: " << white << endl;
//	//				// cout << "---------------------------------" << endl;
//	//				if (black > white / 4) {
//	//					//if (white > 0){
//	//					charSquare[c][l] = 1;
//	//					charBit += pow(2, l * 3 + c);
//	//				}
//	//			}
//	//		}
//	//	}
//	//	
//
//	//	//dec2bin(charBit);
//
//	//	if (charImg[im].cols < 30) {
//	//		regChar = '1';
//	//	}
//	//	else {
//	//		regChar = getChar(charBit);
//	//	}
//
//	//	//cout << charBit << endl;
//	//	cout << regChar;
//
//	//	im--;
//	//}
//	//cout << endl;
//
//
//	////imshow("Line", lineImg);
//	////imshow("Edge", plateEdgeImg);
//	////imshow("Morpho", morpho);
//	imshow("Plate with contour", plateImg);
//
//	waitKey(0);
//	return 0;
//}
//
//int dec2bin(int num) {
//	int total = 0;
//	int count = 0;
//	cout << endl;
//	while (num > 0)
//	{
//		total = num % 2;
//		num /= 2;
//		if (total == 1)
//			cout << total << " ";
//		else
//			cout << "  ";
//		if (count % 3 == 2)
//			cout << endl;
//		count++;
//	}
//	cout << endl;
//	return 0;
//}
//
//char getChar(int code) {
//	//define minimun code of number
//	int zero = 88938,
//		two = 234786,
//		three = 84386, //84258,
//		four = 160338,
//		five = 84175, //116943,
//		six = 88778,
//		seven = 38023,
//		eight = 88746, //86186, //43050,
//		nine = 85354;
//
//	//compare the code with the minimun code
//	//8 9 3 0 are close to each other
//	if ((int)(code & eight) == eight) return '8';
//	else if ((int)(code & three) == three) return '3';
//
//	else if ((int)(code & nine) == nine) return '9';
//	else if ((int)(code & zero) == zero) return '0';
//
//	//3 is close to 2
//	else if ((int)(code & two) == two) return '2';
//
//	else if ((int)(code & four) == four) return '4';
//
//	//6 is close to 5
//	else if ((int)(code & six) == six) return '6';
//	else if ((int)(code & five) == five) return '5';
//
//	else if ((int)(code & seven) == seven) return '7';
//
//	else return ' ';
//}
//
//double xSolve(double a1, double b1, double c1, double a2, double b2, double c2) {
//	double d = a1*b2 - a2*b1;
//	double dx = c1*b2 - c2*b1;
//	double dy = c2*a1 - c1*a2;
//	if (d == 0) return -1;
//	return dx / d;
//}
//
//double ySolve(double a1, double b1, double c1, double a2, double b2, double c2) {
//	double d = a1*b2 - a2*b1;
//	double dx = c1*b2 - c2*b1;
//	double dy = c2*a1 - c1*a2;
//	if (d == 0) return -1;
//	return dy / d;
//}
//
//Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2) {
//	double x, y;
//
//	//(a2 - a1)x + (b2 - b1)y = c1 - c2
//	// TH1:
//	if ((a2 == a1) && (b2 != b1)) {
//		y = (c1 - c2) / (b2 - b1);
//		x = (-c1 - b1*y) / a1;
//	}
//	// TH2:
//	else if ((a2 != a1) && (b2 == b1)) {
//		x = (c1 - c2) / (a2 - a1);
//		y = (-c1 - a1*x) / b1;
//	}
//	// TH3:
//	else if ((a2 != a1) && (b2 != b1)) {
//		y = (a1*c2 - a2*c1) / (a2*b1 - a1*b2);
//		x = (-c1 - b1*y) / a1;
//	}
//
//	Vec2d result = { x, y };
//	return result;
//}
//
//Vec3d linearEquation(double x1, double y1, double x2, double y2) {
//	double a, b, c;
//
//	if ((x1 == x2) && (y1 != y2)) {
//		a = 1;
//		b = 0;
//		c = -x1;
//	}
//	else if ((x1 != x2) && (y1 == y2)) {
//		a = 0;
//		b = 1;
//		c = -y1;
//	}
//	else if ((x1 != x2) && (y1 != y2)) {
//		double T = (y1 - y2) / (x1 - x2);
//		double H = y1 - T*x1;
//		a = (-1)*T;
//		b = 1;
//		c = -(1)*(H);
//	}
//
//	Vec3d result = { a, b, c };
//	return result;
//}
//
//String checkCorner(Mat input) {
//	/*Scalar color = Scalar(255, 255, 255);
//	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
//	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
//
//	//imshow("Plate", input);
//	Mat tl = input(Rect(0, 0, input.cols / 3.0, input.rows / 6.0));
//	Mat tr = input(Rect(input.cols / 3.0 * 2, 0, input.cols / 3.0, input.rows / 6.0));
//	Mat bl = input(Rect(0, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
//	Mat br = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
//	/*cvtColor(tl, tl, CV_BGR2GRAY);
//	threshold(tl, tl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//	cvtColor(tr, tr, CV_BGR2GRAY);
//	threshold(tr, tr, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//	cvtColor(bl, bl, CV_BGR2GRAY);
//	threshold(bl, bl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//	cvtColor(br, br, CV_BGR2GRAY);
//	threshold(br, br, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/
//
//	int white = 0;
//	int black = 0;
//	String outputCor = "";
//
//	for (int cor = 0; cor < 4; cor++) {
//		white = 0; black = 0;
//		Mat corDetect;
//		switch (cor) {
//		case 0:
//			corDetect = tl.clone();
//			break;
//		case 1:
//			corDetect = tr.clone();
//			break;
//		case 2:
//			corDetect = br.clone();
//			break;
//		case 3:
//			corDetect = bl.clone();
//			break;
//		}
//		for (int i = 0; i < corDetect.cols; i++) {
//			for (int j = 0; j < corDetect.rows; j++) {
//				if ((int)corDetect.at<uchar>(Point(i, j)) == 0) {
//					black++;
//				}
//				else white++;
//			}
//		}
//		if (black > white / 3) {
//			outputCor += "1";
//		}
//
//		else outputCor += "0";
//	}
//	//cout << endl << outputCor << endl;
//	return outputCor;
//}
//
//String checkRightEdge(Mat input) {
//	String result;
//	/*Scalar color = Scalar(255, 255, 255);
//	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
//	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
//
//	Mat r2 = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
//	Mat r3 = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0 * 2, input.cols / 3.0, input.rows / 6.0));
//	/*cvtColor(r2, r2, CV_BGR2GRAY);
//	threshold(r2, r2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//	cvtColor(r3, r3, CV_BGR2GRAY);
//	threshold(r3, r3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/
//
//	int white = 0;
//	int black = 0;
//	String outputCor = "";
//	for (int r = 0; r < 2; r++) {
//		Mat rDetect;
//		black = 0; white = 0;
//		rDetect = (r == 0) ? r2 : r3;
//		for (int i = 0; i < rDetect.cols; i++) {
//			for (int j = 0; j < rDetect.rows; j++) {
//				if ((int)rDetect.at<uchar>(Point(i, j)) == 0) {
//					black++;
//				}
//				else white++;
//			}
//		}
//		if (black < white / 4) {
//			outputCor += "0";
//		}
//		else outputCor += "1";
//	}
//
//	if (outputCor == "11" || outputCor == "10") {
//		result = 'P';
//	}
//	else if (outputCor == "00" || outputCor == "01") {
//		result = 'F';
//	}
//
//	return result;
//}
//
//String checkTopEdge(Mat input, String type) {
//	String result;
//	/*Scalar color = Scalar(255, 255, 255);
//	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
//	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
//
//	Mat t = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
//	/*cvtColor(t, t, CV_BGR2GRAY);
//	threshold(t, t, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/
//
//	int white = 0;
//	int black = 0;
//	char outputCor;
//	for (int i = 0; i < t.cols; i++) {
//		for (int j = 0; j < t.rows; j++) {
//			if ((int)t.at<uchar>(Point(i, j)) == 0) {
//				black++;
//			}
//			else white++;
//		}
//	}
//	if (black > white / 1.2) {
//		outputCor = '1';
//	}
//	else outputCor = '0';
//
//	if (outputCor == '1') {
//		result = (type == "1011") ? "Q" : "T";
//	}
//	else if (outputCor = '0') {
//		result = (type == "1011") ? "L" : "none";
//	}
//
//	return result;
//}
//
//String checkMidEdgeRight(Mat input) {
//	String result;
//	/*Scalar color = Scalar(255, 255, 255);
//	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
//	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
//
//	Mat m2 = input(Rect(input.cols / 3.0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
//	Mat m3 = input(Rect(input.cols / 3.0, input.rows / 6.0 * 2, input.cols / 3.0, input.rows / 6.0));
//	/*cvtColor(m2, m2, CV_BGR2GRAY);
//	threshold(m2, m2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//	cvtColor(m3, m3, CV_BGR2GRAY);
//	threshold(m3, m3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/
//
//	int white = 0;
//	int black = 0;
//	String outputCor = "";
//	for (int m = 0; m < 2; m++) {
//		Mat mDetect;
//		black = 0; white = 0;
//		mDetect = (m == 0) ? m2 : m3;
//		for (int i = 0; i < mDetect.cols; i++) {
//			for (int j = 0; j < mDetect.rows; j++) {
//				if ((int)mDetect.at<uchar>(Point(i, j)) == 0) {
//					black++;
//				}
//				else white++;
//			}
//		}
//		if (black > white) {
//			outputCor += "1";
//		}
//		else outputCor += "0";
//	}
//
//	if (outputCor == "11" /*|| outputCor == "01"*/) {
//		result = 'Y';
//	}
//	else {
//		result = 'V';
//	}
//
//	return result;
//}
//
//String checkInner(Mat input) {
//	String result;
//	/*Scalar color = Scalar(255, 255, 255);
//	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
//	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
//	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
//
//	int black = 0, white = 0, damm = 0;
//	for (int l = 0; l < 6; l++) {
//		for (int c = 0; c < 3; c++) {
//			white = 0;
//			black = 0;
//			if (c != 0 && c != 2 && l != 0 && l != 5) {
//				for (int i = 0 + c*input.cols / 3.0; i < (c + 1)*input.cols / 3.0; i++) {
//					for (int j = 0 + l*input.rows / 6.0; j < (l + 1)*input.rows / 6.0; j++) {
//						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
//						else white++;
//					}
//				}
//				damm++;
//				if (black > white / 3) {
//					result += "1";
//				}
//				else {
//					result += "0";
//				}
//			}
//		}
//	}
//	//cout << result;
//	return result;
//}
//
//String checkMN(Mat input) {
//	String result;
//
//	Mat m1 = input(Rect(input.cols / 5.0 * 2, input.rows / 10.0 * 2, input.cols / 5.0, input.rows / 10.0 * 2));
//	//cvtColor(m1, m1, CV_BGR2GRAY);
//	//threshold(m1, m1, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//
//	int black = 0, white = 0, damm = 0;
//	for (int i = 0; i < m1.cols; i++) {
//		for (int j = 0; j < m1.rows; j++) {
//			if ((int)m1.at<uchar>(Point(i, j)) == 0) black++;
//			else white++;
//		}
//	}
//
//	if ((black / white) > 2) {
//		result = "M";
//	}
//	else result = "N";
//	//cout << result;
//	//imshow("Plate", m1);
//	return result;
//}
//
//String checkMidEdge(Mat input) {
//	String result;
//	//Scalar color = Scalar(255, 255, 255);
//	///*line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
//	//line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
//	//line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
//	//line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
//	//line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
//	//line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
//	//line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
//
//	Mat t = input(Rect(0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
//	Mat m = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
//	//imshow("M", m);
//
//	/*cvtColor(input, input, CV_BGR2GRAY);
//	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/
//
//	int black = 0, white = 0, damm = 0;
//	for (int l = 0; l < 6; l++) {
//		for (int c = 0; c < 3; c++) {
//			white = 0;
//			black = 0;
//			if ((c == 0 && l == 0) || (c == 2 && l == 0) || (c == 0 && l == 5) || (c == 2 && l == 5));
//			else if (c == 0 || c == 2 || l == 0 || l == 5) {
//				for (int i = 0 + c*input.cols / 3; i < (c + 1)*input.cols / 3; i++) {
//					for (int j = 0 + l*input.rows / 6; j < (l + 1)*input.rows / 6; j++) {
//						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
//						else white++;
//					}
//				}
//				damm++;
//				if (black > white / 3) {
//					//cout << "Mieng den " << l << " " << c << endl;
//					result += "1";
//				}
//				else {
//					result += "0";
//					//cout << "Mieng trang " << l << " " << c << endl;
//				}
//
//				/*if (c == 1 && l == 0) {
//				cout << "Black " << black << endl;
//				cout << "White " << white << endl;
//				}*/
//			}
//		}
//	}
//
//	long int toNumber = stoi(result);
//	//cout << toNumber;
//	int check = 0;
//	switch (toNumber)
//	{
//	case 1111010111:
//		result = "C";
//		break;
//	case 1101010101:
//		result = "E";
//		break;
//	case 1111011111:
//		result = "G";
//		break;
//	case 1101010111:
//		result = "K";
//		break;
//	case 1111001111:
//		result = "S";
//		break;
//	case 111111111:
//		result = "U";
//		break;
//	case 1010000101:
//		result = "Z";
//		break;
//	case 1110000111:
//		result = "X";
//		break;
//	case 1111111111:
//		check = 1;
//		break;
//	case 111111110:
//		check = 1;
//		break;
//	default:
//		break;
//	}
//	//cout << toNumber;
//
//	if (check == 1) {
//		String temp = checkInner(input);
//		if (toNumber == 1111111111) {
//			if (temp == "0110" || temp == "0100" || temp == "0010") result = "B";
//			else result = "D";
//		}
//		else if (toNumber == 111111110) {
//			if (temp == "0110" || temp == "0100" || temp == "0010") result = "H";
//			else result = checkMN(input);
//		}
//	}
//
//	return result;
//}
//
//String detectChar(Mat input) {
//	String detectedChar, step1_Char;
//	step1_Char = checkCorner(input);
//
//	if (step1_Char == "0011") {
//		detectedChar = "A";
//	}
//	else if (step1_Char == "0111") {
//		detectedChar = "J";
//	}
//	else if (step1_Char == "1101") {
//		detectedChar = checkRightEdge(input);
//	}
//	else if (step1_Char == "1011" || step1_Char == "1100") {
//		//call step2_1011-1100
//		//cout << "Vao day coi " << step1_Char;
//		String temp = checkTopEdge(input, step1_Char);
//		detectedChar = (temp == "none") ? checkMidEdgeRight(input) : temp;
//	}
//	else if (step1_Char == "1111") {
//		//call step2_1111
//		//cout << "No vao day";
//		detectedChar = checkMidEdge(input);
//	}
//
//	//cout << endl << "This is character " << detectedChar << endl;
//	return detectedChar;
//}
#include "stdafx.h"
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
Vec3d linearEquation(double x1, double y1, double x2, double y2);
Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2);
int dec2bin(int num);
char getChar(int code);
String checkCorner(Mat input);
String checkRightEdge(Mat input);
String checkTopEdge(Mat input, String type);
String checkMidEdgeRight(Mat input);
String checkInner(Mat input);
String checkMN(Mat input);
String checkMidEdge(Mat input);
String detectChar(Mat input);

int main(int argc, char** argv) {
	Mat input;
	input = imread("../InputImage/input10.jpg", 1);
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
	imshow("Canny", edgeImg);

	//----------Hough transform-------------
	vector<Vec4i> linesA, linesB;
	HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 60, 10);
	HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 60, 10);

	//----------Draw lines------------------
	Mat lineImg(input.size(), CV_8UC3, Scalar(0, 0, 0));
	Mat lineImgH(input.size(), CV_8UC3, Scalar(0, 0, 0));
	Mat lineImgV(input.size(), CV_8UC3, Scalar(0, 0, 0));
	vector<Vec3d> linesH, linesV;
	int delta = 90;
	for (size_t i = 0; i < linesA.size(); i++) {
		Vec4i l = linesA[i];
		float w = abs(l[1] - l[3]);
		float h = abs(l[0] - l[2]);
		float tan = w / h;
		if (tan < 0.08) {
			line(lineImg, Point(l[0] - delta, l[1]), Point(l[2] + delta, l[3]), Scalar(0, 0, 255), 5, CV_AA);
			line(lineImgH, Point(l[0] - delta, l[1]), Point(l[2] + delta, l[3]), Scalar(0, 0, 255), 5, CV_AA);
			l[0] = l[0] - 50;
			l[2] = l[2] + 50;
			/*double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);*/
			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
			linesH.push_back(constABC);
		}
		if (tan > 11) {
			line(lineImg, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			line(lineImgV, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			l[1] = l[1] + 50;
			l[3] = l[3] - 50;/*
			double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);
			linesV.push_back(Vec2d(a, b))*/;
			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
			linesV.push_back(constABC);
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
			/*double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);
			linesH.push_back(Vec2d(a, b));*/
			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
			linesH.push_back(constABC);
		}
		if (tan > 11) {
			line(lineImg, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			line(lineImgV, Point(l[0], l[1] + 50), Point(l[2], l[3] - 50), Scalar(0, 0, 255), 5, CV_AA);
			l[1] = l[1] + 50;
			l[3] = l[3] - 50;
			/*double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);
			linesV.push_back(Vec2d(a, b));*/
			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
			linesV.push_back(constABC);
		}
	}

	//----------Find intersections---------------
	vector<Vec2i> iPoints;
	for (int i = 0; i < linesV.size(); i++) {
		for (int j = 0; j < linesH.size(); j++) {
			/*int x = (int)xSolve(linesV[i][0], -1, -linesV[i][1], linesH[j][0], -1, -linesH[j][1]);
			int y = (int)ySolve(linesV[i][0], -1, -linesV[i][1], linesH[j][0], -1, -linesH[j][1]);*/
			Vec2d interXY;
			interXY = interPoint(linesV[i][0], linesV[i][1], linesV[i][2], linesH[j][0], linesH[j][1], linesH[j][2]);
			int x = (int)interXY[0];
			int y = (int)interXY[1];
			if (x < 10 || y < 10) continue;
			if (x > input.cols || y > input.rows) continue;
			if (((int)lineImgH.at<cv::Vec3b>(y, x)[2] == 255) && ((int)lineImgV.at<cv::Vec3b>(y, x)[2] == 255)) {
				iPoints.push_back(Vec2i(x, y));
				line(lineImg, Point(x, y), Point(x, y), Scalar(0, 255, 0), 5, CV_AA);
			}
		}
	}

	imshow("Vertical ", lineImgV);
	imshow("Horizontal ", lineImgH);
	imshow("V + H", lineImg);

	//----------Crop the plate-------------------
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

					plateTempImg = input(Rect(Point2i(int(iPoints[i][0]), int(iPoints[i][1])), Point2i(int(iPoints[j][0]), int(iPoints[j][1]))));
					cvtColor(plateTempImg, plateGrayImg, CV_BGR2GRAY);
					threshold(plateGrayImg, plateThresholdImg, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
					for (int m = 0; m < plateThresholdImg.cols; m++) {
						for (int n = 0; n < plateThresholdImg.rows; n++) {
							if ((int)plateThresholdImg.at<uchar>(Point(m, n)) == 0) black++;
							else white++;
						}
					}

					if (black > 0 && white / black >= 2 && plateTempImg.rows > 20)
						plateImg = plateTempImg;
				}
			}
		}
	}

	try{
		imshow("Plate before", plateImg);
	}
	catch (Exception e) {
		cout << "\nCannot find the plate " << endl;

		waitKey(0);
		return 0;
	}
	//----------Extract characters------------
	//Mat plateGrayImg, plateBinImg;
	//cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);
	//threshold(plateGrayImg, plateBinImg, 120, 255, CV_THRESH_BINARY);
	resize(plateImg, plateImg, Size(270, 200));
	Mat plateBlurImg, plateEdgeImg, plateBinImg, plateGrayImg, plateContourImg;

	//GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
	//Canny(plateImg, plateEdgeImg, 50, 300, 3) ;
	//Mat morpho;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	//erode(plateImg, morpho, element, Point(-1, -1), 3);
	//imshow("Erode", morpho);
	//cvtColor(morpho, morpho, CV_BGR2GRAY);
	//threshold(morpho, morpho, 110, 255, CV_THRESH_BINARY);

	cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);
	threshold(plateGrayImg, plateBinImg, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//adaptiveThreshold(plateGrayImg, plateBinImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 1);

	//dilate(plateBinImg, plateBinImg, element, Point(-1,-1), 2);
	erode(plateBinImg, plateBinImg, element);

	plateContourImg = plateBinImg.clone();
	imshow("Bin", plateBinImg);

	dilate(plateBinImg, plateBinImg, element, Point(-1, -1), 2);

	vector < vector<Point> > contours;
	findContours(plateContourImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	Mat charImg[9];
	Point2i topLeft[9];
	int index = 0;

	/// Approximate contours to polygons and get bounding rects
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	/// Draw bonding rects
	// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
	// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

	for (int i = 0; i< contours.size(); i++) {
		Rect r = boundRect[i];
		if ((r.width / (double)r.height > 0.2f && r.width / (double)r.height < 0.6f)
			&& (r.height < 100 && r.height > 70)) {
			topLeft[index] = r.tl();
			rectangle(plateImg, r, Scalar(0, 0, 255), 1, 8, 0);
			charImg[index] = plateBinImg(r);
			index++;
		}
	}

	//Sort the charImg array
	int top = 0;
	for (int i = 0; i < index; i++) {
		if (topLeft[i].y < 50) {
			top = i;
			break;
		}
	}

	for (int i = 0; i < top - 1; i++) {
		for (int j = i; j < top; j++) {
			if (topLeft[i].x < topLeft[j].x) {
				Point2i tempPoint = topLeft[i];
				topLeft[i] = topLeft[j];
				topLeft[j] = tempPoint;

				Mat tempImg = charImg[i];
				charImg[i] = charImg[j];
				charImg[j] = tempImg;
			}
		}
	}

	for (int i = top; i < index - 1; i++) {
		for (int j = i; j < index; j++) {
			if (topLeft[i].x < topLeft[j].x) {
				Point2i tempPoint = topLeft[i];
				topLeft[i] = topLeft[j];
				topLeft[j] = tempPoint;

				Mat tempImg = charImg[i];
				charImg[i] = charImg[j];
				charImg[j] = tempImg;
			}
		}
	}

	//cout << "So ky tu: " << index << endl;

	//int im = index - 1;
	//while (im >= 0) {
	//	// int im = 8;
	//	//cout << charImg[im].cols << " : " << charImg[im].rows << endl;
	//	// Scalar color = Scalar(255,255,255);
	//	// line(charImg[im], Point(charImg[im].cols/3, 0), Point(charImg[im].cols/3, charImg[im].rows), color, 1, CV_AA);
	//	// line(charImg[im], Point(charImg[im].cols/3*2, 0), Point(charImg[im].cols/3*2, charImg[im].rows), color, 1, CV_AA);
	//	// line(charImg[im], Point(0, charImg[im].rows/6), Point(charImg[im].cols, charImg[im].rows/6), color, 1, CV_AA);
	//	// line(charImg[im], Point(0, charImg[im].rows/6*2), Point(charImg[im].cols, charImg[im].rows/6*2), color, 1, CV_AA);
	//	// line(charImg[im], Point(0, charImg[im].rows/6*3), Point(charImg[im].cols, charImg[im].rows/6*3), color, 1, CV_AA);
	//	// line(charImg[im], Point(0, charImg[im].rows/6*4), Point(charImg[im].cols, charImg[im].rows/6*4), color, 1, CV_AA);
	//	// line(charImg[im], Point(0, charImg[im].rows/6*5), Point(charImg[im].cols, charImg[im].rows/6*5), color, 1, CV_AA);
	//	imshow("num " + char(im), charImg[im]);

	//	int charSquare[3][6] = { { 0 } };
	//	int charBit = 0;
	//	char regChar = '-';

	//	if (im == 6) {
	//		cout << detectChar(charImg[im]);
	//	}
	//	else {
	//		int black = 0, white = 0;
	//		for (int l = 0; l < 6; l++) {
	//			for (int c = 0; c < 3; c++) {
	//				white = 0;
	//				black = 0;
	//				for (int i = 0 + c*charImg[im].cols / 3; i < (c + 1)*charImg[im].cols / 3; i++) {
	//					for (int j = 0 + l*charImg[im].rows / 6; j < (l + 1)*charImg[im].rows / 6; j++) {
	//						//cout << (int)charImg[0].at<uchar>(Point(i, j)) << " ";
	//						if ((int)charImg[im].at<uchar>(Point(i, j)) == 0) black++;
	//						else white++;
	//					}
	//					//cout << endl;
	//				}
	//				// cout << "[" << l << "," << c << "]" << endl;
	//				// cout << "black: " << black << " - white: " << white << endl;
	//				// cout << "---------------------------------" << endl;
	//				if (black > white / 4) {
	//					//if (white > 0){
	//					charSquare[c][l] = 1;
	//					charBit += pow(2, l * 3 + c);
	//				}
	//			}
	//		}
	//	}

	//	//dec2bin(charBit);

	//	if (charImg[im].cols < 30) {
	//		regChar = '1';
	//	}
	//	else {
	//		regChar = getChar(charBit);
	//	}

	//	//cout << charBit << endl;
	//	cout << regChar;

	//	im--;
	//}
	//cout << endl;


	////imshow("Line", lineImg);
	////imshow("Edge", plateEdgeImg);
	////imshow("Morpho", morpho);
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

int dec2bin(int num) {
	int total = 0;
	int count = 0;
	cout << endl;
	while (num > 0)
	{
		total = num % 2;
		num /= 2;
		if (total == 1)
			cout << total << " ";
		else
			cout << "  ";
		if (count % 3 == 2)
			cout << endl;
		count++;
	}
	cout << endl;
	return 0;
}

char getChar(int code) {
	//define minimun code of number
	int zero = 88938,
		two = 234786,
		three = 84386, //84258,
		four = 160338,
		five = 84175, //116943,
		six = 88778,
		seven = 38023,
		eight = 88746, //86186, //43050,
		nine = 85354;

	//compare the code with the minimun code
	//8 9 3 0 are close to each other
	if ((int)(code & eight) == eight) return '8';
	else if ((int)(code & three) == three) return '3';

	else if ((int)(code & nine) == nine) return '9';
	else if ((int)(code & zero) == zero) return '0';

	//3 is close to 2
	else if ((int)(code & two) == two) return '2';

	else if ((int)(code & four) == four) return '4';

	//6 is close to 5
	else if ((int)(code & six) == six) return '6';
	else if ((int)(code & five) == five) return '5';

	else if ((int)(code & seven) == seven) return '7';

	else return ' ';
}

Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2) {
	double x, y;

	//(a2 - a1)x + (b2 - b1)y = c1 - c2
	// TH1:
	if ((a2 == a1) && (b2 != b1)) {
		y = (c1 - c2) / (b2 - b1);
		x = (-c1 - b1*y) / a1;
	}
	// TH2:
	else if ((a2 != a1) && (b2 == b1)) {
		x = (c1 - c2) / (a2 - a1);
		y = (-c1 - a1*x) / b1;
	}
	// TH3:
	else if ((a2 != a1) && (b2 != b1)) {
		y = (a1*c2 - a2*c1) / (a2*b1 - a1*b2);
		x = (-c1 - b1*y) / a1;
	}

	Vec2d result = { x, y };
	return result;
}

Vec3d linearEquation(double x1, double y1, double x2, double y2) {
	double a, b, c;

	if ((x1 == x2) && (y1 != y2)) {
		a = 1;
		b = 0;
		c = -x1;
	}
	else if ((x1 != x2) && (y1 == y2)) {
		a = 0;
		b = 1;
		c = -y1;
	}
	else if ((x1 != x2) && (y1 != y2)) {
		double T = (y1 - y2) / (x1 - x2);
		double H = y1 - T*x1;
		a = (-1)*T;
		b = 1;
		c = -(1)*(H);
	}

	Vec3d result = { a, b, c };
	return result;
}

String checkCorner(Mat input) {
	/*Scalar color = Scalar(255, 255, 255);
	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/

	//imshow("Plate", input);
	Mat tl = input(Rect(0, 0, input.cols / 3.0, input.rows / 6.0));
	Mat tr = input(Rect(input.cols / 3.0 * 2, 0, input.cols / 3.0, input.rows / 6.0));
	Mat bl = input(Rect(0, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
	Mat br = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
	/*cvtColor(tl, tl, CV_BGR2GRAY);
	threshold(tl, tl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(tr, tr, CV_BGR2GRAY);
	threshold(tr, tr, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(bl, bl, CV_BGR2GRAY);
	threshold(bl, bl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(br, br, CV_BGR2GRAY);
	threshold(br, br, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/

	int white = 0;
	int black = 0;
	String outputCor = "";

	for (int cor = 0; cor < 4; cor++) {
		white = 0; black = 0;
		Mat corDetect;
		switch (cor) {
		case 0:
			corDetect = tl.clone();
			break;
		case 1:
			corDetect = tr.clone();
			break;
		case 2:
			corDetect = br.clone();
			break;
		case 3:
			corDetect = bl.clone();
			break;
		}
		for (int i = 0; i < corDetect.cols; i++) {
			for (int j = 0; j < corDetect.rows; j++) {
				if ((int)corDetect.at<uchar>(Point(i, j)) == 0) {
					black++;
				}
				else white++;
			}
		}
		if (black > white / 3) {
			outputCor += "1";
		}

		else outputCor += "0";
	}
	//cout << endl << outputCor << endl;
	return outputCor;
}

String checkRightEdge(Mat input) {
	String result;
	/*Scalar color = Scalar(255, 255, 255);
	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/

	Mat r2 = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
	Mat r3 = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0 * 2, input.cols / 3.0, input.rows / 6.0));
	/*cvtColor(r2, r2, CV_BGR2GRAY);
	threshold(r2, r2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(r3, r3, CV_BGR2GRAY);
	threshold(r3, r3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/

	int white = 0;
	int black = 0;
	String outputCor = "";
	for (int r = 0; r < 2; r++) {
		Mat rDetect;
		black = 0; white = 0;
		rDetect = (r == 0) ? r2 : r3;
		for (int i = 0; i < rDetect.cols; i++) {
			for (int j = 0; j < rDetect.rows; j++) {
				if ((int)rDetect.at<uchar>(Point(i, j)) == 0) {
					black++;
				}
				else white++;
			}
		}
		if (black < white / 4) {
			outputCor += "0";
		}
		else outputCor += "1";
	}

	if (outputCor == "11" || outputCor == "10") {
		result = 'P';
	}
	else if (outputCor == "00" || outputCor == "01") {
		result = 'F';
	}

	return result;
}

String checkTopEdge(Mat input, String type) {
	String result;
	/*Scalar color = Scalar(255, 255, 255);
	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/

	Mat t = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
	/*cvtColor(t, t, CV_BGR2GRAY);
	threshold(t, t, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/

	int white = 0;
	int black = 0;
	char outputCor;
	for (int i = 0; i < t.cols; i++) {
		for (int j = 0; j < t.rows; j++) {
			if ((int)t.at<uchar>(Point(i, j)) == 0) {
				black++;
			}
			else white++;
		}
	}
	if (black > white / 1.2) {
		outputCor = '1';
	}
	else outputCor = '0';

	if (outputCor == '1') {
		result = (type == "1011") ? "Q" : "T";
	}
	else if (outputCor = '0') {
		result = (type == "1011") ? "L" : "none";
	}

	return result;
}

String checkMidEdgeRight(Mat input) {
	String result;
	/*Scalar color = Scalar(255, 255, 255);
	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/

	Mat m2 = input(Rect(input.cols / 3.0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
	Mat m3 = input(Rect(input.cols / 3.0, input.rows / 6.0 * 2, input.cols / 3.0, input.rows / 6.0));
	/*cvtColor(m2, m2, CV_BGR2GRAY);
	threshold(m2, m2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(m3, m3, CV_BGR2GRAY);
	threshold(m3, m3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/

	int white = 0;
	int black = 0;
	String outputCor = "";
	for (int m = 0; m < 2; m++) {
		Mat mDetect;
		black = 0; white = 0;
		mDetect = (m == 0) ? m2 : m3;
		for (int i = 0; i < mDetect.cols; i++) {
			for (int j = 0; j < mDetect.rows; j++) {
				if ((int)mDetect.at<uchar>(Point(i, j)) == 0) {
					black++;
				}
				else white++;
			}
		}
		if (black > white) {
			outputCor += "1";
		}
		else outputCor += "0";
	}

	if (outputCor == "11" /*|| outputCor == "01"*/) {
		result = 'Y';
	}
	else {
		result = 'V';
	}

	return result;
}

String checkInner(Mat input) {
	String result;
	/*Scalar color = Scalar(255, 255, 255);
	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/

	int black = 0, white = 0, damm = 0;
	for (int l = 0; l < 6; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			if (c != 0 && c != 2 && l != 0 && l != 5) {
				for (int i = 0 + c*input.cols / 3.0; i < (c + 1)*input.cols / 3.0; i++) {
					for (int j = 0 + l*input.rows / 6.0; j < (l + 1)*input.rows / 6.0; j++) {
						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
						else white++;
					}
				}
				damm++;
				if (black > white / 3) {
					result += "1";
				}
				else {
					result += "0";
				}
			}
		}
	}
	//cout << result;
	return result;
}

String checkMN(Mat input) {
	String result;

	Mat m1 = input(Rect(input.cols / 5.0 * 2, input.rows / 10.0 * 2, input.cols / 5.0, input.rows / 10.0 * 2));
	//cvtColor(m1, m1, CV_BGR2GRAY);
	//threshold(m1, m1, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	int black = 0, white = 0, damm = 0;
	for (int i = 0; i < m1.cols; i++) {
		for (int j = 0; j < m1.rows; j++) {
			if ((int)m1.at<uchar>(Point(i, j)) == 0) black++;
			else white++;
		}
	}

	if ((black / white) > 2) {
		result = "M";
	}
	else result = "N";
	//cout << result;
	//imshow("Plate", m1);
	return result;
}

String checkMidEdge(Mat input) {
	String result;
	//Scalar color = Scalar(255, 255, 255);
	///*line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
	//line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
	//line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
	//line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
	//line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
	//line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
	//line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/

	Mat t = input(Rect(0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
	Mat m = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
	//imshow("M", m);

	/*cvtColor(input, input, CV_BGR2GRAY);
	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/

	int black = 0, white = 0, damm = 0;
	for (int l = 0; l < 6; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			if ((c == 0 && l == 0) || (c == 2 && l == 0) || (c == 0 && l == 5) || (c == 2 && l == 5));
			else if (c == 0 || c == 2 || l == 0 || l == 5) {
				for (int i = 0 + c*input.cols / 3; i < (c + 1)*input.cols / 3; i++) {
					for (int j = 0 + l*input.rows / 6; j < (l + 1)*input.rows / 6; j++) {
						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
						else white++;
					}
				}
				damm++;
				if (black > white / 3) {
					//cout << "Mieng den " << l << " " << c << endl;
					result += "1";
				}
				else {
					result += "0";
					//cout << "Mieng trang " << l << " " << c << endl;
				}

				/*if (c == 1 && l == 0) {
				cout << "Black " << black << endl;
				cout << "White " << white << endl;
				}*/
			}
		}
	}

	long int toNumber = stoi(result);
	//cout << toNumber;
	int check = 0;
	switch (toNumber)
	{
	case 1111010111:
		result = "C";
		break;
	case 1101010101:
		result = "E";
		break;
	case 1111011111:
		result = "G";
		break;
	case 1101010111:
		result = "K";
		break;
	case 1111001111:
		result = "S";
		break;
	case 111111111:
		result = "U";
		break;
	case 1010000101:
		result = "Z";
		break;
	case 1110000111:
		result = "X";
		break;
	case 1111111111:
		check = 1;
		break;
	case 111111110:
		check = 1;
		break;
	default:
		break;
	}
	//cout << toNumber;

	if (check == 1) {
		String temp = checkInner(input);
		if (toNumber == 1111111111) {
			if (temp == "0110" || temp == "0100" || temp == "0010") result = "B";
			else result = "D";
		}
		else if (toNumber == 111111110) {
			if (temp == "0110" || temp == "0100" || temp == "0010") result = "H";
			else result = checkMN(input);
		}
	}

	return result;
}

String detectChar(Mat input) {
	String detectedChar, step1_Char;
	step1_Char = checkCorner(input);

	if (step1_Char == "0011") {
		detectedChar = "A";
	}
	else if (step1_Char == "0111") {
		detectedChar = "J";
	}
	else if (step1_Char == "1101") {
		detectedChar = checkRightEdge(input);
	}
	else if (step1_Char == "1011" || step1_Char == "1100") {
		//call step2_1011-1100
		//cout << "Vao day coi " << step1_Char;
		String temp = checkTopEdge(input, step1_Char);
		detectedChar = (temp == "none") ? checkMidEdgeRight(input) : temp;
	}
	else if (step1_Char == "1111") {
		//call step2_1111
		//cout << "No vao day";
		detectedChar = checkMidEdge(input);
	}

	//cout << endl << "This is character " << detectedChar << endl;
	return detectedChar;
}
