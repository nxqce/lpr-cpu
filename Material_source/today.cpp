//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <String.h>
//
////#include "charAlphabetDetection.cpp"
//
//using namespace cv;
//using namespace std;
///*
//int main(int, char**)
//{
//VideoCapture cap(0); // open the default camera
//if (!cap.isOpened())  // check if we succeeded
//return -1;
//Mat edges;
//namedWindow("edges", 1);
//for (;;)
//{
//Mat frame;
//cap >> frame; // get a new frame from camera
//cvtColor(frame, edges, CV_BGR2GRAY);
//GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
//Canny(edges, edges, 0, 30, 3);
//imshow("edges", edges);
//if (waitKey(30) >= 0) break;
//}
//// the camera will be deinitialized automatically in VideoCapture destructor
//return 0;
//}
//*/
//
//#include <memory.h>
//#include "PCIE.h"
//
//
//#define DEMO_PCIE_USER_BAR			PCIE_BAR0
//#define DEMO_PCIE_IO_LED_ADDR		0x00
//#define DEMO_PCIE_IO_BUTTON_ADDR	0x20
//#define DEMO_PCIE_FIFO_WRITE_ADDR	0x40
//#define DEMO_PCIE_FIFO_STATUS_ADDR	0x60
//#define DEMO_PCIE_FIFO_READ_ADDR	0x80
//#define DEMO_PCIE_MEM_ADDR			0x80000
//#define DEMO_PCIE_MEM_ADDR_1		0x180000
//
//#define MEM_SIZE			(300*1024) //128KB
//#define FIFO_SIZE			(16*1024) // 2KBx8
//
//
//double xSolve(double a1, double b1, double c1, double a2, double b2, double c2);
//double ySolve(double a1, double b1, double c1, double a2, double b2, double c2);
//int dec2bin(int num);
//char getChar(int code);
//Vec3d linearEquation(double x1, double y1, double x2, double y2);
//Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2);
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
//int checkPF(Mat input) {
//	Mat l = input(Rect(0, input.rows / 6.0, input.cols / 4.0, input.rows / 6.0 * 5));
//	/*cvtColor(l, l, CV_BGR2GRAY);
//	threshold(l, l, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/
//
//	int black = 0, white = 0;
//	for (int i = 0; i < l.cols; i++) {
//		for (int j = 0; j < l.rows; j++) {
//			if ((int)l.at<uchar>(Point(i, j)) == 0) black++;
//			else white++;
//		}
//	}
//
//	return (white >= black * 3.0) ? 0 : 1;
//}
//
//String checkRightEdge(Mat input) {
//	String result;
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
//	if (checkPF(input) == 0) return result;
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
//int checkBD(Mat input) {
//	Mat tl = input(Rect(0, 0, input.cols / 4.0, input.rows / 8.0));
//	Mat bl = input(Rect(0, input.rows / 8.0 * 7, input.cols / 4.0, input.rows / 8.0));
//
//	float black_tl = 0, white_tl = 0, black_bl = 0, white_bl = 0;
//	for (int i = 0; i < tl.cols; i++) {
//		for (int j = 0; j < tl.rows; j++) {
//			if ((int)tl.at<uchar>(Point(i, j)) == 0) black_tl++;
//			else white_tl++;
//			if ((int)bl.at<uchar>(Point(i, j)) == 0) black_bl++;
//			else white_bl++;
//		}
//	}
//
//	if ((white_tl + white_bl) >= (black_bl + black_tl)) return 0;
//	else return 1;
//}
//
//String checkMidEdge(Mat input) {
//	String result;
//
//	Mat t = input(Rect(0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
//	Mat m = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
//	//imshow("M", m);
//
//	/*cvtColor(input, input, CV_BGR2GRAY);
//	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//*/
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
//	case 1111111110:
//		result = "R";
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
//			if (checkBD(input) == 0) return result;
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
//
//double xSolve(double a1, double b1, double c1, double a2, double b2, double c2) {
//	double d = a1*b2 - a2*b1;
//	double dx = c1*b2 - c2*b1;
//	double dy = c2*a1 - c1*a2;
//	if (dx == 0 || dy == 0) return -1;
//	return dx / d;
//}
//
//double ySolve(double a1, double b1, double c1, double a2, double b2, double c2) {
//	double d = a1*b2 - a2*b1;
//	double dx = c1*b2 - c2*b1;
//	double dy = c2*a1 - c1*a2;
//	if (dx == 0 || dy == 0) return -1;
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
//	else {
//		cout << a1 << " " << b1 << " " << c1 << endl;
//		cout << a2 << " " << b2 << " " << c2 << endl;
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
//void findCharacter(Mat input, Mat *charImg, int &index, Point2i *topLeft) {
//	//cout << "Debug: findCharacter()" << endl;
//	//----------Extract characters------------
//	Mat plateImg = input;
//	//resize(plateImg, plateImg, Size(640, 480)); //270;200
//	resize(plateImg, plateImg, Size(270, 200));
//	//imshow("Plate", plateImg);
//
//	Mat plateBlurImg, plateEdgeImg, plateBinImg, plateGrayImg, plateContourImg;
//
//	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
//
//	cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);
//	threshold(plateGrayImg, plateBinImg, 80, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
//	//adaptiveThreshold(plateGrayImg, plateBinImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 1);
//
//	//dilate(plateBinImg, plateBinImg, element, Point(-1,-1), 2);
//	erode(plateBinImg, plateBinImg, element);
//
//	plateContourImg = plateBinImg.clone();
//	//imshow("Bin", plateBinImg);
//
//	//dilate(plateBinImg, plateBinImg, element, Point(-1, -1), 2);
//
//	//vector < vector<Point> > contours;
//	//findContours(plateEdgeImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
//
//	CvMemStorage *contoursMem = cvCreateMemStorage(0);
//	CvSeq *contours = 0;
//	IplImage plate = plateContourImg;
//
//	int n = cvFindContours(&plate, contoursMem, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
//
//	//Mat charImgArray[9];
//	//Mat * charImg = charImgArray;
//	//Point2i topLeft[9];
//	//int index = 0;
//
//	/// Approximate contours to polygons and get bounding rects
//	vector<vector<Point> > contours_poly(n);
//	vector<CvRect> boundRect(n);
//
//	CvMemStorage *polyMem = cvCreateMemStorage(0);
//	CvSeq *poly;
//
//	poly = cvApproxPoly(contours, sizeof(CvContour), polyMem, CV_POLY_APPROX_DP, 3, 1);
//	int totalPoly = 0;
//	for (CvSeq *i = poly; i != NULL; i = i->h_next) {
//		boundRect[totalPoly] = cvBoundingRect((CvContour*)i, 1);
//		totalPoly++;
//	}
//
//	/// Draw bonding rects
//	// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
//	// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);
//
//	for (int i = 0; i < totalPoly; i++) {
//		if (index > 8) break;
//		Rect r = boundRect[i];
//		double rate = r.width / (double)r.height;
//		if ((rate > 0.2f && rate < 0.6f) && (r.height < 100 && r.height > 40)) {
//			topLeft[index] = r.tl();
//			rectangle(plateImg, r, Scalar(0, 0, 255), 1, 8, 0);
//			charImg[index] = plateBinImg(r);
//			index++;
//		}
//	}
//	//imshow("plateBin", plateBinImg);
//	if (index > 7 && index < 10) imshow("plate", plateImg);
//	//waitKey(0);
//
//	cvClearMemStorage(contoursMem);
//	cvClearMemStorage(polyMem);
//
//	cvReleaseMemStorage(&contoursMem);
//	cvReleaseMemStorage(&polyMem);
//
//	return;
//}
//
//void sortPlate(Mat *charImg, Point2i *topLeft, int index){
//	//cout << "Debug: sortPlate" << endl;
//	//Sort the charImg array
//	int top = 0;
//	for (int i = 0; i < index; i++) {
//		if (topLeft[i].y < 50) {
//			top = i;
//			break;
//		}
//	}
//
//	for (int i = 0; i < top - 1; i++) {
//		for (int j = i; j < top; j++) {
//			if (topLeft[i].x < topLeft[j].x) {
//				Point2i tempPoint = topLeft[i];
//				topLeft[i] = topLeft[j];
//				topLeft[j] = tempPoint;
//
//				Mat tempImg = charImg[i];
//				charImg[i] = charImg[j];
//				charImg[j] = tempImg;
//			}
//		}
//	}
//
//	for (int i = top; i < index - 1; i++) {
//		for (int j = i; j < index; j++) {
//			if (topLeft[i].x < topLeft[j].x) {
//				Point2i tempPoint = topLeft[i];
//				topLeft[i] = topLeft[j];
//				topLeft[j] = tempPoint;
//
//				Mat tempImg = charImg[i];
//				charImg[i] = charImg[j];
//				charImg[j] = tempImg;
//			}
//		}
//	}
//
//	return;
//}
//
//char detectNumber(Mat * charImg, int im) {
//	//cout << "Debug: detectNumber" << endl;
//	int charSquare[3][6] = { { 0 } };
//	int charBit = 0;
//	char regChar = '-';
//
//	int black = 0, white = 0;
//	for (int l = 0; l < 6; l++) {
//		for (int c = 0; c < 3; c++) {
//			white = 0;
//			black = 0;
//			for (int i = 0 + c*charImg[im].cols / 3; i < (c + 1)*charImg[im].cols / 3; i++) {
//				for (int j = 0 + l*charImg[im].rows / 6; j < (l + 1)*charImg[im].rows / 6; j++) {
//					//cout << (int)charImg[0].at<uchar>(Point(i, j)) << " ";
//					if ((int)charImg[im].at<uchar>(Point(i, j)) == 0) black++;
//					else white++;
//				}
//				//cout << endl;
//			}
//			// cout << "[" << l << "," << c << "]" << endl;
//			// cout << "black: " << black << " - white: " << white << endl;
//			// cout << "---------------------------------" << endl;
//			if (white > black / 4) {
//				//if (white > 0){
//				charSquare[c][l] = 1;
//				charBit += pow(2, l * 3 + c);
//			}
//		}
//	}
//
//	//cout << charImg[im].cols << endl;
//
//	if (charImg[im].cols < 25) {
//		regChar = '1';
//	}
//	else {
//		//cout << charBit << endl;
//		regChar = getChar(charBit);
//	}
//
//	return regChar;
//}
//
//char PAT_GEN(Mat nIndex, int x, int y) {
//	char Data;
//	Data = nIndex.at<uchar>(y, x) & 0xFF;
//	return Data;
//}
//
//BOOL writeImgToDmaMem0(PCIE_HANDLE hPCIe, Mat input) {
//	BOOL bPass = TRUE;
//	int i;
//	const int nTestSize = MEM_SIZE;
//	const PCIE_LOCAL_ADDRESS LocalAddr = DEMO_PCIE_MEM_ADDR;
//	char *pWrite;
//	char *pRead;
//	char szError[256];
//
//
//	pWrite = (char *)malloc(nTestSize);
//	if (!pWrite) {
//		bPass = FALSE;
//		sprintf(szError, "DMA Memory:malloc failed\r\n");
//	}
//
//
//	// init test pattern
//	int n = 0;
//	for (i = 0; i<input.cols && bPass; i++)
//	for (int j = 0; j < input.rows; j++) {
//		*(pWrite + n) = PAT_GEN(input, i, j);
//		n++;
//	}
//
//	// write test pattern
//	if (bPass) {
//		bPass = PCIE_DmaWrite(hPCIe, LocalAddr, pWrite, nTestSize);
//		if (!bPass)
//			sprintf(szError, "DMA Memory:PCIE_DmaWrite failed\r\n");
//	}
//	printf("Write to RAM 0 successfully!\n");
//
//
//	// free resource
//	if (pWrite)
//		free(pWrite);
//
//	return bPass;
//}
//
//BOOL readImgFromDmaMem1(PCIE_HANDLE hPCIe, Mat *output) {
//	BOOL bPass = TRUE;
//	int i;
//	const int nTestSize = MEM_SIZE;
//	const PCIE_LOCAL_ADDRESS LocalAddr_1 = DEMO_PCIE_MEM_ADDR_1;
//	char *pRead_1;
//	char szError[256];
//
//
//	pRead_1 = (char *)malloc(nTestSize);
//	if (!pRead_1) {
//		bPass = FALSE;
//		sprintf(szError, "DMA Memory:malloc failed\r\n");
//	}
//
//
//	// read
//	if (bPass) {
//		bPass = PCIE_DmaRead(hPCIe, LocalAddr_1, pRead_1, nTestSize);
//		Mat temp(480, 640, CV_8UC1);
//
//		if (!bPass) {
//			sprintf(szError, "DMA Memory 1:PCIE_DmaRead failed\r\n");
//		}
//		else {
//			for (i = 0; i < nTestSize && bPass; i++) {
//				temp.at<uchar>(i % 480, i / 480) = *(pRead_1 + i);
//			}
//		}
//		// imshow("out1", temp);
//		*output = temp.clone();
//	}
//
//	if (pRead_1)
//		free(pRead_1);
//
//	if (!bPass)
//		printf("%s", szError);
//	else
//		printf("DMA-Memory (Size = %d byes) pass\r\n", nTestSize);
//
//
//	return bPass;
//}
//
//int main() {
//	///----------Khoi tao PCIE 
//	void *lib_handle;
//	PCIE_HANDLE hPCIE;
//	BOOL bQuit = FALSE;
//	int nSel;
//
//	printf("== Terasic: PCIe Demo Program ==\r\n");
//
//	lib_handle = PCIE_Load();
//	if (!lib_handle) {
//		printf("PCIE_Load failed!\r\n");
//		return 0;
//	}
//
//	//hPCIE = PCIE_Open(0, 0, 0);
//	hPCIE = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
//	if (!hPCIE) {
//		printf("PCIE_Open failed\r\n");
//		return 0;
//	}
//
//	///----------Bat dau chay camera-----------------------------
//	VideoCapture cap(0); // open the default camera
//	if (!cap.isOpened())  // check if we succeeded
//		return -1;
//	//cap.set(CV_CAP_PROP_FPS, 15);
//
//	//Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
//	namedWindow("Direction Histogram", 1);
//	for (;;)
//	{
//		///----------Gửi tin hiệu chưa chuyển ảnh sang canny---------
//		// enb = 0;
//		BOOL bPass;
//		int Mask = 0;
//		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
//		printf("Disable flag is sent\n");
//
//		Mat frame;
//		cap >> frame; // get a new frame from camera
//
//		Mat input;
//		resize(frame, input, Size(640, 480));
//
//		///---------Noise filter---------------
//		Size size(5, 5);
//		GaussianBlur(input, input, size, 0);
//
//		///---------Convert to gray image
//		Mat grayImg, grayFilterImg;
//		cvtColor(input, grayImg, CV_BGR2GRAY);
//
//		///---------Ghi anh xam xuong FPGA
//		writeImgToDmaMem0(hPCIE, grayImg);
//		printf("The gray image is sent\n");
//
//		///---------Gui tin hieu enable
//		Mask = 4;
//		printf("\nMask = %d\n", Mask);
//
//		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
//		if (bPass) printf("Enable flag is sent\n");
//
//		///---------Wait for FPGA Edge detection done
//		bPass = TRUE;
//		DWORD Status;
//
//		do {
//			bPass = PCIE_Read32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_BUTTON_ADDR, &Status);
//			printf("\n%d\n", Status);
//			if (!bPass) printf("Read failed\n");
//		} while (Status % 2 == 1);
//		printf("\n%d - FPGA Done\n", Status);
//
//		///---------Read the edge image
//		Mat output(480, 640, CV_8UC1);
//		readImgFromDmaMem1(hPCIE, &output);
//		printf("\nEdge image is read\n");
//
//		///---------Transfer canny to threshold
//		imshow("From FPGA", output);
//		Mat edgeImg = output;
//		threshold(edgeImg, edgeImg, 100, 255, CV_THRESH_BINARY);
//
//		///----------Hough transform-------------
//		CvMemStorage* linesAMem = cvCreateMemStorage(0);
//		CvMemStorage* linesBMem = cvCreateMemStorage(0);
//
//		IplImage edge = edgeImg;
//
//		//HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 30, 50);
//		CvSeq* linesA = cvHoughLines2(&edge, linesAMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 70, 30, 50);
//		//HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 50, 50);
//		CvSeq* linesB = cvHoughLines2(&edge, linesBMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 30, 50, 50);
//
//		///----------Draw lines------------------
//		Mat lineImg(input.size(), CV_8UC1, Scalar(0));
//		int stretch = 0; //Number of pixel to stretch lines
//		double tanV = 11;
//		double tanH = 0.08;
//		for (size_t i = 0; i < linesA->total; i++) {
//			Point* l = (Point*)cvGetSeqElem(linesA, i);
//			float w = abs(l[0].x - l[1].x);
//			float h = abs(l[0].y - l[1].y);
//			float tan = h / w;
//			if (tan < tanH) {
//				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
//			}
//			if (tan > tanV) {
//				line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
//			}
//		}
//		//for (size_t i = 0; i < linesB->total; i++) {
//		//	Point* l = (Point*)cvGetSeqElem(linesB, i);
//		//	float w = abs(l[0].x - l[1].x);
//		//	float h = abs(l[0].y - l[1].y);
//		//	float tan = h / w;
//		//	if (tan < tanH) {
//		//		line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
//		//	}
//		//	if (tan > tanV) {
//		//		line(lineImg, l[0], l[1], Scalar(255), 1, CV_AA);
//		//	}
//		//}
//
//		//imshow("Line", lineImg);
//
//		int vHis[640] = { 0 };
//		int hHis[480] = { 0 };
//
//		//Tim histogram theo phuong doc
//		for (int i = 0; i < edgeImg.cols; i++) {
//			for (int j = 0; j < edgeImg.rows; j++) {
//				if (lineImg.at<uchar>(j, i) != 0)
//					vHis[i]++;
//			}
//		}
//
//		//Tim histogram theo phuong ngang
//		for (int i = 0; i < edgeImg.rows; i++) {
//			for (int j = 0; j < edgeImg.cols; j++) {
//				if (lineImg.at<uchar>(i, j) != 0)
//					hHis[i]++;
//			}
//		}
//
//		//Ve histogram len anh bien
//		Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
//		cvtColor(edgeImg, vhHisImg, CV_GRAY2BGR);
//
//		//Tim nguong trung binh phuong doc
//		int vMin = 480;
//		int vMax = 0;
//		int vSum = 0;
//		for (int i = 0; i < 640; i++) {
//			if (vHis[i] < vMin) vMin = vHis[i];
//			if (vHis[i] > vMax) vMax = vHis[i];
//			vSum += vHis[i];
//		}
//		int vAvgMM = (vMax + vMin) / 2;
//		int vAvgS = vSum / 640;
//
//		//Tim nguong trung binh phuong ngang
//		int hMin = 640;
//		int hMax = 0;
//		int hSum = 0;
//		for (int i = 0; i < 480; i++) {
//			if (hHis[i] < hMin) hMin = hHis[i];
//			if (hHis[i] > hMax) hMax = hHis[i];
//			hSum += hHis[i];
//		}
//
//		int hAvgMM = (hMax + hMin) / 2;
//		int hAvgS = hSum / 480;
//
//		int vAvg = (vAvgMM + vAvgS) / 2;
//		int hAvg = (hAvgMM + hAvgS) / 2;
//
//		//ve duong thang
//		Vector<int> hLines, vLines;
//		int count = 0, start = 0;
//		int countMax = 0;
//		for (int i = 0; i < 640; i++) {
//			if (vHis[i] >= vAvg) {
//				//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
//				if (count == 0) {
//					//line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
//					vLines.push_back(i);
//				}
//				start = 1;
//			}
//			if (start == 1) {
//				count++;
//				if (count > countMax) start = 0;
//			}
//			else {
//				count = 0;
//			}
//		}
//		count = 0;
//		start = 0;
//		for (int i = 639; i >= 0; i--) {
//			if (vHis[i] >= vAvg) {
//				//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
//				if (count == 0) {
//					//line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
//					vLines.push_back(i);
//				}
//				start = 1;
//			}
//			if (start == 1) {
//				count++;
//				if (count > countMax) start = 0;
//			}
//			else {
//				count = 0;
//			}
//		}
//
//		count = 0;
//		start = 0;
//		for (int i = 0; i < 480; i++) {
//			if (hHis[i] >= hAvg) {
//				//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
//				if (count == 0) {
//					//line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
//					hLines.push_back(i);
//				}
//				start = 1;
//			}
//			if (start == 1) {
//				count++;
//				if (count > countMax) start = 0;
//			}
//			else {
//				count = 0;
//			}
//		}
//		count = 0;
//		start = 0;
//		for (int i = 479; i >= 0; i--) {
//			if (hHis[i] >= hAvg) {
//				//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
//				if (count == 0) {
//					//line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
//					hLines.push_back(i);
//				}
//				start = 1;
//			}
//			if (start == 1) {
//				count++;
//				if (count > countMax) start = 0;
//			}
//			else {
//				count = 0;
//			}
//		}
//
//
//		//ve histogram
//		for (int i = 0; i < 640; i++) {
//			line(vhHisImg, Point(i, 480), Point(i, 480 - vHis[i]), Scalar(255, 255, 0), 1);
//		}
//
//		for (int i = 0; i < 480; i++) {
//			line(vhHisImg, Point(0, i), Point(hHis[i], i), Scalar(255, 255, 0), 1);
//		}
//
//		//duong trung binh
//		line(vhHisImg, Point(0, 480 - vAvgMM), Point(639, 480 - vAvgMM), Scalar(0, 255, 0), 1);
//		line(vhHisImg, Point(0, 480 - vAvgS), Point(639, 480 - vAvgS), Scalar(0, 0, 255), 1);
//
//		line(vhHisImg, Point(hAvgMM, 0), Point(hAvgMM, 480), Scalar(0, 255, 0), 1);
//		line(vhHisImg, Point(hAvgS, 0), Point(hAvgS, 480), Scalar(0, 0, 255), 1);
//
//		//Tim tat ca cac giao diem
//		Vector<Vec2i> iPoints;
//		for (int i = 0; i < vLines.size(); i++) {
//			for (int j = 0; j < hLines.size(); j++) {
//				iPoints.push_back(Vec2i(vLines[i], hLines[j]));
//				line(vhHisImg, Point(vLines[i], hLines[j]), Point(vLines[i], hLines[j]), Scalar(0, 0, 255), 3);
//			}
//		}
//
//
//		double w, h, l;
//		double iMax = 0, iHMax = 0, iVMax = 0;
//		Point selectedP1 = Point(0, 0), selectedP2 = Point(0, 0);
//		Point vP1 = Point(0, 0), vP2 = Point(0, 0), hP1 = Point(0, 0), hP2 = Point(0, 0);
//		int found = 0;
//
//		int index = 0;
//		Mat charImgArray[9];
//		//Mat * charImg = charImgArray;
//		Point2i topLeftArray[9];
//		//Point2i * topLeft = topLeftArray;
//
//		if (!iPoints.empty()) {
//			for (int i = 0; i < iPoints.size() - 1; i++) {
//				for (int j = i + 1; j < iPoints.size(); j++) {
//					w = abs(iPoints[i][0] - iPoints[j][0]);
//					h = abs(iPoints[i][1] - iPoints[j][1]);
//					l = w / h;
//					if (w < 120.0 || h < 90.0) continue;
//					if (l > 1.3 && l < 1.4) {
//						//int xTl, xBr, yTl, yBr;
//						//xTl = find(vLines.begin(), vLines.end(), iPoints[i][0]) - vLines.begin();
//						//xBr = find(vLines.begin(), vLines.end(), iPoints[j][0]) - vLines.begin();
//						//yTl = find(hLines.begin(), hLines.end(), iPoints[i][1]) - hLines.begin();
//						//yBr = find(hLines.begin(), hLines.end(), iPoints[j][1]) - hLines.begin();
//						//int iSum = abs(xBr - xTl) + abs(yBr - yTl);
//
//						//int iVSum = 0;
//
//						//int xStart, xEnd;
//						//xStart = (iPoints[i][0] > iPoints[j][0]) ? iPoints[j][0] : iPoints[i][0];
//						//xEnd = (iPoints[i][0] > iPoints[j][0]) ? iPoints[i][0] : iPoints[j][0];
//						//for (int i = xStart; i <= xEnd; i++) iVSum += vHis[i];
//
//						//int iHSum = 0;
//						//int yStart, yEnd;
//						//yStart = (iPoints[i][1] > iPoints[j][1]) ? iPoints[j][1] : iPoints[i][1];
//						//yEnd = (iPoints[i][1] > iPoints[j][1]) ? iPoints[i][1] : iPoints[j][1];
//						//for (int i = yStart; i <= yEnd; i++) iHSum += hHis[i];
//
//						//double iAvg = (double)((iVSum / (double)(xEnd - xStart)) + (iHSum / (double)(yEnd - yStart)));
//
//
//						//if (iAvg > iMax) {
//						//	selectedP1 = Point(iPoints[i]);
//						//	selectedP2 = Point(iPoints[j]);
//						//	iMax = iAvg;
//						//}
//						//if (iVSum / (double)(xEnd - xStart) >= iVMax) {
//						//	vP1 = Point(iPoints[i]);
//						//	vP2 = Point(iPoints[j]);
//						//	iVMax = iVSum / (double)(xEnd - xStart);
//						//}
//						//if (iHSum / (double)(yEnd - yStart) > iHMax) {
//						//	hP1 = Point(iPoints[i]);
//						//	hP2 = Point(iPoints[j]);
//						//	iHMax = iHSum / (double)(yEnd - yStart);
//						//}
//
//						Point tempP1 = Point(iPoints[i]);
//						Point tempP2 = Point(iPoints[j]);
//						Mat tempPlateImg = input(Rect(tempP1, tempP2));
//
//						int tempIndex = 0;
//						Mat tempCharImgArray[9];
//						//Mat * tempCharImg = new Mat[9];
//						Point2i tempTopLeftArray[9];
//						//Point2i * tempTopLeft = new Point2i[9];
//
//						findCharacter(tempPlateImg, charImgArray, tempIndex, topLeftArray);
//
//						if (tempIndex > 7 && tempIndex < 10) {
//							selectedP1 = tempP1;
//							selectedP2 = tempP2;
//
//							index = tempIndex;
//							//for(int i = 0; i < index; i++) charImgArray[i] = tempCharImgArray[i].clone();
//							//for (int i = 0; i < index; i++) topLeftArray[i] = tempTopLeftArray[i];
//
//							//charImg = tempCharImg;
//							//topLeft = tempTopLeft;
//
//							found = 1;
//							break;
//						}
//
//					}
//				}
//				if (found) break;
//			}
//		}
//
//
//
//		//line(vhHisImg, Point(vP1.x, 0), Point(vP1.x, 479), Scalar(255, 0, 255), 2);
//		//line(vhHisImg, Point(vP2.x, 0), Point(vP2.x, 479), Scalar(255, 0, 255), 2);
//		//line(vhHisImg, Point(0, hP1.y), Point(639, hP1.y), Scalar(255, 0, 255), 2);
//		//line(vhHisImg, Point(0, hP2.y), Point(639, hP2.y), Scalar(255, 0, 255), 2);
//		rectangle(vhHisImg, Rect(selectedP1, selectedP2), Scalar(0, 255, 0), 3);
//
//
//		//imshow("Origin", input);
//		imshow("Direction Histogram", vhHisImg);
//
//		//if (selectedP1 == Point(0, 0) && selectedP2 == Point(0, 0)) {
//		//	if (waitKey(30) >= 0) break;
//		//	continue;
//		//}
//
//		//Mat plateImg = input(Rect(selectedP1, selectedP2));
//
//		//cout << "Debug: cropping the plate completed" << endl;
//
//		//int index = 0;
//		//Mat charImgArray[9];
//		//Mat * charImg = charImgArray;
//		//Point2i topLeftArray[9];
//		//Point2i * topLeft = topLeftArray;
//		//
//		//findCharacter(plateImg, charImg, index, topLeft);
//
//		cout << "So ky tu: " << index << endl;
//
//		if (index < 8) {
//			if (waitKey(30) >= 0) break;
//			continue;
//		}
//
//		sortPlate(charImgArray, topLeftArray, index);
//
//		cout << "Bang so xe: ";
//		int im = index - 1;
//		while (im >= 0) {
//			if ((im == 5 && index == 8) || (im == 6 && index == 9)) {
//				//char regChar = detectCharacter(charImg[im]);
//				cout << " - ";
//				//threshold(charImgArray[im], charImgArray[im], 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//				bitwise_not(charImgArray[im], charImgArray[im]);
//				imshow("Character", charImgArray[im]);
//				String regChar = detectChar(charImgArray[im]);
//				cout << regChar;
//			}
//			else if ((im == 4 && index == 8) || (im == 5 && index == 9)) {
//				/*String regChar = detecCharacter(charImg[im]);
//				if (regChar not in range 'A' to 'Z') {
//				regChar = detectNumber(charImg, im);
//				}*/
//				
//				cout << "	";
//			}
//			else {
//				char regChar = detectNumber(charImgArray, im);
//				cout << regChar;
//			}
//
//			if (im == 2) cout << ".";
//
//			im--;
//		}
//
//		cout << endl;
//
//		if (waitKey(30) >= 0) break;
//		//cout << linesA.size() << " " << linesB.size() << endl;
//		//cout << linesA.max_size() << " " << linesB.max_size() << endl;
//		//vector<Vec4i>().swap(linesA);
//		//vector<Vec4i>().swap(linesB);
//
//	}
//	// the camera will be deinitialized automatically in VideoCapture destructor
//	PCIE_Close(hPCIE);
//	PCIE_Unload(lib_handle);
//
//	return 0;
//}

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "PCIE.h"
#include <ctime>


