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

	//----------Find intersections---------------
	vector<Vec2i> iPoints;
	for (int i = 0; i < linesV.size(); i++) {
		for (int j = 0; j < linesH.size(); j++) {
			int x = (int)xSolve(linesV[i][0], -1, -linesV[i][1], linesH[j][0], -1, -linesH[j][1]);
			int y = (int)ySolve(linesV[i][0], -1, -linesV[i][1], linesH[j][0], -1, -linesH[j][1]);
			if (x < 10 || y < 10) continue;
			if (x > input.cols || y > input.rows) continue;
			if (((int)lineImgH.at<cv::Vec3b>(y, x)[2] == 255) && ((int)lineImgV.at<cv::Vec3b>(y, x)[2] == 255)) {
				iPoints.push_back(Vec2i(x, y));
				line(lineImg, Point(x, y), Point(x, y), Scalar(0, 255, 0), 5, CV_AA);
			}
		}
	}
	
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
					threshold(plateGrayImg, plateThresholdImg, 100, 255, 0);
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

	dilate(plateBinImg, plateBinImg, element, Point(-1,-1), 2);

	vector < vector<Point> > contours;
	findContours(plateContourImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	Mat charImg[9];
	Point2i topLeft[9];
	int index = 0;

	/// Approximate contours to polygons and get bounding rects
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );

	for( int i = 0; i < contours.size(); i++ ) {
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	}

	/// Draw bonding rects
	// GaussianBlur(plateImg, plateBlurImg, Size(3, 3), 0);
	// Canny(plateBlurImg, plateEdgeImg, 50, 300, 3);

	for(int i = 0; i< contours.size(); i++) {
		Rect r = boundRect[i];
		if ((r.width / (double)r.height > 0.2f && r.width / (double)r.height < 0.6f)
			&& (r.height < 100 && r.height > 70)){
			topLeft[index] = r.tl();
			rectangle( plateImg, r, Scalar(0, 0, 255), 1, 8, 0 );
			charImg[index] = plateBinImg(r);
			index++;
		}
	}

	//Sort the charImg array
	int top = 0;
	for (int i = 0; i < index; i++){
		if (topLeft[i].y < 50){
			top = i;
			break;
		}
	}

	for (int i = 0; i < top - 1; i++){
		for (int j = i; j < top; j++){
			if (topLeft[i].x < topLeft[j].x){
				Point2i tempPoint = topLeft[i];
				topLeft[i] = topLeft[j];
				topLeft[j] = tempPoint;

				Mat tempImg = charImg[i];
				charImg[i] = charImg[j];
				charImg[j] = tempImg;
			}
		}
	}

	for (int i = top; i < index - 1; i++){
		for (int j = i; j < index; j++){
			if (topLeft[i].x < topLeft[j].x){
				Point2i tempPoint = topLeft[i];
				topLeft[i] = topLeft[j];
				topLeft[j] = tempPoint;

				Mat tempImg = charImg[i];
				charImg[i] = charImg[j];
				charImg[j] = tempImg;
			}
		}
	}

	cout << "So ky tu: " << index << endl;

	int im = index - 1;
	while (im >= 0) {
	// int im = 8;
		//cout << charImg[im].cols << " : " << charImg[im].rows << endl;
		// Scalar color = Scalar(255,255,255);
		// line(charImg[im], Point(charImg[im].cols/3, 0), Point(charImg[im].cols/3, charImg[im].rows), color, 1, CV_AA);
		// line(charImg[im], Point(charImg[im].cols/3*2, 0), Point(charImg[im].cols/3*2, charImg[im].rows), color, 1, CV_AA);
		// line(charImg[im], Point(0, charImg[im].rows/6), Point(charImg[im].cols, charImg[im].rows/6), color, 1, CV_AA);
		// line(charImg[im], Point(0, charImg[im].rows/6*2), Point(charImg[im].cols, charImg[im].rows/6*2), color, 1, CV_AA);
		// line(charImg[im], Point(0, charImg[im].rows/6*3), Point(charImg[im].cols, charImg[im].rows/6*3), color, 1, CV_AA);
		// line(charImg[im], Point(0, charImg[im].rows/6*4), Point(charImg[im].cols, charImg[im].rows/6*4), color, 1, CV_AA);
		// line(charImg[im], Point(0, charImg[im].rows/6*5), Point(charImg[im].cols, charImg[im].rows/6*5), color, 1, CV_AA);
		imshow("num " + char(im), charImg[im]);

		int charSquare[3][6] = {{0}};
		int charBit = 0;
		char regChar = '-';
		
		int black = 0, white = 0;
		for (int l = 0; l < 6; l++) {
			for (int c = 0; c < 3; c++){
				white = 0;
				black = 0;
				for (int i = 0 + c*charImg[im].cols/3; i < (c + 1)*charImg[im].cols/3; i++) {
					for (int j = 0 + l*charImg[im].rows/6; j < (l + 1)*charImg[im].rows/6; j++) {
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
					charBit += pow(2,l*3 + c);
				}
			}
		}

		//dec2bin(charBit);

		if (charImg[im].cols < 30){
			regChar = '1';
		}
		else {
			regChar = getChar(charBit);
		}

		//cout << charBit << endl;
		cout << regChar;

		im--;
	}
	cout << endl;
	

	//imshow("Line", lineImg);
	//imshow("Edge", plateEdgeImg);
	//imshow("Morpho", morpho);
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

int dec2bin(int num){
	int total = 0;
	int count = 0;
	cout << endl;
	while(num > 0)
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

char getChar(int code){
	//define minimun code of number
	int zero	= 88938,
		two 	= 234786,
		three 	= 84386, //84258,
		four	= 160338,
		five	= 84175, //116943,
		six		= 88778,
		seven	= 38023,
		eight	= 88746, //86186, //43050,
		nine	= 85354;

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
