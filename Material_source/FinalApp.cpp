//#include "stdafx.h"
#pragma comment(lib, "MSCOREE.lib")
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "PCIE.h"
#include <ctime>
#include <string.h>

using namespace std;
using namespace cv;

#define DEMO_PCIE_USER_BAR			PCIE_BAR0
#define DEMO_PCIE_IO_LED_ADDR		0x00
#define DEMO_PCIE_IO_WORD_ADDR		0x10
#define DEMO_PCIE_IO_BUTTON_ADDR	0x20
#define DEMO_PCIE_FIFO_WRITE_ADDR	0x40
#define DEMO_PCIE_FIFO_STATUS_ADDR	0x60
#define DEMO_PCIE_FIFO_READ_ADDR	0x80
#define DEMO_PCIE_MEM_ADDR			0x80000
#define DEMO_PCIE_MEM_ADDR_1		0x180000


#define MEM_SIZE			(300*1024) //128KB
#define FIFO_SIZE			(16*1024) // 2KBx8
#define PI 3.14159265

int dec2bin(int num);
char getChar(int code);
void findCharacter(Mat input, Mat *charImg, int &index, Point2i *topLeft);
void sortPlate(Mat *charImg, Point2i *topLeft, int index);

/* //char detectNumber(Mat * charImg, int im) {
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
//	if (charImg[im].cols < 20) {
//		regChar = '1';
//	}
//	else {
//		//cout << charBit << endl;
//		regChar = getChar(charBit);
//	}
//
//	return regChar;
//} */

String checkCorner(Mat input);
String checkMidEdgeNumber(Mat input);
String detectNumber(Mat input);
int checkPF(Mat input);
String checkRightEdge(Mat input);
String checkTopEdge(Mat input, String type);
String checkMidEdgeRight(Mat input);
String checkInner(Mat input);
String checkMN(Mat input);
int checkBD(Mat input);
long returnChar(String s);
String checkMidEdge(Mat input);
String detectChar(Mat input);
BOOL writeImgToDmaMem0(PCIE_HANDLE hPCIe, Mat input);
BOOL readImgFromDmaMem1(PCIE_HANDLE hPCIe, Mat *output);
char PAT_GEN(Mat nIndex, int x, int y);

struct _showValue {
	long value;
	long num = 0;
};
int _sizeBuf = 8;
vector< vector<struct _showValue> > _showBuffer(_sizeBuf);
vector< long > _charBuffer(9, 0);
int _showTime = 0;