#define DEMO_PCIE_USER_BAR			PCIE_BAR0
#define DEMO_PCIE_IO_LED_ADDR		0x00
#define DEMO_PCIE_IO_BUTTON_ADDR	0x20
#define DEMO_PCIE_FIFO_WRITE_ADDR	0x40
#define DEMO_PCIE_FIFO_STATUS_ADDR	0x60
#define DEMO_PCIE_FIFO_READ_ADDR	0x80
#define DEMO_PCIE_MEM_ADDR			0x80000
#define DEMO_PCIE_MEM_ADDR_1		0x180000

#define MEM_SIZE			(300*1024) //128KB
#define FIFO_SIZE			(16*1024) // 2KBx8


char PAT_GEN(Mat nIndex, int x, int y) {
	char Data;
	Data = nIndex.at<uchar>(y, x) & 0xFF;
	return Data;
}

BOOL writeImgToDmaMem0(PCIE_HANDLE hPCIe, Mat input) {
	BOOL bPass = TRUE;
	int i;
	const int nTestSize = MEM_SIZE;
	const PCIE_LOCAL_ADDRESS LocalAddr = DEMO_PCIE_MEM_ADDR;
	char *pWrite;
	char *pRead;
	char szError[256];


	pWrite = (char *)malloc(nTestSize);
	if (!pWrite) {
		bPass = FALSE;
		sprintf(szError, "DMA Memory:malloc failed\r\n");
	}


	// init test pattern
	int n = 0;
	for (i = 0; i<input.cols && bPass; i++)
	for (int j = 0; j < input.rows; j++) {
		*(pWrite + n) = PAT_GEN(input, i, j);
		n++;
	}

	// write test pattern
	if (bPass) {
		bPass = PCIE_DmaWrite(hPCIe, LocalAddr, pWrite, nTestSize);
		if (!bPass)
			sprintf(szError, "DMA Memory:PCIE_DmaWrite failed\r\n");
	}
	printf("Write to RAM 0 successfully!\n");


	// free resource
	if (pWrite)
		free(pWrite);

	return bPass;
}

