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
int dec2bin(int num);
char getChar(int code);
Vec3d linearEquation(double x1, double y1, double x2, double y2);
Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2);

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
	if (dx == 0 || dy == 0) return -1;
	return dy / d;
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
	else {
		cout << a1 << " " << b1 << " " << c1 << endl;
		cout << a2 << " " << b2 << " " << c2 << endl;
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
	//cout << "Debug: getChar()" << endl;
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

	//cout << (int)(code & three) << " == 84386" << endl;

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

	else return '*';
}

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
		if (( rate > 0.2f && rate < 0.6f) && (r.height < 100 && r.height > 40)) {
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

void sortPlate(Mat *charImg, Point2i *topLeft, int index){
	//cout << "Debug: sortPlate" << endl;
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

	return;
}

char detectNumber(Mat * charImg, int im) {
	//cout << "Debug: detectNumber" << endl;
	int charSquare[3][6] = { { 0 } };
	int charBit = 0;
	char regChar = '-';

	int black = 0, white = 0;
	for (int l = 0; l < 6; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			for (int i = 0 + c*charImg[im].cols / 3; i < (c + 1)*charImg[im].cols / 3; i++) {
				for (int j = 0 + l*charImg[im].rows / 6; j < (l + 1)*charImg[im].rows / 6; j++) {
					//cout << (int)charImg[0].at<uchar>(Point(i, j)) << " ";
					if ((int)charImg[im].at<uchar>(Point(i, j)) == 0) black++;
					else white++;
				}
				//cout << endl;
			}
			// cout << "[" << l << "," << c << "]" << endl;
			// cout << "black: " << black << " - white: " << white << endl;
			// cout << "---------------------------------" << endl;
			if (white > black / 4) {
				//if (white > 0){
				charSquare[c][l] = 1;
				charBit += pow(2, l * 3 + c);
			}
		}
	}

	//cout << charImg[im].cols << endl;

	if (charImg[im].cols < 20) {
		regChar = '1';
	}
	else {
		//cout << charBit << endl;
		regChar = getChar(charBit);
	}

	return regChar;
}

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
		
		Mat input;
		resize(frame, input, Size(640, 480));

		//----------Noise filter---------------
		Size size(5, 5);
		GaussianBlur(input, input, size, 0);

		//Convert to gray image
		Mat grayImg, grayFilterImg;
		cvtColor(input, grayImg, CV_BGR2GRAY);
		
		//----------Edge dectect by Canny------
		int lowThreshold = 50;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;
		//imshow("Input", input);
		Mat edgeImg;
		Canny(grayImg, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);
		//imshow("Edge", edgeImg);
		

		////FPGA
		//imshow("Input", input);
		//Mat edgeImg = input;
		//threshold(edgeImg, edgeImg, 150, 255, CV_THRESH_BINARY);



		//----------Hough transform-------------
		//vector<Vec4i> linesA, linesB;

		CvMemStorage* linesAMem = cvCreateMemStorage(0);
		CvMemStorage* linesBMem = cvCreateMemStorage(0);

		IplImage edge = edgeImg;

		//HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 30, 50);
		CvSeq* linesA = cvHoughLines2(&edge, linesAMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 70, 30, 50);
		//HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 50, 50);
		CvSeq* linesB = cvHoughLines2(&edge, linesBMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 30, 50, 50);

		//----------Draw lines------------------
		Mat lineImg(input.size(), CV_8UC1, Scalar(0));
		int stretch = 0; //Number of pixel to stretch lines
		double tanV = 11;
		double tanH = 0.08;
		for (size_t i = 0; i < linesA->total; i++) {
			Point* l = (Point*)cvGetSeqElem(linesA, i);
			float w = abs(l[0].x - l[1].x);
			float h = abs(l[0].y - l[1].y);
			float tan = h / w;
			if (tan < tanH) {
				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
			}
			if (tan > tanV) {
				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
			}
		}
		//for (size_t i = 0; i < linesB->total; i++) {
		//	Point* l = (Point*)cvGetSeqElem(linesB, i);
		//	float w = abs(l[0].x - l[1].x);
		//	float h = abs(l[0].y - l[1].y);
		//	float tan = h / w;
		//	if (tan < tanH) {
		//		line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
		//	}
		//	if (tan > tanV) {
		//		line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
		//	}
		//}

		//imshow("Line", lineImg);

		int vHis[640] = { 0 };
		int hHis[480] = { 0 };

		//Tim histogram theo phuong doc
		for (int i = 0; i < edgeImg.cols; i++) {
			for (int j = 0; j < edgeImg.rows; j++) {
				if (lineImg.at<uchar>(j, i) != 0)
					vHis[i]++;
			}
		}

		//Tim histogram theo phuong ngang
		for (int i = 0; i < edgeImg.rows; i++) {
			for (int j = 0; j < edgeImg.cols; j++) {
				if (lineImg.at<uchar>(i, j) != 0)
					hHis[i]++;
			}
		}

		//Ve histogram len anh bien
		Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
		cvtColor(edgeImg, vhHisImg, CV_GRAY2BGR);

		//Tim nguong trung binh phuong doc
		int vMin = 480;
		int vMax = 0;
		int vSum = 0;
		for (int i = 0; i < 640; i++) {
			if (vHis[i] < vMin) vMin = vHis[i];
			if (vHis[i] > vMax) vMax = vHis[i];
			vSum += vHis[i];
		}
		int vAvgMM = (vMax + vMin) / 2;
		int vAvgS = vSum / 640;

		//Tim nguong trung binh phuong ngang
		int hMin = 640;
		int hMax = 0;
		int hSum = 0;
		for (int i = 0; i < 480; i++) {
			if (hHis[i] < hMin) hMin = hHis[i];
			if (hHis[i] > hMax) hMax = hHis[i];
			hSum += hHis[i];
		}

		int hAvgMM = (hMax + hMin) / 2;
		int hAvgS = hSum / 480;

		int vAvg = (vAvgMM + vAvgS) / 2;
		int hAvg = (hAvgMM + hAvgS) / 2;

		//ve duong thang
		Vector<int> hLines, vLines;
		int count = 0, start = 0;
		int countMax = 0;
		for (int i = 0; i < 640; i++) {
			if (vHis[i] >= vAvg) {
				//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
				if (count == 0) {
					//line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
					vLines.push_back(i);
				}
				start = 1;
			}
			if (start == 1) {
				count++;
				if (count > countMax) start = 0;
			}
			else {
				count = 0;
			}
		}
		count = 0;
		start = 0;
		for (int i = 639; i >= 0; i--) {
			if (vHis[i] >= vAvg) {
				//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
				if (count == 0) {
					//line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
					vLines.push_back(i);
				}
				start = 1;
			}
			if (start == 1) {
				count++;
				if (count > countMax) start = 0;
			}
			else {
				count = 0;
			}
		}

		count = 0;
		start = 0;
		for (int i = 0; i < 480; i++) {
			if (hHis[i] >= hAvg) {
				//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
				if (count == 0) {
					//line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
					hLines.push_back(i);
				}
				start = 1;
			}
			if (start == 1) {
				count++;
				if (count > countMax) start = 0;
			}
			else {
				count = 0;
			}
		}
		count = 0;
		start = 0;
		for (int i = 479; i >= 0; i--) {
			if (hHis[i] >= hAvg) {
				//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
				if (count == 0) {
					//line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
					hLines.push_back(i);
				}
				start = 1;
			}
			if (start == 1) {
				count++;
				if (count > countMax) start = 0;
			}
			else {
				count = 0;
			}
		}


		//ve histogram
		for (int i = 0; i < 640; i++) {
			line(vhHisImg, Point(i, 480), Point(i, 480 - vHis[i]), Scalar(255, 255, 0), 1);
		}

		for (int i = 0; i < 480; i++) {
			line(vhHisImg, Point(0, i), Point(hHis[i], i), Scalar(255, 255, 0), 1);
		}

		//duong trung binh
		line(vhHisImg, Point(0, 480 - vAvgMM), Point(639, 480 - vAvgMM), Scalar(0, 255, 0), 1);
		line(vhHisImg, Point(0, 480 - vAvgS), Point(639, 480 - vAvgS), Scalar(0, 0, 255), 1);

		line(vhHisImg, Point(hAvgMM, 0), Point(hAvgMM, 480), Scalar(0, 255, 0), 1);
		line(vhHisImg, Point(hAvgS, 0), Point(hAvgS, 480), Scalar(0, 0, 255), 1);

		//Tim tat ca cac giao diem
		Vector<Vec2i> iPoints;
		for (int i = 0; i < vLines.size(); i++) {
			for (int j = 0; j < hLines.size(); j++) {
				iPoints.push_back(Vec2i(vLines[i], hLines[j]));
				line(vhHisImg, Point(vLines[i], hLines[j]), Point(vLines[i], hLines[j]), Scalar(0, 0, 255), 3);
			}
		}


		double w, h, l;
		double iMax = 0, iHMax = 0, iVMax = 0;
		Point selectedP1 = Point(0, 0), selectedP2 = Point(0, 0);
		Point vP1 = Point(0, 0), vP2 = Point(0, 0), hP1 = Point(0, 0), hP2 = Point(0, 0);
		int found = 0;

		int index = 0;
		Mat charImgArray[9];
		//Mat * charImg = charImgArray;
		Point2i topLeftArray[9];
		//Point2i * topLeft = topLeftArray;

		if (!iPoints.empty()) {
			for (int i = 0; i < iPoints.size() - 1; i++) {
				for (int j = i + 1; j < iPoints.size(); j++) {
					w = abs(iPoints[i][0] - iPoints[j][0]);
					h = abs(iPoints[i][1] - iPoints[j][1]);
					l = w / h;
					if (w < 120.0 || h < 90.0) continue;
					if (l > 1.3 && l < 1.4) {
						//int xTl, xBr, yTl, yBr;
						//xTl = find(vLines.begin(), vLines.end(), iPoints[i][0]) - vLines.begin();
						//xBr = find(vLines.begin(), vLines.end(), iPoints[j][0]) - vLines.begin();
						//yTl = find(hLines.begin(), hLines.end(), iPoints[i][1]) - hLines.begin();
						//yBr = find(hLines.begin(), hLines.end(), iPoints[j][1]) - hLines.begin();
						//int iSum = abs(xBr - xTl) + abs(yBr - yTl);

						//int iVSum = 0;

						//int xStart, xEnd;
						//xStart = (iPoints[i][0] > iPoints[j][0]) ? iPoints[j][0] : iPoints[i][0];
						//xEnd = (iPoints[i][0] > iPoints[j][0]) ? iPoints[i][0] : iPoints[j][0];
						//for (int i = xStart; i <= xEnd; i++) iVSum += vHis[i];

						//int iHSum = 0;
						//int yStart, yEnd;
						//yStart = (iPoints[i][1] > iPoints[j][1]) ? iPoints[j][1] : iPoints[i][1];
						//yEnd = (iPoints[i][1] > iPoints[j][1]) ? iPoints[i][1] : iPoints[j][1];
						//for (int i = yStart; i <= yEnd; i++) iHSum += hHis[i];

						//double iAvg = (double)((iVSum / (double)(xEnd - xStart)) + (iHSum / (double)(yEnd - yStart)));


						//if (iAvg > iMax) {
						//	selectedP1 = Point(iPoints[i]);
						//	selectedP2 = Point(iPoints[j]);
						//	iMax = iAvg;
						//}
						//if (iVSum / (double)(xEnd - xStart) >= iVMax) {
						//	vP1 = Point(iPoints[i]);
						//	vP2 = Point(iPoints[j]);
						//	iVMax = iVSum / (double)(xEnd - xStart);
						//}
						//if (iHSum / (double)(yEnd - yStart) > iHMax) {
						//	hP1 = Point(iPoints[i]);
						//	hP2 = Point(iPoints[j]);
						//	iHMax = iHSum / (double)(yEnd - yStart);
						//}

						Point tempP1 = Point(iPoints[i]);
						Point tempP2 = Point(iPoints[j]);
						Mat tempPlateImg = input(Rect(tempP1, tempP2));
						
						int tempIndex = 0;
						Mat tempCharImgArray[9];
						//Mat * tempCharImg = new Mat[9];
						Point2i tempTopLeftArray[9];
						//Point2i * tempTopLeft = new Point2i[9];

						findCharacter(tempPlateImg, charImgArray, tempIndex, topLeftArray);

						if (tempIndex > 7 && tempIndex < 10) {
							selectedP1 = tempP1;
							selectedP2 = tempP2;

							index = tempIndex;
							//for(int i = 0; i < index; i++) charImgArray[i] = tempCharImgArray[i].clone();
							//for (int i = 0; i < index; i++) topLeftArray[i] = tempTopLeftArray[i];

							//charImg = tempCharImg;
							//topLeft = tempTopLeft;

							found = 1;
							break;
						}

					}
				}
				if (found) break;
			}
		}



		//line(vhHisImg, Point(vP1.x, 0), Point(vP1.x, 479), Scalar(255, 0, 255), 2);
		//line(vhHisImg, Point(vP2.x, 0), Point(vP2.x, 479), Scalar(255, 0, 255), 2);
		//line(vhHisImg, Point(0, hP1.y), Point(639, hP1.y), Scalar(255, 0, 255), 2);
		//line(vhHisImg, Point(0, hP2.y), Point(639, hP2.y), Scalar(255, 0, 255), 2);
		rectangle(vhHisImg, Rect(selectedP1, selectedP2), Scalar(0, 255, 0), 3);


		//imshow("Origin", input);
		imshow("Direction Histogram", vhHisImg);

		//if (selectedP1 == Point(0, 0) && selectedP2 == Point(0, 0)) {
		//	if (waitKey(30) >= 0) break;
		//	continue;
		//}

		//Mat plateImg = input(Rect(selectedP1, selectedP2));

		//cout << "Debug: cropping the plate completed" << endl;

		//int index = 0;
		//Mat charImgArray[9];
		//Mat * charImg = charImgArray;
		//Point2i topLeftArray[9];
		//Point2i * topLeft = topLeftArray;
		//
		//findCharacter(plateImg, charImg, index, topLeft);
		
		cout << "So ky tu: " << index << endl;

		if (index < 8) {
			if (waitKey(30) >= 0) break;
			continue;
		}

		sortPlate(charImgArray, topLeftArray, index);

		cout << "Bang so xe: ";
		int im = index - 1;
		while (im >= 0) {
			if (im == 5) {
				//char regChar = detectCharacter(charImg[im]);
			}
			else if (im == 4) {
				/*String regChar = detecCharacter(charImg[im]);
				if (regChar not in range 'A' to 'Z') {
				regChar = detectNumber(charImg, im);
				}*/
			}
			else {
				char regChar = detectNumber(charImgArray, im);
				cout << regChar;
			}

			im--;
		}

		cout << endl;
	
		if (waitKey(30) >= 0) break;
		//cout << linesA.size() << " " << linesB.size() << endl;
		//cout << linesA.max_size() << " " << linesB.max_size() << endl;
		//vector<Vec4i>().swap(linesA);
		//vector<Vec4i>().swap(linesB);
		
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}