int main() {
	///----------Khoi tao PCIE 
	void *lib_handle;
	PCIE_HANDLE hPCIE;
	BOOL bQuit = FALSE;
	int nSel;

	printf("== Terasic: PCIe Demo Program ==\r\n");

	lib_handle = PCIE_Load();
	if (!lib_handle) {
		printf("PCIE_Load failed!\r\n");
		return 0;
	}

	hPCIE = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
	if (!hPCIE) {
		printf("PCIE_Open failed\r\n");
		return 0;
	}
	///----------Bat dau chay camera-----------------------------

	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	cap.set(CV_CAP_PROP_FPS, 15);

	Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
	for (;;)
	{

		printf("\n\n\n== CE: LPD Demo Program ==\r\n");

		/// ----------G?i tin hi?u chua chuy?n ?nh sang canny---------
		BOOL bPass;
		int Mask = 0;
		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
		printf("Disable flag is sent\n");

		Mat frame;
		cap >> frame; // get a new frame from camera

		Mat input = frame;
		if (!input.data) continue;
		resize(input, input, Size(640, 480));

		Mat img = input.clone();

		Mat grayImg;
		cvtColor(input, grayImg, CV_BGR2GRAY);

		//imshow("gray", grayImg);

		/// Apply Histogram Equalization
		equalizeHist(grayImg, grayImg);

		///---------Ghi anh xam xuong FPGA
		clock_t a = clock();
		writeImgToDmaMem0(hPCIE, grayImg);
		printf("The gray image is sent\n");
		clock_t b = clock();

		double c = double(b - a) / CLOCKS_PER_SEC;
		cout << "hrfgjhgfsddghui hjksdhuidddddddddddffffffffffff: " << c << endl;
		//int lowThreshold = 70;
		//int const max_lowThreshold = 100;
		//int ratio = 3;
		//int kernel_size = 3;
		////imshow("Input", input);
		//Mat edgeImg;
		//Canny(grayImg, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);

		///---------Gui tin hieu enable
		Mask = 4;
		printf("\nMask = %d\n", Mask);

		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
		if (bPass) printf("Enable flag is sent\n");

		///---------Wait for FPGA Edge detection done
		bPass = TRUE;
		DWORD Status;

		do {
			bPass = PCIE_Read32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_BUTTON_ADDR, &Status);
			printf("\n%d\n", Status);
			if (!bPass) printf("Read failed\n");
		} while (Status % 2 == 1);
		printf("\n%d - FPGA Done\n", Status);

		///---------Read the edge image
		Mat output(480, 640, CV_8UC1);
		readImgFromDmaMem1(hPCIE, &output);
		printf("\nEdge image is read\n");

		///---------Transfer canny to threshold
		/*imshow("From FPGA", output);*/
		Mat edgeImg = output;
		threshold(edgeImg, edgeImg, 100, 255, CV_THRESH_BINARY);
		imshow("From FPGA", output);

		//Mat binImg;
		//threshold(grayImg, binImg, 200, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

		Mat diEdgeImg;
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
		dilate(edgeImg, diEdgeImg, element);
		dilate(diEdgeImg, diEdgeImg, element);

		//Mat element = getStructuringElement(MORPH_RECT, Size(17, 3));
		//morphologyEx(edgeImg, edgeImg, CV_MOP_CLOSE, element);

		cout << "Pass bitwise_not\n";

		imshow("bin", diEdgeImg);

		CvMemStorage *contoursMem = cvCreateMemStorage(0);
		CvSeq *contours = 0;
		Mat diEdgeImg2 = diEdgeImg.clone();
		bitwise_not(diEdgeImg2, diEdgeImg2);
		IplImage edgeIpl = diEdgeImg2;

		int n = cvFindContours(&edgeIpl, contoursMem, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

		if (n < 1) {
			cvClearMemStorage(contoursMem);
			cvReleaseMemStorage(&contoursMem);
			continue;
		}
		cout << "Pass find contour\n";

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

		cout << "Pass approx\n";

		/// Draw bonding rects
		// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
		// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

		vector<Mat> plates, edgePlates, diEdgePlates;
		int j = 0;
		for (int i = 0; i < totalPoly; i++) {
			Rect r = boundRect[i];
			CvBox2D b = boundBox[i];
			double rate = r.width / (double)r.height;
			if (rate > 1.0 && rate < 2 && r.height > 50) {
				rectangle(img, r, Scalar(0, 0, 255), 1, 8, 0);
				if (r.tl().x - 20 < 0 || r.tl().y - 20 < 0 || r.br().x + 20 > input.cols || r.br().y + 20 > input.rows) continue;
				Rect sr = Rect(Point(r.tl().x - 20, r.tl().y - 20), Point(r.br().x + 20, r.br().y + 20));
				plates.push_back(input(sr));
				edgePlates.push_back(edgeImg(sr));
				diEdgePlates.push_back(diEdgeImg(sr));

				if (b.size.height < 100) continue;
				RotatedRect rRect = RotatedRect(b);
				Point2f vertices[4];
				rRect.points(vertices);
				for (int i = 0; i < 4; i++)
					line(img, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 3);

				j++;
			}
		}
		imshow("Contour temp", img);

		cout << "Pass add contour\n";

		int index = 0;
		Mat charImgArray[9];
		Point2i topLeftArray[9];
		for (int i = 0; i < edgePlates.size(); i++) {
			//Hough line
			CvMemStorage* linesMem = cvCreateMemStorage(0);
			IplImage plate = edgePlates[i];
			CvSeq* linesA = cvHoughLines2(&plate, linesMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 70, 30, 50);

			if (linesA->total < 1) {
				cvClearMemStorage(linesMem);
				cvReleaseMemStorage(&linesMem);
				cout << "CHUA tim dc Line nao\n";
				continue;
			}

			float avgA = 0;
			int avgS = 0;
			for (size_t j = 0; j < linesA->total; j++) {
				Point* lP = (Point*)cvGetSeqElem(linesA, j);
				float w = abs(lP[0].x - lP[1].x);
				float h = lP[0].y - lP[1].y;
				float tan = h / w;
				if (tan > 1 || tan < -1) continue;
				avgA += atan(tan);
				avgS++;
			}
			if (avgS) avgA = avgA / avgS;
			else continue; ///Tam qua vong moi
			double angle = -(double)(avgA * 180 / PI);

			cvClearMemStorage(linesMem);
			cvReleaseMemStorage(&linesMem);

			//Rotate plate, edge
			Point2f center((plates[i].cols) / 2.0F, (plates[i].rows) / 2.0F);

			Mat rot = getRotationMatrix2D(center, angle, 1.0);

			Point2f cen = Point2f();
			cout << "-------A N G L E--------" << angle << endl;
			Rect bbox = RotatedRect(cen, (cv::Size2f)plates[i].size(), (double)angle).boundingRect2f();

			rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
			//cout << "Debug truoc khi xoay4" << endl;
			rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

			Mat dst;
			warpAffine(plates[i], plates[i], rot, bbox.size());
			warpAffine(diEdgePlates[i], diEdgePlates[i], rot, bbox.size());
			bitwise_not(diEdgePlates[i], diEdgePlates[i]);

			//imshow("edge to fin contour for cropping", diEdgePlates[i]);

			//Find contours again to crop the plate
			CvMemStorage *contoursMem2 = cvCreateMemStorage(0);
			CvSeq *contours2 = 0;
			IplImage plate2 = diEdgePlates[i];

			int n = cvFindContours(&plate2, contoursMem2, &contours2, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

			if (n < 1) {
				cvClearMemStorage(contoursMem2);
				cvReleaseMemStorage(&contoursMem2);
				continue;
			}
			///// Approximate contours to polygons and get bounding rects
			vector<vector<Point> > contours_poly2(n);
			vector<CvRect> boundRect2(n);

			CvMemStorage *polyMem2 = cvCreateMemStorage(0);
			CvSeq *poly2;

			poly2 = cvApproxPoly(contours2, sizeof(CvContour), polyMem2, CV_POLY_APPROX_DP, 3, 1);
			int totalPoly2 = 0;
			for (CvSeq *j = poly2; j != NULL; j = j->h_next) {
				boundRect2[totalPoly2] = cvBoundingRect((CvContour*)j, 1);
				totalPoly2++;
			}

			/// Draw bonding rects
			// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
			// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

			Mat cropedPlates = plates[i].clone();
			int j = 0;
			for (int j = 0; j < totalPoly2; j++) {
				Rect r = boundRect2[j];
				double rate = r.width / (double)r.height;
				//rectangle(plates[i], r, Scalar(0, 0, 255), 1, 8, 0);
				if (rate > 1.3 && r.height > plates[i].rows / 3 && r.height < (plates[i].rows - 5)) {
					rectangle(plates[i], r, Scalar(0, 0, 255), 1, 8, 0);
					cropedPlates = plates[i](r);
				}
			}
			//imshow("conto on plate", plates[i]);
			imshow("cropped plate", cropedPlates);
			//imshow(to_string(i), plates[i]);

			cvClearMemStorage(contoursMem2);
			cvClearMemStorage(polyMem2);

			cvReleaseMemStorage(&contoursMem2);
			cvReleaseMemStorage(&polyMem2);

			int tempIndex = 0;
			findCharacter(cropedPlates, charImgArray, tempIndex, topLeftArray);
			cout << "So ky tu temp: " << tempIndex << endl;
			if (tempIndex > 7 && tempIndex < 10) {
				index = tempIndex;
				break;
			}
		}

		cout << "So ky tu: " << index << endl;

		if (index < 8) {
			cvClearMemStorage(contoursMem);
			cvClearMemStorage(polyMem);

			cvReleaseMemStorage(&contoursMem);
			cvReleaseMemStorage(&polyMem);

			if (waitKey(30) >= 0) break;
			continue;
		}

		cout << "Pass catch plate\n";

		sortPlate(charImgArray, topLeftArray, index);

		cout << "Bang so xe: ";
		int im = index - 1;
		int run = 0;
		long result[9];
		while (im >= 0) {
			if ((im == 5 && index == 8) || (im == 6 && index == 9)) {
				bitwise_not(charImgArray[im], charImgArray[im]);
				string regChar = detectChar(charImgArray[im]);
				imshow("helo", charImgArray[im]);
				long returnNum = returnChar(regChar);
				result[run] = (returnNum == 99) ? 127 : returnNum;
				cout << " - " << regChar << " ";
			}
			else if ((im == 4 && index == 8) || (im == 5 && index == 9)) {
				bitwise_not(charImgArray[im], charImgArray[im]);
				string regChar = detectChar(charImgArray[im]);
				long returnNum = returnChar(regChar);
				if (returnNum == 99) {
					regChar = detectNumber(charImgArray[im]);
				}
				int number = (int)regChar[0] - 48;

				if (number >= 0 && number < 10 && regChar.length() == 1) {
					result[run] = (long)number;
				}
				else result[run] = 127;
				cout << regChar << "\nxuong dong   ";
			}
			else {
				bitwise_not(charImgArray[im], charImgArray[im]);
				string regChar = detectNumber(charImgArray[im]);
				int number = (int)regChar[0] - 48;

				if (number >= 0 && number < 10 && regChar.length() == 1) {
					result[run] = (long)number;
				}
				else result[run] = 127;
				
				if (im == 0) {
					cout << "SO CUOI CUNG NE " << regChar << " ";
					im--;
					run++;
					continue;
				}
				cout << regChar << " ";
			}

			if (im == 2) cout << ".";

			im--;
			run++;
		}

		if (run < 8) continue;

		//For greater accurate
		//add 
		if (_showTime < 3) {
			_sizeBuf = 0;
			for (int i = 0; i < run; i++) {
				int flagSame = 0;

				if (i >= _showBuffer.size()) {
					struct _showValue temp;
					temp.value = result[i];
					temp.num++;
					vector<struct _showValue> newValue;
					newValue.push_back(temp);
					_showBuffer.push_back(newValue);
					cout << "ASDASDASDASDASDASDASDASDASDASDASDASDASDASD --- " << run << " " << i << " " << _showBuffer.size() << endl;
				}

				for (int j = 0; j < _showBuffer[i].size(); j++) {
					if (result[i] == _showBuffer[i][j].value) {
						_showBuffer[i][j].num++;
						flagSame = 1;
						break;
					}
				}
				if (flagSame == 0) {
					struct _showValue temp;
					temp.value = result[i];
					temp.num++;
					_showBuffer[i].push_back(temp);

				}
			}
		}
		//show
		else if (_showTime == 3 || _showTime == 0) {
			//int sizeBuff = (run < _showBuffer.size()) ? run : _showBuffer.size();
			int sizeBuff = _showBuffer.size();
			cout << "SIZE BUFFER FUCK YOU BITCH " << sizeBuff << "\n";
			int max = 0, value = 0;
			vector<int> _showStream;
			for (int i = 0; i < sizeBuff; i++) {
				max = 0;
				for (int j = 0; j < _showBuffer[i].size(); j++) {
					if (max < _showBuffer[i][j].num && (_showBuffer[i][j].value != 127)) {
						max = _showBuffer[i][j].num;
						value = _showBuffer[i][j].value;
					}
				}
				if (value == 0 || value == 127) value = 127;
				else _charBuffer[i] = value;
				_showStream.push_back(value);
			}

			double powEx = 15.0;
			for (int i = 0; i < _showStream.size(); i++) {
				int Word1, charSent;
				charSent = ((result[i] != 127) || (_charBuffer[i] == 0 && result[i] == 127)) ? result[i] : _charBuffer[i];
				Word1 = charSent + (int)pow(2.0, powEx);
				if (i == run) { Word1 = 127 + (int)pow(2.0, powEx); }
				powEx--;

				printf("\nMask = %d %d %d %d %d\n", Word1, i, run, charSent, _charBuffer[i]);
				bool bbbPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_WORD_ADDR, (DWORD)Word1);
				if (bbbPass) printf("Enable word is sent\n");
			}
			if (_showStream.size() == 8) {
				int Word1;
				Word1 = 127 + (int)pow(2.0, powEx);
				bool bbbPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_WORD_ADDR, (DWORD)Word1);
				if (bbbPass) printf("Enable word is sent\n");
			}

			//resize lại mảng bên trong 
			for (int i = 0; i < _showBuffer.size(); i++) {
				_showBuffer[i].resize(0);
			}
			_showBuffer.resize(8);
			_showTime = 0;
		}

		_showTime++;
		//--------------------

		cvClearMemStorage(contoursMem);
		cvClearMemStorage(polyMem);

		cvReleaseMemStorage(&contoursMem);
		cvReleaseMemStorage(&polyMem);

		try {
			imshow("contours", img);
		}
		catch (Exception e) {
			cout << "What the fuck\n";
		}
		//waitKey(0);

		if (waitKey(30) >= 0) break;
	}

	PCIE_Close(hPCIE);
	PCIE_Unload(lib_handle);

	return 0;
}