BOOL readImgFromDmaMem1(PCIE_HANDLE hPCIe, Mat *output) {
	BOOL bPass = TRUE;
	int i;
	const int nTestSize = MEM_SIZE;
	const PCIE_LOCAL_ADDRESS LocalAddr_1 = DEMO_PCIE_MEM_ADDR_1;
	char *pRead_1;
	char szError[256];


	pRead_1 = (char *)malloc(nTestSize);
	if (!pRead_1) {
		bPass = FALSE;
		sprintf(szError, "DMA Memory:malloc failed\r\n");
	}


	// read
	if (bPass) {
		bPass = PCIE_DmaRead(hPCIe, LocalAddr_1, pRead_1, nTestSize);
		Mat temp(480, 640, CV_8UC1);

		if (!bPass) {
			sprintf(szError, "DMA Memory 1:PCIE_DmaRead failed\r\n");
		}
		else {
			for (i = 0; i < nTestSize && bPass; i++) {
				temp.at<uchar>(i % 480, i / 480) = *(pRead_1 + i);
			}
		}
		// imshow("out1", temp);
		*output = temp.clone();
	}

	if (pRead_1)
		free(pRead_1);

	if (!bPass)
		printf("%s", szError);
	else
		printf("DMA-Memory (Size = %d byes) pass\r\n", nTestSize);


	return bPass;
}

