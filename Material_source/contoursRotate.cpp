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

String checkMidEdge(Mat input) {
	String result;

	//cvtColor(input, input, CV_BGR2GRAY);
	//threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("input", input);

	int black = 0, white = 0, damm = 0;
	for (int l = 0; l < 6; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			if ((l == 0) || (l == 5));
			else if (c == 0 || c == 2) {
				for (int i = 0 + c*input.cols / 3; i < (c + 1)*input.cols / 3; i++) {
					for (int j = 0 + l*input.rows / 6; j < (l + 1)*input.rows / 6; j++) {
						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
						else white++;
					}
				}
				damm++;
				if (black > white / 2) {
					result += "1";
				}
				else {
					result += "0";
				}
			}
		}
	}

	long int toNumber = stoi(result);
	int check = 0;
	switch (toNumber)
	{
	case 11010010:
		result = "2";
		break;
	case 11010111:
		result = "3";
		break;
	case 10110111:
		result = "5";
		break;
	case 10111111:
		result = "6";
		break;
	case 11111101:
		result = "9";
		break;
	case 11111111:
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

String detectChar(Mat input) {
	String detectedChar, step1_Char;
	step1_Char = checkCorner(input);

	if (step1_Char == "1110") {
		detectedChar = "1";
	}
	else if (step1_Char == "0010") {
		detectedChar = "4";
	}
	else if (step1_Char == "1101") {
		detectedChar = "7";
	}
	else {
		detectedChar = checkMidEdge(input);
	}

	cout << endl << "This is number " << detectedChar << endl;
	return detectedChar;
}

int main() {
	//string name;
	//cin >> name;
	//name = "../../bs/" + name + ".jpg";
	//Mat input = imread(name);
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	cap.set(CV_CAP_PROP_FPS, 15);

	Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		
		Mat input = frame;
		resize(input, input, Size(640, 480));

		Mat img = input.clone();

		Mat grayImg;
		cvtColor(input, grayImg, CV_BGR2GRAY);

		//imshow("gray", grayImg);

		/// Apply Histogram Equalization
		equalizeHist(grayImg, grayImg);

		//imshow("equalized", grayImg);

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

		if (n < 1) {
			cvClearMemStorage(contoursMem);
			cvReleaseMemStorage(&contoursMem);
			continue;
		}
		
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

		vector<Mat> plates, edgePlates, diEdgePlates;
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
				diEdgePlates.push_back(diEdgeImg(sr));

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
			//Hough line
			CvMemStorage* linesMem = cvCreateMemStorage(0);
			IplImage plate = edgePlates[i];
			CvSeq* linesA = cvHoughLines2(&plate, linesMem, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 70, 30, 50);

			if (linesA->total < 1) {
				cvClearMemStorage(linesMem);
				cvReleaseMemStorage(&linesMem);
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
				
				//line(plates[i], lP[0], lP[1], Scalar(255,0,0), 1, CV_AA);
				//line(plates[i], lP[0], lP[0], Scalar(255, 255, 0), 3, CV_AA);
				//line(plates[i], lP[1], lP[1], Scalar(0, 0, 255), 3, CV_AA);
			}
			avgA = avgA / avgS;
			double angle = -avgA * 180 / CV_PI;

			cvClearMemStorage(linesMem);
			cvReleaseMemStorage(&linesMem);

			//Rotate plate, edge
			Point2f center((plates[i].cols - 1) / 2.0, (plates[i].rows - 1) / 2.0);
			Mat rot = getRotationMatrix2D(center, angle, 1.0);
			Rect bbox = RotatedRect(cv::Point2f(), plates[i].size(), angle).boundingRect();
			rot.at<double>(0, 2) += bbox.width / 2.0 - plates[i].cols / 2.0;
			rot.at<double>(1, 2) += bbox.height / 2.0 - plates[i].rows / 2.0;
			warpAffine(plates[i], plates[i], rot, bbox.size());
			warpAffine(diEdgePlates[i], diEdgePlates[i], rot, bbox.size());

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

			Mat plates2 = plates[i].clone();
			int j = 0;
			for (int j = 0; j < totalPoly2; j++) {
				Rect r = boundRect2[j];
				double rate = r.width / (double)r.height;
				if (rate > 1.3 && rate < 1.4 && r.height > 30) {
					rectangle(plates[i], r, Scalar(0, 0, 255), 1, 8, 0);
					plates2 = plates[i](r);
				}
			}

			//imshow(to_string(i), plates[i]);

			cvClearMemStorage(contoursMem2);
			cvClearMemStorage(polyMem2);

			cvReleaseMemStorage(&contoursMem2);
			cvReleaseMemStorage(&polyMem2);

			int tempIndex = 0;
			findCharacter(plates2, charImgArray, tempIndex, topLeftArray);
			if (tempIndex > 7 && tempIndex < 10) {
				index = tempIndex;
				imshow("Plate", plates2);
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
				bitwise_not(charImgArray[im], charImgArray[im]);
				String regChar = detectChar(charImgArray[im]);
				cout << regChar;
			}

			im--;
		}

		cvClearMemStorage(contoursMem);
		cvClearMemStorage(polyMem);

		cvReleaseMemStorage(&contoursMem);
		cvReleaseMemStorage(&polyMem);

		imshow("contours", img);

		//waitKey(0);

		if (waitKey(30) >= 0) break;
	}
	
	return 0;
}