int checkPF(Mat input) {
	Mat l = input(Rect(0, input.rows / 6.0, input.cols / 4.0, input.rows / 6.0 * 5));
	/*cvtColor(l, l, CV_BGR2GRAY);
	threshold(l, l, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);*/

	int black = 0, white = 0;
	for (int i = 0; i < l.cols; i++) {
		for (int j = 0; j < l.rows; j++) {
			if ((int)l.at<uchar>(Point(i, j)) == 0) black++;
			else white++;
		}
	}

	return (white >= black * 3.0) ? 0 : 1;
}

string checkRightEdge(Mat input) {
	String result;

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

	if (checkPF(input) == 0) return result;
	if (outputCor == "11" || outputCor == "10") {
		result = 'P';
	}
	else if (outputCor == "00" || outputCor == "01") {
		result = 'F';
	}

	return result;
}

string checkTopEdge(Mat input, string type) {
	String result;

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

string checkMidEdgeRight(Mat input) {
	String result;

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

string checkInner(Mat input) {
	String result;

	int black = 0, white = 0, damm = 0;
	for (int l = 0; l < 7; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			if (c != 0 && c != 2 && l != 0 && l != 6) {
				for (int i = 0 + c * input.cols / 3.0; i < (c + 1)*input.cols / 3.0; i++) {
					for (int j = 0 + l * input.rows / 7.0; j < (l + 1)*input.rows / 7.0; j++) {
						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
						else white++;
					}
				}
				damm++;
				if ((double)black > (double)white / 4.0) {
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

string checkMN(Mat input) {
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

int checkBD(Mat input) {
	Mat tl = input(Rect(0, 0, input.cols / 4.0, input.rows / 8.0));
	Mat bl = input(Rect(0, input.rows / 8.0 * 7, input.cols / 4.0, input.rows / 8.0));

	float black_tl = 0, white_tl = 0, black_bl = 0, white_bl = 0;
	for (int i = 0; i < tl.cols; i++) {
		for (int j = 0; j < tl.rows; j++) {
			if ((int)tl.at<uchar>(Point(i, j)) == 0) black_tl++;
			else white_tl++;
			if ((int)bl.at<uchar>(Point(i, j)) == 0) black_bl++;
			else white_bl++;
		}
	}

	if ((white_tl + white_bl) >= (black_bl + black_tl)) return 0;
	else return 1;
}

String checkMidEdge(Mat input) {
	String result;

	Mat t = input(Rect(0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
	Mat m = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
	//imshow("M", m);

	/*cvtColor(input, input, CV_BGR2GRAY);
	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	*/
	int black = 0, white = 0, damm = 0;
	for (int l = 0; l < 6; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			if ((c == 0 && l == 0) || (c == 2 && l == 0) || (c == 0 && l == 5) || (c == 2 && l == 5));
			else if (c == 0 || c == 2 || l == 0 || l == 5) {
				for (int i = 0 + c * input.cols / 3; i < (c + 1)*input.cols / 3; i++) {
					for (int j = 0 + l * input.rows / 6; j < (l + 1)*input.rows / 6; j++) {
						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
						else white++;
					}
				}
				damm++;
				if ((double)black > (double)white / 4.0) {
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
	case 1111111110:
		result = "R";
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
			if (checkBD(input) == 0) return result;
			int test = stoi(temp);
			if (test > 0) result = "B";
			//if (temp == "01000" || temp == "00100" || temp == "00010") result = "B";
			else result = "D";
		}
		else if (toNumber == 111111110) {
			int test = stoi(temp);
			if (test > 0) result = "H";
			//if (temp == "01000" || temp == "00100" || temp == "00010") result = "H";
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

String detectNumber(Mat input) {
	String detectedChar, step1_Char;
	step1_Char = checkCorner(input);

	if (step1_Char == "1110" || input.cols < 10) {
		detectedChar = "1";
	}
	else if (step1_Char == "0010" || step1_Char == "0110") {
		detectedChar = "4";
	}
	else if (step1_Char == "1101") {
		detectedChar = "7";
	}
	else {
		detectedChar = checkMidEdgeNumber(input);
	}

	//cout << endl << "This is number " << detectedChar << endl;
	return detectedChar;
}

String checkMidEdgeNumber(Mat input) {
	String result;

	/*cvtColor(input, input, CV_BGR2GRAY);
	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("input", input);*/

	int black = 0, white = 0, damm = 0;
	for (int l = 0; l < 8; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			if ((l == 0) || (l == 7));
			else if (c == 0 || c == 2) {
				for (double i = c * input.cols / 3.0; i < (c + 1)*input.cols / 3.0; i++) {
					for (double j = l * input.rows / 8.0; j < (l + 1)*input.rows / 8.0; j++) {
						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
						else white++;
					}
				}
				damm++;
				if ((double)black >(double)white / 2) {
					result += "1";
				}
				else {
					result += "0";
				}
			}
		}
	}

	cout << "result: " << result << endl;
	long long int toNumber = stoll(result);
	int check = 0;
	switch (toNumber)
	{
	case 110101001010:
		result = "2";
		break;
	case  10100001010:
		result = "2";
		break;
	case  10100001011:
		result = "2";
	case 110101010111:
		result = "3";
		break;
	case 101011011111:
		result = "5";
		break;
	case 111011111111:
		result = "6";
		break;
	case 111111110111:
		result = "9";
		break;
	case 111111111111:
		result = "0 or 8";
		check = 1;
		break;
	}

	if (check == 1) {
		Mat mid = input(Rect(input.cols / 3.0, input.rows / 6.0 * 2.0, input.cols / 3.0, input.rows / 6.0 * 2.0));
		imshow("cac", mid);
		for (int i = 0; i < mid.cols; i++) {
			for (int j = 0; j < mid.rows; j++) {
				if ((int)mid.at<uchar>(Point(i, j)) == 0) black++;
				else white++;
			}
		}

		if (black > white / 1.2) result = "8";
		else result = "0";
	}

	return result;
}

String checkCorner(Mat input) {
	Mat tl = input(Rect(0, 0, input.cols / 3.0, input.rows / 6.0));
	Mat tr = input(Rect(input.cols / 3.0 * 2, 0, input.cols / 3.0, input.rows / 6.0));
	Mat bl = input(Rect(0, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
	Mat br = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
	//cvtColor(tl, tl, CV_BGR2GRAY);
	//threshold(tl, tl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//cvtColor(tr, tr, CV_BGR2GRAY);
	//threshold(tr, tr, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//cvtColor(bl, bl, CV_BGR2GRAY);
	//threshold(bl, bl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//cvtColor(br, br, CV_BGR2GRAY);
	//threshold(br, br, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

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

void sortPlate(Mat *charImg, Point2i *topLeft, int index) {
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

void findCharacter(Mat input, Mat *charImg, int &index, Point2i *topLeft) {
	//cout << "Debug: findCharacter()" << endl;
	//----------Extract characters------------
	Mat plateImg = input;
	//resize(plateImg, plateImg, Size(640, 480)); //270;200
	resize(plateImg, plateImg, Size(270, 200));
	//imshow("Plate to fine char", plateImg);

	Mat plateBlurImg, plateEdgeImg, plateBinImg, plateGrayImg, plateContourImg;

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));

	cvtColor(plateImg, plateGrayImg, CV_BGR2GRAY);

	//int lowThreshold = 70;
	//int const max_lowThreshold = 100;
	//int ratio = 3;
	//int kernel_size = 3;
	//Mat edgeImg;
	//Canny(plateGrayImg, plateBinImg, lowThreshold, lowThreshold*ratio, kernel_size);

	threshold(plateGrayImg, plateBinImg, 90, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	//adaptiveThreshold(plateGrayImg, plateBinImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 1);

	//dilate(plateBinImg, plateBinImg, element, Point(-1,-1), 2);
	erode(plateBinImg, plateBinImg, element);

	plateContourImg = plateBinImg.clone();
	imshow("Bin", plateBinImg);

	dilate(plateBinImg, plateBinImg, element, Point(-1, -1), 2);

	//vector < vector<Point> > contours;
	//findContours(plateEdgeImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	CvMemStorage *contoursMem = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	IplImage plate = plateContourImg;

	int n = cvFindContours(&plate, contoursMem, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	//cout << "Debug tim contour" << endl;
	//Mat charImgArray[9];
	//Mat * charImg = charImgArray;
	//Point2i topLeft[9];
	//int index = 0;

	if (n < 1) {
		cvClearMemStorage(contoursMem);
		cvReleaseMemStorage(&contoursMem);
		return;
	}

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
	cout << "Debug poly" << endl;
	/// Draw bonding rects
	// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
	// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

	for (int i = 0; i < totalPoly; i++) {
		if (index > 8) break;
		Rect rr = boundRect[i];
		Rect r;
		if (rr.tl().x > 2 && rr.tl().y > 2 && rr.br().x < 268 && rr.br().y < 198)
			r = Rect(Point(rr.tl().x - 2, rr.tl().y - 2),Point(rr.br().x + 2,rr.br().y +2));
		else r = rr;
		double rate = r.width / (double)r.height;
		if ((rate > 0.2f && rate < 0.6f) && (r.height < 100 && r.height > 50)) {
			topLeft[index] = r.tl();
			rectangle(plateImg, r, Scalar(0, 0, 255), 1, 8, 0);

			charImg[index] = plateBinImg(r);
			index++;
		}
	}
	//imshow("plateBin", plateBinImg);
	//if (index > 7 && index < 10) 
	imshow("plate", plateImg);
	//waitKey(0);

	cvClearMemStorage(contoursMem);
	cvClearMemStorage(polyMem);

	cvReleaseMemStorage(&contoursMem);
	cvReleaseMemStorage(&polyMem);

	return;
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
	//pWrite = (char*)input.data;

	// write test pattern
	if (bPass) {
		bPass = PCIE_DmaWrite(hPCIe, LocalAddr, pWrite, nTestSize);
		if (!bPass)
			sprintf(szError, "DMA Memory:PCIE_DmaWrite failed\r\n");
	}
	printf("Write to RAM 0 successfully!\n");


	//free resource
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

long returnChar(string s) {
	char c = s[0];
	int i = int(c);
	// do stuff
	if (s.length() == 1) {
		return i - 55;
	}
	else return 99;
}