int main() {
	void *lib_handle;
	PCIE_HANDLE hPCIE;
	BOOL bQuit = FALSE;
	int nSel;

	//printf("== Terasic: PCIe Demo Program ==\r\n");

	lib_handle = PCIE_Load();
	if (!lib_handle) {
		printf("PCIE_Load failed!\r\n");
		return 0;
	}

	//hPCIE = PCIE_Open(0, 0, 0);
	hPCIE = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
	if (!hPCIE) {
		printf("PCIE_Open failed\r\n");
		return 0;
	}

	String filename;
	cin >> filename;
	filename += ".jpg";

	for (int i = 0; i < 10; i++){
		

		Mat input = imread(filename);
		//imshow("input", input);

		resize(input, input, Size(640, 480));

		// enb = 0;
		BOOL bPass;
		int Mask = 0;
		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
		//printf("Disable flag is sent\n");

		//Convert to grayscale image
		Mat grayImg;
		cvtColor(input, grayImg, CV_BGR2GRAY);

		//imshow("Gray", grayImg);
		// TEST_DMA_MEMORY(hPCIE, input);
		clock_t start_write_img = clock();
		writeImgToDmaMem0(hPCIE, grayImg);
		clock_t end_write_img = clock();
		//printf("The gray image is sent\n");

		//Enable
		// int enb = 1;
		Mask = 4;
		//printf("Mask = %d\n", Mask);

		clock_t start_before_en = clock();
		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
		clock_t end_write_en = clock();
		if (bPass) printf("Enable flag is sent\n");
		clock_t start_after_en = clock();

		//Wait for FPGA Edge detection done
		bPass = TRUE;
		DWORD Status;

		do {
			bPass = PCIE_Read32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_BUTTON_ADDR, &Status);
			printf("%d\n", Status);
			if (!bPass) printf("Read failed\n");
		} while (Status % 2 == 1);
		//printf("%d - FPGA Done\n", Status);

		// enb = 0;
		// Mask = 0;
		// bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR,(DWORD)Mask);
		// printf("Disable flag is sent\n");

		//Read the edge image
		Mat output(480, 640, CV_8UC1);
		clock_t start_read_img = clock();
		readImgFromDmaMem1(hPCIE, &output);
		//printf("Edge image is read\n");
		//imwrite("edge_" + filename, output);

		//---------------
		clock_t start_before_gau_atom = clock();
		Size size(5, 5);
		GaussianBlur(grayImg, grayImg, size, 0);

		//----------Edge dectect by Canny------
		int lowThreshold = 50;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;
		//imshow("Input", input);
		Mat edgeImg;
		Canny(grayImg, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);
		clock_t end_after_canny_atom = clock();
		//----------------------------

		clock_t end = clock();
		double elapsed_beEn_enProg = double(end - start_before_en) / CLOCKS_PER_SEC;
		double elapsed_beEn_enEn = double(end_write_en - start_before_en) / CLOCKS_PER_SEC;
		double elapsed_beDMA_enDMA = double(end_write_img - start_write_img) / CLOCKS_PER_SEC;
		double elapsed_enEn_enProg = double(end - start_after_en) / CLOCKS_PER_SEC;
		double elapsed_beGau_enCan_atom = double(end_after_canny_atom - start_before_gau_atom) / CLOCKS_PER_SEC;
		double elapsed_beRead_beGau = double(start_before_gau_atom - start_read_img) / CLOCKS_PER_SEC;
		double elapsed_FPGA_canny = double(start_read_img - start_after_en) / CLOCKS_PER_SEC;

		cout << "elapsed_beEn_enProg: " << elapsed_beEn_enProg << endl;
		cout << "elapsed_beEn_enEn: " << elapsed_beEn_enEn << endl;
		cout << "elapsed_beDMA_enDMA: " << elapsed_beDMA_enDMA << endl;
		cout << "elapsed_enEn_enProg: " << elapsed_enEn_enProg << endl;
		cout << "elapsed_beGau_enCan_atom: " << elapsed_beGau_enCan_atom << endl;
		cout << "elapsed_beRead_beGau: " << elapsed_beRead_beGau << endl;
		cout << "elapsed_FPGA_canny: " << elapsed_FPGA_canny << endl;

		imshow("Output", output);
	}
	PCIE_Close(hPCIE);
	PCIE_Unload(lib_handle);

	waitKey(0);
	return 0;
}
