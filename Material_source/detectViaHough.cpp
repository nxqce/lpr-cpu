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

Mat * findCharacter(Mat input, int &index);
char detectNumber(Mat * charImg, int im);
Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2);
Vec3d linearEquation(double x1, double y1, double x2, double y2);

float vAvg_old = 0;
float hAvg_old = 0;
int flag = 0;

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
		imshow("Edge", edgeImg);


		////FPGA
		//imshow("Input", input);
		//Mat edgeImg = input;
		//threshold(edgeImg, edgeImg, 150, 255, CV_THRESH_BINARY);



		///----------Hough transform-------------
		////vector<Vec4i> linesA, linesB;

		CvMemStorage* linesAMem = cvCreateMemStorage(0);
		CvMemStorage* linesBMem = cvCreateMemStorage(0);

		IplImage edge = edgeImg;

		//HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 30, 50);
		CvSeq* linesA = cvHoughLines2(&edge, linesAMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 70, 30, 50);
		//HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 50, 50);
		CvSeq* linesB = cvHoughLines2(&edge, linesBMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 30, 50, 50);

		///----------Draw lines------------------
		Mat lineImg_A(input.size(), CV_8UC3, Scalar(0, 0, 0));
		Mat lineImgH_A(input.size(), CV_8UC3, Scalar(0, 0, 0));
		Mat lineImgV_A(input.size(), CV_8UC3, Scalar(0, 0, 0));
		vector<Vec3d> linesH, linesV;
			
		Mat lineImg(input.size(), CV_8UC1, Scalar(0));
		int stretch = 0; //Number of pixel to stretch lines
		double tanV = 8;
		double tanH = 0.08;
		int delta = 50;
		for (size_t i = 0; i < linesA->total; i++) {
			Point* l = (Point*)cvGetSeqElem(linesA, i);
			float w = abs(l[0].x - l[1].x);
			float h = abs(l[0].y - l[1].y);
			float tan = h / w;
			if (tan < tanH) {
				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
				line(lineImg_A, Point(l[0].x - delta, l[0].y), Point(l[1].x + delta, l[1].y), Scalar(0, 0, 255), 5, CV_AA);
				line(lineImgH_A, Point(l[0].x - delta, l[0].y), Point(l[1].x + delta, l[1].y), Scalar(0, 0, 255), 5, CV_AA);
				l[0].x = l[0].x - delta;
				l[1].x = l[1].x + delta;

				Vec3d constABC = linearEquation(l[0].x, l[0].y, l[1].x, l[1].y);
				if ((constABC[0] != 0) | (constABC[1] != 0)) linesH.push_back(constABC);
			}
			if (tan > tanV) {
				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
				int y1, y2;
				if (l[0].y > l[1].y) {
					y1 = l[0].y + delta; l[0].y = y1;
					y2 = l[1].y - delta; l[1].y = y2;
				}
				else {
					y1 = l[0].y - delta; l[0].y = y1;
					y2 = l[1].y + delta; l[1].y = y2;
				}
				line(lineImg_A, Point(l[0].x, y1), Point(l[1].x, y2), Scalar(0, 0, 255), 5, CV_AA);
				line(lineImgV_A, Point(l[0].x, y1), Point(l[1].x, y2), Scalar(0, 0, 255), 5, CV_AA);
				
				Vec3d constABC = linearEquation(l[0].x, l[0].y, l[1].x, l[1].y);
				if ((constABC[0] != 0) | (constABC[1] != 0)) linesV.push_back(constABC);
			}
		}
		/*for (size_t i = 0; i < linesB->total; i++) {
			Point* l = (Point*)cvGetSeqElem(linesB, i);
			float w = abs(l[0].x - l[1].x);
			float h = abs(l[0].y - l[1].y);
			float tan = h / w;
			if (tan < tanH) {
				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
				line(lineImg_A, Point(l[0].x - delta, l[0].y), Point(l[1].x + delta, l[1].y), Scalar(0, 0, 255), 1, CV_AA);
			}
			if (tan > tanV) {
				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
				line(lineImg_A, Point(l[0].x, l[0].y - delta), Point(l[1].x, l[1].y + delta), Scalar(0, 0, 255), 1, CV_AA);
			}
		}*/

		///Tim giao diem 
		vector<Vec2i> iPoints_A;
		for (int i = 0; i < linesV.size(); i++) {
			for (int j = 0; j < linesH.size(); j++) {
				Vec2d interXY;
				interXY = interPoint(linesV[i][0], linesV[i][1], linesV[i][2], linesH[j][0], linesH[j][1], linesH[j][2]);
				int x = (int)interXY[0];
				int y = (int)interXY[1];
				if (x < 10 || y < 10) continue;
				if (x > input.cols || y > input.rows) continue;
				if (((int)lineImgH_A.at<cv::Vec3b>(y, x)[2] == 255) && ((int)lineImgV_A.at<cv::Vec3b>(y, x)[2] == 255)) {
					iPoints_A.push_back(Vec2i(x, y));
					line(lineImg_A, Point(x, y), Point(x, y), Scalar(0, 255, 0), 7, CV_AA);
				}
				line(lineImg_A, Point(x, y), Point(x, y), Scalar(255, 0, 0), 3, CV_AA);
			}
		}

		try {
			imshow("Line", lineImg);
			imshow("Line_A", lineImg_A);
		}
		catch (Exception e) {
			cout << "Exception: " << e.what() << endl;
		}

		int vHis[640] = { 0 };
		int hHis[480] = { 0 };

		///Tim histogram theo phuong doc
		for (int i = 0; i < edgeImg.cols; i++) {
			for (int j = 0; j < edgeImg.rows; j++) {
				if (lineImg.at<uchar>(j, i) != 0)
					vHis[i]++;
			}
		}

		///Tim histogram theo phuong ngang
		for (int i = 0; i < edgeImg.rows; i++) {
			for (int j = 0; j < edgeImg.cols; j++) {
				if (lineImg.at<uchar>(i, j) != 0)
					hHis[i]++;
			}
		}

		///Ve histogram len anh bien
		Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
		cvtColor(edgeImg, vhHisImg, CV_GRAY2BGR);

		///Tim nguong trung binh phuong doc
		int vMin = 480;
		int vMax = 0;
		int vSum = 0;
		for (int i = 0; i < 640; i++) {
			if (vHis[i] < vMin) vMin = vHis[i];
			if (vHis[i] > vMax) vMax = vHis[i];
			vSum += vHis[i];
		}
		float vAvgMM = (vMax + vMin) / 2;
		float vAvgS = vSum / 640;

		///Tim nguong trung binh phuong ngang
		int hMin = 640;
		int hMax = 0;
		int hSum = 0;
		for (int i = 0; i < 480; i++) {
			if (hHis[i] < hMin) hMin = hHis[i];
			if (hHis[i] > hMax) hMax = hHis[i];
			hSum += hHis[i];
		}

		float hAvgMM = (hMax + hMin) / 2;
		float hAvgS = hSum / 480;

		float vAvg = (vAvgMM + vAvgS) / 2;
		float hAvg = (hAvgMM + hAvgS) / 2;

		cout << "\n vAvg_new -- " << vAvg << endl;
		cout << "\n hAvg_new -- " << hAvg << endl;

		if (flag == 0) {
			vAvg_old = vAvg;
			hAvg_old = hAvg;
			flag = 1;
		}
		if (flag == 1) {
			if (abs(vAvg - vAvg_old) <= 20.0) {
				vAvg = (vAvg + vAvg_old) / 2.0;
				hAvg = (hAvg + hAvg_old) / 2.0;
			}
			else {
				vAvg_old = vAvg;
				hAvg_old = hAvg;
			}
		}

		cout << "\n vAvg_Old -- " << vAvg_old << endl;
		cout << "\n vAvg_average -- " << vAvg << endl;
		cout << "\n hAvg_Old -- " << hAvg_old << endl;
		cout << "\n hAvg_average -- " << hAvg << endl;

		///ve duong thang
		Vector<int> hLines, vLines;
		int count = 0, start = 0;
		int countMax = 0;
		for (int i = 0; i < 640; i++) {
			if (vHis[i] >= (int)vAvg) {
				//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
				if (count == 0) {
					line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
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
			if (vHis[i] >= (int)vAvg) {
				//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
				if (count == 0) {
					line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
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
			if (hHis[i] >= (int)hAvg) {
				//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
				if (count == 0) {
					line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
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
			if (hHis[i] >= (int)hAvg) {
				//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
				if (count == 0) {
					line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
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


		///ve histogram
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
		if (!iPoints.empty()) {
			for (int i = 0; i < iPoints.size() - 1; i++) {
				for (int j = i + 1; j < iPoints.size(); j++) {
					w = abs(iPoints[i][0] - iPoints[j][0]);
					h = abs(iPoints[i][1] - iPoints[j][1]);
					l = w / h;
					if (w < 120.0 || h < 90.0) continue;
					if (l > 1.3 && l < 1.4) {
						int xTl, xBr, yTl, yBr;
						xTl = find(vLines.begin(), vLines.end(), iPoints[i][0]) - vLines.begin();
						xBr = find(vLines.begin(), vLines.end(), iPoints[j][0]) - vLines.begin();
						yTl = find(hLines.begin(), hLines.end(), iPoints[i][1]) - hLines.begin();
						yBr = find(hLines.begin(), hLines.end(), iPoints[j][1]) - hLines.begin();
						int iSum = abs(xBr - xTl) + abs(yBr - yTl);

						int iVSum = 0;

						int xStart, xEnd;
						xStart = (iPoints[i][0] > iPoints[j][0]) ? iPoints[j][0] : iPoints[i][0];
						xEnd = (iPoints[i][0] > iPoints[j][0]) ? iPoints[i][0] : iPoints[j][0];
						for (int i = xStart; i <= xEnd; i++) iVSum += vHis[i];

						int iHSum = 0;
						int yStart, yEnd;
						yStart = (iPoints[i][1] > iPoints[j][1]) ? iPoints[j][1] : iPoints[i][1];
						yEnd = (iPoints[i][1] > iPoints[j][1]) ? iPoints[i][1] : iPoints[j][1];
						for (int i = yStart; i <= yEnd; i++) iHSum += hHis[i];

						double iAvg = (double)((iVSum / (double)(xEnd - xStart)) + (iHSum / (double)(yEnd - yStart)));


						if (iAvg > iMax) {
							selectedP1 = Point(iPoints[i]);
							selectedP2 = Point(iPoints[j]);
							iMax = iAvg;
						}
						if (iVSum / (double)(xEnd - xStart) >= iVMax) {
							vP1 = Point(iPoints[i]);
							vP2 = Point(iPoints[j]);
							iVMax = iVSum / (double)(xEnd - xStart);
						}
						if (iHSum / (double)(yEnd - yStart) > iHMax) {
							hP1 = Point(iPoints[i]);
							hP2 = Point(iPoints[j]);
							iHMax = iHSum / (double)(yEnd - yStart);
						}
					}
				}
			}
		}



		line(vhHisImg, Point(vP1.x, 0), Point(vP1.x, 479), Scalar(255, 0, 255), 2);
		line(vhHisImg, Point(vP2.x, 0), Point(vP2.x, 479), Scalar(255, 0, 255), 2);
		line(vhHisImg, Point(0, hP1.y), Point(639, hP1.y), Scalar(255, 0, 255), 2);
		line(vhHisImg, Point(0, hP2.y), Point(639, hP2.y), Scalar(255, 0, 255), 2);
		rectangle(vhHisImg, Rect(selectedP1, selectedP2), Scalar(0, 255, 0), 3);

		//Mat plateImg_A;
		double iMax_A = 0;
		Point selectedP1_A = Point(0, 0), selectedP2_A = Point(0, 0);
		if (!iPoints_A.empty()) {
			for (int i = 0; i < iPoints_A.size() - 1; i++) {
				for (int j = i + 1; j < iPoints_A.size(); j++) {
					double w = abs(iPoints_A[i][0] - iPoints_A[j][0]);
					double h = abs(iPoints_A[i][1] - iPoints_A[j][1]);
					double l = w / h;
					//if (w < 200 || h < 200) continue;
					if (l > 1.3 && l < 1.4) {
						int xTl, xBr, yTl, yBr;
						xTl = find(vLines.begin(), vLines.end(), iPoints_A[i][0]) - vLines.begin();
						xBr = find(vLines.begin(), vLines.end(), iPoints_A[j][0]) - vLines.begin();
						yTl = find(hLines.begin(), hLines.end(), iPoints_A[i][1]) - hLines.begin();
						yBr = find(hLines.begin(), hLines.end(), iPoints_A[j][1]) - hLines.begin();
						int iSum = abs(xBr - xTl) + abs(yBr - yTl);

						int iVSum = 0;
						int xStart, xEnd;
						xStart = (iPoints_A[i][0] > iPoints_A[j][0]) ? iPoints_A[j][0] : iPoints_A[i][0];
						xEnd = (iPoints_A[i][0] > iPoints_A[j][0]) ? iPoints_A[i][0] : iPoints_A[j][0];
						for (int i = xStart; i <= xEnd; i++) iVSum += vHis[i];

						int iHSum = 0;
						int yStart, yEnd;
						yStart = (iPoints_A[i][1] > iPoints_A[j][1]) ? iPoints_A[j][1] : iPoints_A[i][1];
						yEnd = (iPoints_A[i][1] > iPoints_A[j][1]) ? iPoints_A[i][1] : iPoints_A[j][1];
						for (int i = yStart; i <= yEnd; i++) iHSum += hHis[i];

						double iAvg = (double)((iVSum / (double)(xEnd - xStart)) + (iHSum / (double)(yEnd - yStart)));


						if (iAvg > iMax_A) {
							selectedP1_A = Point(iPoints_A[i]);
							selectedP2_A = Point(iPoints_A[j]);
							iMax_A = iAvg;
							cout << "\n\n\n\n\n\n\nHELLO\n\n\n\n\n\n";
						}
					}
				}
			}
		}

		if (selectedP1_A != Point(0, 0) | selectedP2_A != Point(0, 0)) {
			rectangle(vhHisImg, Rect(selectedP1_A, selectedP2_A), Scalar(249, 191, 191), 3);
			Mat plateImg_A = input(Rect(Point(selectedP1_A), Point(selectedP2_A)));
			imshow("Plate hough", plateImg_A);
			imshow("Direction Histogram hello", vhHisImg);
			waitKey(1000);
			//system("pause");
			//return 1;
		}
		
		/*plateImg_A = input(Rect(selectedP1_A, selectedP1_A));
		try {
			imshow("Plate hough", plateImg_A);
		}
		catch (Exception e) {
			cout << e.what();
		}*/

		//imshow("Origin", input);
		imshow("Direction Histogram", vhHisImg);

		if (selectedP1 == Point(0, 0) && selectedP2 == Point(0, 0)) {
			if (waitKey(30) >= 0) break;
			continue;
		}

		Mat plateImg = input(Rect(selectedP1, selectedP2));

		///----------Extract characters------------
		//resize(plateImg, plateImg, Size(640, 480)); //270;200
		//Mat plateBlurImg, plateEdgeImg, plateBinImg, plateGrayImg, plateContourImg;

		//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));

		//cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);
		//threshold(plateGrayImg, plateBinImg, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		//erode(plateBinImg, plateBinImg, element);

		//plateContourImg = plateBinImg.clone();

		//dilate(plateBinImg, plateBinImg, element, Point(-1, -1), 2);

		//CvMemStorage *contoursMem = cvCreateMemStorage(0);
		//CvSeq *contours = 0;
		//IplImage plate = plateBinImg;

		//int n = cvFindContours(&plate, contoursMem, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

		//Mat charImg[9];
		//Point2i topLeft[9];
		//int index = 0;

		///// Approximate contours to polygons and get bounding rects
		//vector<vector<Point> > contours_poly(contours->total);
		//vector<CvRect> boundRect(contours->total);

		//CvMemStorage *polyMem = cvCreateMemStorage(0);
		//CvSeq *poly;

		////for (; contours != 0; contours = contours->h_next) {
		//poly = cvApproxPoly(contours, sizeof(CvContour), polyMem, CV_POLY_APPROX_DP, 3, 1);
		//int totalPoly = 0;
		//for (int i = 0; poly != 0; poly = poly->h_next, i++) {
		//	boundRect[i] = cvBoundingRect(poly);
		//	totalPoly++;
		//}

		///Test old bounding
		int index = 0;
		Mat * charImg = new Mat();
		charImg = findCharacter(plateImg, index);
		cout << "So ky tu: " << index << endl;

		cout << "Bang so xe: ";
		int im = index - 1;
		while (im >= 0) {
			//imshow("num " + char(im), charImg[im]);
		
			if (im == 6) {
				//char regChar = detectCharacter(charImg[im]);
			}
			else if (im == 5) {
				/*String regChar = detecCharacter(charImg[im]);
				if (regChar not in range 'A' to 'Z') {
					regChar = detectNumber(charImg, im);
				}*/
			}
			else {
				char regChar = detectNumber(charImg, im);
				cout << regChar << "     ---     ";
			}
			
			im--;
		}

		if (waitKey(30) >= 0) break;
		//cout << linesA.size() << " " << linesB.size() << endl;
		//cout << linesA.max_size() << " " << linesB.max_size() << endl;
		//vector<Vec4i>().swap(linesA);
		//vector<Vec4i>().swap(linesB);

	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

Mat * findCharacter(Mat input, int &index) {
	resize(input, input, Size(640, 480));
	
	Mat plateBlurImg, plateEdgeImg, plateBinImg, plateGrayImg, plateContourImg;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	
	cvtColor(input, plateGrayImg, CV_BGR2GRAY);
	threshold(plateGrayImg, plateBinImg, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	erode(plateBinImg, plateBinImg, element);
	
	plateContourImg = plateBinImg.clone();
	dilate(plateBinImg, plateBinImg, element, Point(-1, -1), 2);
	
	vector < vector<Point> > contours;
	findContours(plateContourImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	//cvFindContours(CvArr* image, CvMemStorage* storage, CvSeq** first_contour, int header_size = sizeof(CvContour), int mode = CV_RETR_LIST, int method = CV_CHAIN_APPROX_SIMPLE, CvPoint offset = cvPoint(0, 0))
	
	Mat * charImg = new Mat[9];
	Point2i topLeft[9];
	//int index = 0;
	
	/// Approximate contours to polygons and get bounding rects
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	
	/// Draw bonding rects
	for (int i = 0; i< contours.size(); i++) {
		Rect r = boundRect[i];
		if ((r.width / (double)r.height > 0.2f && r.width / (double)r.height < 0.6f)
			&& (r.height < 240 && r.height > 140)) {
			topLeft[index] = r.tl();
			rectangle(input, r, Scalar(0, 0, 255), 1, 8, 0);
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
	
	imshow("Bin", plateBinImg);
	imshow("fuck", input);
	
	return charImg;
}

char detectNumber(Mat * charImg, int im) {
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
			if (black > white / 4) {
				//if (white > 0){
				charSquare[c][l] = 1;
				charBit += pow(2, l * 3 + c);
			}
		}
	}

	if (charImg[im].cols < 30) {
		regChar = '1';
	}
	else {
		regChar = getChar(charBit);
	}

	return regChar;
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
