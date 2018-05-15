////#include "stdafx.h"
////#include <iostream>
////#include <stdio.h>
////#include <stdlib.h>
////#include <opencv2/opencv.hpp>
////#include <opencv2/core/core.hpp>
////#include <opencv2/highgui/highgui.hpp>
////#include <opencv2/imgproc/imgproc.hpp>
////
////using namespace std;
////using namespace cv;
////
////String checkCorner(Mat input) {
////	/*Scalar color = Scalar(255, 255, 255);
////	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
////	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
////
////	//imshow("Plate", input);
////	Mat tl = input(Rect(0, 0, input.cols / 3.0, input.rows / 6.0));
////	Mat tr = input(Rect(input.cols / 3.0 * 2, 0, input.cols / 3.0, input.rows / 6.0));
////	Mat bl = input(Rect(0, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
////	Mat br = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0 * 5, input.cols / 3.0, input.rows / 6.0));
////	cvtColor(tl, tl, CV_BGR2GRAY);
////	threshold(tl, tl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////	cvtColor(tr, tr, CV_BGR2GRAY);
////	threshold(tr, tr, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////	cvtColor(bl, bl, CV_BGR2GRAY);
////	threshold(bl, bl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////	cvtColor(br, br, CV_BGR2GRAY);
////	threshold(br, br, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////
////	int white = 0;
////	int black = 0;
////	String outputCor = "";
////
////	for (int cor = 0; cor < 4; cor++) {
////		white = 0; black = 0;
////		Mat corDetect;
////		switch (cor) {
////		case 0:
////			corDetect = tl.clone();
////			break;
////		case 1:
////			corDetect = tr.clone();
////			break;
////		case 2:
////			corDetect = br.clone();
////			break;
////		case 3:
////			corDetect = bl.clone();
////			break;
////		}
////		for (int i = 0; i < corDetect.cols; i++) {
////			for (int j = 0; j < corDetect.rows; j++) {
////				if ((int)corDetect.at<uchar>(Point(i, j)) == 0) {
////					black++;
////				}
////				else white++;
////			}
////		}
////		if (black > white / 3) {
////			outputCor += "1";
////		}
////
////		else outputCor += "0";
////	}
////	//cout << endl << outputCor << endl;
////	return outputCor;
////}
////
////int checkPF(Mat input) {
////	Mat l = input(Rect(0, input.rows / 6.0, input.cols / 4.0, input.rows / 6.0 * 5));
////	cvtColor(l, l, CV_BGR2GRAY);
////	threshold(l, l, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////
////	int black = 0, white = 0;
////	for (int i = 0; i < l.cols; i++) {
////		for (int j = 0; j < l.rows; j++) {
////			if ((int)l.at<uchar>(Point(i, j)) == 0) black++;
////			else white++;
////		}
////	}
////
////	return (white >= black * 3.0) ? 0 : 1;
////}
////
////String checkRightEdge(Mat input) {
////	String result;
////	/*Scalar color = Scalar(255, 255, 255);
////	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
////	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
////
////	Mat r2 = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
////	Mat r3 = input(Rect(input.cols / 3.0 * 2, input.rows / 6.0 * 2, input.cols / 3.0, input.rows / 6.0));
////	cvtColor(r2, r2, CV_BGR2GRAY);
////	threshold(r2, r2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////	cvtColor(r3, r3, CV_BGR2GRAY);
////	threshold(r3, r3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////
////	int white = 0;
////	int black = 0;
////	String outputCor = "";
////	for (int r = 0; r < 2; r++) {
////		Mat rDetect;
////		black = 0; white = 0;
////		rDetect = (r == 0) ? r2 : r3;
////		for (int i = 0; i < rDetect.cols; i++) {
////			for (int j = 0; j < rDetect.rows; j++) {
////				if ((int)rDetect.at<uchar>(Point(i, j)) == 0) {
////					black++;
////				}
////				else white++;
////			}
////		}
////		if (black < white / 4) {
////			outputCor += "0";
////		}
////		else outputCor += "1";
////	}
////
////	if (checkPF(input) == 0) return result;
////	if (outputCor == "11" || outputCor == "10") {
////		result = 'P';
////	}
////	else if (outputCor == "00" || outputCor == "01") {
////		result = 'F';
////	}
////
////	return result;
////}
////
////String checkTopEdge(Mat input, String type) {
////	String result;
////	/*Scalar color = Scalar(255, 255, 255);
////	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
////	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
////
////	Mat t = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
////	cvtColor(t, t, CV_BGR2GRAY);
////	threshold(t, t, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////
////	int white = 0;
////	int black = 0;
////	char outputCor;
////	for (int i = 0; i < t.cols; i++) {
////		for (int j = 0; j < t.rows; j++) {
////			if ((int)t.at<uchar>(Point(i, j)) == 0) {
////				black++;
////			}
////			else white++;
////		}
////	}
////	if (black > white / 1.2) {
////		outputCor = '1';
////	}
////	else outputCor = '0';
////
////	if (outputCor == '1') {
////		result = (type == "1011") ? "Q" : "T";
////	}
////	else if (outputCor = '0') {
////		result = (type == "1011") ? "L" : "none";
////	}
////
////	return result;
////}
////
////String checkMidEdgeRight(Mat input) {
////	String result;
////	/*Scalar color = Scalar(255, 255, 255);
////	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
////	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
////
////	Mat m2 = input(Rect(input.cols / 3.0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
////	Mat m3 = input(Rect(input.cols / 3.0, input.rows / 6.0 * 2, input.cols / 3.0, input.rows / 6.0));
////	cvtColor(m2, m2, CV_BGR2GRAY);
////	threshold(m2, m2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////	cvtColor(m3, m3, CV_BGR2GRAY);
////	threshold(m3, m3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////
////	int white = 0;
////	int black = 0;
////	String outputCor = "";
////	for (int m = 0; m < 2; m++) {
////		Mat mDetect;
////		black = 0; white = 0;
////		mDetect = (m == 0) ? m2 : m3;
////		for (int i = 0; i < mDetect.cols; i++) {
////			for (int j = 0; j < mDetect.rows; j++) {
////				if ((int)mDetect.at<uchar>(Point(i, j)) == 0) {
////					black++;
////				}
////				else white++;
////			}
////		}
////		if (black > white) {
////			outputCor += "1";
////		}
////		else outputCor += "0";
////	}
////
////	if (outputCor == "11" /*|| outputCor == "01"*/) {
////		result = 'Y';
////	}
////	else {
////		result = 'V';
////	}
////
////	return result;
////}
////
////String checkInner(Mat input) {
////	String result;
////	/*Scalar color = Scalar(255, 255, 255);
////	line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
////	line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
////	line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
////
////	int black = 0, white = 0, damm = 0;
////	for (int l = 0; l < 6; l++) {
////		for (int c = 0; c < 3; c++) {
////			white = 0;
////			black = 0;
////			if (c != 0 && c != 2 && l != 0 && l != 5) {
////				for (int i = 0 + c*input.cols / 3.0; i < (c + 1)*input.cols / 3.0; i++) {
////					for (int j = 0 + l*input.rows / 6.0; j < (l + 1)*input.rows / 6.0; j++) {
////						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
////						else white++;
////					}
////				}
////				damm++;
////				if (black > white / 3) {
////					result += "1";
////				}
////				else {
////					result += "0";
////				}
////			}
////		}
////	}
////	//cout << result;
////	return result;
////}
////
////String checkMN(Mat input) {
////	String result;
////
////	Mat m1 = input(Rect(input.cols / 5.0 * 2, input.rows / 10.0 * 2, input.cols / 5.0, input.rows / 10.0 * 2));
////	//cvtColor(m1, m1, CV_BGR2GRAY);
////	//threshold(m1, m1, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////
////	int black = 0, white = 0, damm = 0;
////	for (int i = 0; i < m1.cols; i++) {
////		for (int j = 0; j < m1.rows; j++) {
////			if ((int)m1.at<uchar>(Point(i, j)) == 0) black++;
////			else white++;
////		}
////	}
////
////	if ((black / white) > 2) {
////		result = "M";
////	}
////	else result = "N";
////	//cout << result;
////	//imshow("Plate", m1);
////	return result;
////}
////
////int checkBD(Mat input) {
////	Mat tl = input(Rect(0, 0, input.cols / 4.0, input.rows / 8.0));
////	Mat bl = input(Rect(0, input.rows / 8.0 * 7, input.cols / 4.0, input.rows / 8.0));
////
////	float black_tl = 0, white_tl = 0, black_bl = 0, white_bl = 0;
////	for (int i = 0; i < tl.cols; i++) {
////		for (int j = 0; j < tl.rows; j++) {
////			if ((int)tl.at<uchar>(Point(i, j)) == 0) black_tl++;
////			else white_tl++;
////			if ((int)bl.at<uchar>(Point(i, j)) == 0) black_bl++;
////			else white_bl++;
////		}
////	}
////
////	if ((white_tl + white_bl) >= (black_bl + black_tl)*1.1) return 0;
////	else return 1;
////}
////
////String checkMidEdge(Mat input) {
////	String result;
////	//Scalar color = Scalar(255, 255, 255);
////	///*line(input, Point(input.cols / 3, 0), Point(input.cols / 3, input.rows), color, 1, CV_AA);
////	//line(input, Point(input.cols / 3 * 2, 0), Point(input.cols / 3 * 2, input.rows), color, 1, CV_AA);
////	//line(input, Point(0, input.rows / 6), Point(input.cols, input.rows / 6), color, 1, CV_AA);
////	//line(input, Point(0, input.rows / 6 * 2), Point(input.cols, input.rows / 6 * 2), color, 1, CV_AA);
////	//line(input, Point(0, input.rows / 6 * 3), Point(input.cols, input.rows / 6 * 3), color, 1, CV_AA);
////	//line(input, Point(0, input.rows / 6 * 4), Point(input.cols, input.rows / 6 * 4), color, 1, CV_AA);
////	//line(input, Point(0, input.rows / 6 * 5), Point(input.cols, input.rows / 6 * 5), color, 1, CV_AA);*/
////
////	Mat t = input(Rect(0, input.rows / 6.0, input.cols / 3.0, input.rows / 6.0));
////	Mat m = input(Rect(input.cols / 3.0, 0, input.cols / 3.0, input.rows / 6.0));
////	//imshow("M", m);
////
////	cvtColor(input, input, CV_BGR2GRAY);
////	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
////
////	int black = 0, white = 0, damm = 0;
////	for (int l = 0; l < 6; l++) {
////		for (int c = 0; c < 3; c++) {
////			white = 0;
////			black = 0;
////			if ((c == 0 && l == 0) || (c == 2 && l == 0) || (c == 0 && l == 5) || (c == 2 && l == 5));
////			else if (c == 0 || c == 2 || l == 0 || l == 5) {
////				for (int i = 0 + c*input.cols / 3; i < (c + 1)*input.cols / 3; i++) {
////					for (int j = 0 + l*input.rows / 6; j < (l + 1)*input.rows / 6; j++) {
////						if ((int)input.at<uchar>(Point(i, j)) == 0) black++;
////						else white++;
////					}
////				}
////				damm++;
////				if (black > white / 3) {
////					//cout << "Mieng den " << l << " " << c << endl;
////					result += "1";
////				}
////				else {
////					result += "0";
////					//cout << "Mieng trang " << l << " " << c << endl;
////				}
////
////				/*if (c == 1 && l == 0) {
////				cout << "Black " << black << endl;
////				cout << "White " << white << endl;
////				}*/
////			}
////		}
////	}
////
////	long int toNumber = stoi(result);
////	//cout << toNumber;
////	int check = 0;
////	switch (toNumber)
////	{
////	case 1111010111:
////		result = "C";
////		break;
////	case 1101010101:
////		result = "E";
////		break;
////	case 1111011111:
////		result = "G";
////		break;
////	case 1101010111:
////		result = "K";
////		break;
////	case 1111001111:
////		result = "S";
////		break;
////	case 111111111:
////		result = "U";
////		break;
////	case 1010000101:
////		result = "Z";
////		break;
////	case 1110000111:
////		result = "X";
////		break;
////	case 1111111110:
////		result = "R";
////		break;
////	case 1111111111:
////		check = 1;
////		break;
////	case 111111110:
////		check = 1;
////		break;
////	default:
////		break;
////	}
////	//cout << toNumber;
////
////	if (check == 1) {
////		String temp = checkInner(input);
////		if (toNumber == 1111111111) {
////			if (checkBD(input) == 0) return result;
////			if (temp == "0110" || temp == "0100" || temp == "0010") result = "B";
////			else result = "D";
////		}
////		else if (toNumber == 111111110) {
////			if (temp == "0110" || temp == "0100" || temp == "0010") result = "H";
////			else result = checkMN(input);
////		}
////	}
////
////	return result;
////}
////
////String detectChar(Mat input) {
////	String detectedChar, step1_Char;
////	step1_Char = checkCorner(input);
////
////	if (step1_Char == "0011") {
////		detectedChar = "A";
////	}
////	else if (step1_Char == "0111") {
////		detectedChar = "J";
////	}
////	else if (step1_Char == "1101") {
////		detectedChar = checkRightEdge(input);
////	}
////	else if (step1_Char == "1011" || step1_Char == "1100") {
////		//call step2_1011-1100
////		//cout << "Vao day coi " << step1_Char;
////		String temp = checkTopEdge(input, step1_Char);
////		detectedChar = (temp == "none") ? checkMidEdgeRight(input) : temp;
////	}
////	else if (step1_Char == "1111") {
////		//call step2_1111
////		//cout << "No vao day";
////		detectedChar = checkMidEdge(input);
////	}
////
////	cout << endl << "This is character " << detectedChar << endl;
////	return detectedChar;
////}
////
////int main(int argc, char** argv) {
////	Mat A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
////	A = imread("../InputImage/allCharacterNumber/TestAC_T.jpg", 1);
////	B = imread("../InputImage/allCharacterNumber/TestBC_T.jpg", 1);
////	C = imread("../InputImage/allCharacterNumber/TestCC_T.jpg", 1);
////	D = imread("../InputImage/allCharacterNumber/TestDC_T.jpg", 1);
////	E = imread("../InputImage/allCharacterNumber/TestEC_T.jpg", 1);
////	F = imread("../InputImage/allCharacterNumber/TestFC_T.jpg", 1);
////	G = imread("../InputImage/allCharacterNumber/TestGC_T.jpg", 1);
////	H = imread("../InputImage/allCharacterNumber/TestHC_T.jpg", 1);
////	I = imread("../InputImage/allCharacterNumber/TestIC_T.jpg", 1);
////	J = imread("../InputImage/allCharacterNumber/TestJC_T.jpg", 1);
////	K = imread("../InputImage/allCharacterNumber/TestKC_T.jpg", 1);
////	L = imread("../InputImage/allCharacterNumber/TestLC_T.jpg", 1);
////	M = imread("../InputImage/allCharacterNumber/TestMC_T.jpg", 1);
////	N = imread("../InputImage/allCharacterNumber/TestNC_T.jpg", 1);
////	O = imread("../InputImage/allCharacterNumber/TestOC_T.jpg", 1);
////	P = imread("../InputImage/allCharacterNumber/TestPC_T.jpg", 1);
////	Q = imread("../InputImage/allCharacterNumber/TestQC_T.jpg", 1);
////	R = imread("../InputImage/allCharacterNumber/TestRC_T.jpg", 1);
////	S = imread("../InputImage/allCharacterNumber/TestSC_T.jpg", 1);
////	T = imread("../InputImage/allCharacterNumber/TestTC_T.jpg", 1);
////	U = imread("../InputImage/allCharacterNumber/TestUC_T.jpg", 1);
////	V = imread("../InputImage/allCharacterNumber/TestVC_T.jpg", 1);
////	W = imread("../InputImage/allCharacterNumber/TestWC_T.jpg", 1);
////	X = imread("../InputImage/allCharacterNumber/TestXC_T.jpg", 1);
////	Y = imread("../InputImage/allCharacterNumber/TestYC_T.jpg", 1);
////	Z = imread("../InputImage/allCharacterNumber/TestZC_T.jpg", 1);
////
////	/*cout << " A " << checkCorner(A) << endl;
////	cout << " B " << checkCorner(B) << endl;
////	cout << " C " << checkCorner(C) << endl;
////	cout << " D " << checkCorner(D) << endl;
////	cout << " E " << checkCorner(E) << endl;
////	cout << " F " << checkCorner(F) << endl;
////	cout << " G " << checkCorner(G) << endl;
////	cout << " H " << checkCorner(H) << endl;
////	cout << " I " << checkCorner(I) << endl;
////	cout << " J " << checkCorner(J) << endl;
////	cout << " K " << checkCorner(K) << endl;
////	cout << " L " << checkCorner(L) << endl;
////	cout << " M " << checkCorner(M) << endl;
////	cout << " N " << checkCorner(N) << endl;
////	cout << " O " << checkCorner(O) << endl;
////	cout << " P " << checkCorner(P) << endl;
////	cout << " Q " << checkCorner(Q) << endl;
////	cout << " R " << checkCorner(R) << endl;
////	cout << " S " << checkCorner(S) << endl;
////	cout << " T " << checkCorner(T) << endl;
////	cout << " U " << checkCorner(U) << endl;
////	cout << " V " << checkCorner(V) << endl;
////	cout << " W " << checkCorner(W) << endl;
////	cout << " X " << checkCorner(X) << endl;
////	cout << " Y " << checkCorner(Y) << endl;
////	cout << " Z " << checkCorner(Z) << endl;*/
////	std::cout << "Demo character regconition" << endl;
////	/*checkMidEdge(B);
////	checkMidEdge(C);
////	checkMidEdge(D);
////	checkMidEdge(E);
////	checkMidEdge(G);
////	checkMidEdge(H);
////	checkMidEdge(K);
////	checkMidEdge(M);
////	checkMidEdge(N);
////	checkMidEdge(S);
////	checkMidEdge(U);
////	checkMidEdge(Z);
////	checkMidEdge(X);*/
////
////	//detectChar(Q);
////	Mat Character[26] = { A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z };
////	char id = 'A';
////	for (int i = 0; i < 26; i++) {
////		cout << "Character identity: " << id;
////		detectChar(Character[i]);
////		id++;
////	}
////	//checkTopEdge(Y, "1100");
////	//checkInner(B);
////	//checkMN(N);
////	//checkMN(M);
////
////	waitKey(0);
////	system("pause");
////	return 1;
////}
////
////
////
//
//#include "stdafx.h"
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <math.h>
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
//
//int main(int argc, char** argv) {
//	Mat input;
//	input = imread("../InputImage/input8.jpg", 1);
//	if (!input.data) {
//		return 1;
//	}
//
//	resize(input, input, Size(640, 480));
//
//	//----------Noise filter---------------
//	Size size(3, 3);
//	GaussianBlur(input, input, size, 0);
//
//	//Convert to gray image
//	Mat grayImg, grayFilterImg;
//	cvtColor(input, grayImg, CV_BGR2GRAY);
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
//
//	//----------Hough transform-------------
//	vector<Vec4i> linesA, linesB;
//	HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 60, 10);
//	HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 60, 10);
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
//	Mat lineImg(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	Mat lineImgH(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	Mat lineImgV(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	vector<Vec3d> linesH, linesV;
//	int delta = 90;
//	for (size_t i = 0; i < linesA.size(); i++) {
//		Vec4i l = linesA[i];
//		float w = abs(l[1] - l[3]);
//		float h = abs(l[0] - l[2]);
//		float tan = w / h;
//		if (tan < 1) {
//			line(lineImg, Point(l[0] - delta, l[1]), Point(l[2] + delta, l[3]), Scalar(0, 0, 255), 5, CV_AA);
//			line(lineImgH, Point(l[0] - delta, l[1]), Point(l[2] + delta, l[3]), Scalar(0, 0, 255), 5, CV_AA);
//			l[0] = l[0] - 50;
//			l[2] = l[2] + 50;
//			/*double a = xSolve(l[0], 1, l[1], l[2], 1, l[3]);
//			double b = ySolve(l[0], 1, l[1], l[2], 1, l[3]);*/
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			if ((constABC[0] != 0) | (constABC[1] != 0)) linesH.push_back(constABC);
//		}
//		if (tan > 1) {
//			int y1, y2;
//			if (l[1] > l[3]) {
//				y1 = l[1] + delta; l[1] = y1;
//				y2 = l[3] - delta; l[3] = y2;
//			}
//			else {
//				y1 = l[1] - delta; l[1] = y1;
//				y2 = l[3] + delta; l[3] = y2;
//			}
//			line(lineImg, Point(l[0], y1), Point(l[2], y2), Scalar(0, 0, 255), 5, CV_AA);
//			line(lineImgV, Point(l[0], y1), Point(l[2], y2), Scalar(0, 0, 255), 5, CV_AA);
//
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			if ((constABC[0] != 0) | (constABC[1] != 0)) linesV.push_back(constABC);
//		}
//	}
//	
//	for (size_t i = 0; i < linesB.size(); i++) {
//		Vec4i l = linesB[i];
//		float w = abs(l[1] - l[3]);
//		float h = abs(l[0] - l[2]);
//		float tan = w / h;
//		if (tan < 1) {
//			line(lineImg, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), 5, CV_AA);
//			line(lineImgH, Point(l[0] - 50, l[1]), Point(l[2] + 50, l[3]), Scalar(0, 0, 255), 5, CV_AA);
//			l[0] = l[0] - 50;
//			l[2] = l[2] + 50;
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			if ((constABC[0] != 0) | (constABC[1] != 0)) linesH.push_back(constABC);
//		}
//		if (tan > 1) {
//			int y1, y2;
//			if (l[1] > l[3]) {
//				y1 = l[1] + delta; l[1] = y1;
//				y2 = l[3] - delta; l[3] = y2;
//			}
//			else {
//				y1 = l[1] - delta; l[1] = y1;
//				y2 = l[3] + delta; l[3] = y2;
//			}
//			line(lineImg, Point(l[0], y1), Point(l[2], y2), Scalar(0, 0, 255), 5, CV_AA);
//			line(lineImgV, Point(l[0], y1), Point(l[2], y2), Scalar(0, 0, 255), 5, CV_AA);
//
//			Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
//			if ((constABC[0] != 0) | (constABC[1] != 0)) linesV.push_back(constABC);
//		}
//	}
//	
//	//----------Find intersections---------------
//	vector<Vec2i> iPoints;
//	Mat lineImgH_90(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	Mat lineImgV_90(input.size(), CV_8UC3, Scalar(0, 0, 0));
//	for (int i = 0; i < linesV.size(); i++) {
//		for (int j = 0; j < linesH.size(); j++) {
//			Vec2d interXY;
//			interXY = interPoint(linesV[i][0], linesV[i][1], linesV[i][2], linesH[j][0], linesH[j][1], linesH[j][2]);
//			int x = (int)interXY[0];
//			int y = (int)interXY[1];
//			if (x < 10 || y < 10) continue;
//			if (x > input.cols || y > input.rows) continue;
//			if (((int)lineImgH.at<cv::Vec3b>(y, x)[2] == 255) && ((int)lineImgV.at<cv::Vec3b>(y, x)[2] == 255)) {
//				iPoints.push_back(Vec2i(x, y));
//				line(lineImg, Point(x, y), Point(x, y), Scalar(0, 255, 0), 5, CV_AA);
//			}
//
//			double son = abs(linesV[i][0]*linesH[j][0]*1.0 + linesV[i][1]*linesH[j][1]*1.0);
//			double mother = sqrt(pow(linesV[i][0] * 1.0, 2.0) + pow(linesV[i][1] * 1.0, 2.0))
//							* sqrt(pow(linesH[j][0] * 1.0, 2.0) + pow(linesH[j][0] * 1.0, 2.0));
//
//			double cos = (double)(son / mother);
//
//			if (cos < 0.087) {
//				cout << cos << endl;
//				line(lineImg, Point(x, y), Point(x, y), Scalar(255, 0, 0), 10, CV_AA);
//			}
//		}
//	}
//
//	imshow("Vertical ", lineImgV);
//	imshow("Horizontal ", lineImgH);
//	imshow("V + H", lineImg);
//
//	//----------Crop the plate-------------------
//	/*
//	//Mat plateImg;
//	//if (!iPoints.empty()) {
//	//	for (int i = 0; i < iPoints.size() - 1; i++) {
//	//		for (int j = 1; j < iPoints.size(); j++) {
//	//			double w = abs(iPoints[i][0] - iPoints[j][0]);
//	//			double h = abs(iPoints[i][1] - iPoints[j][1]);
//	//			double l = w / h;
//	//			//if (w < 200 || h < 200) continue;
//	//			if (l > 1.3 && l < 1.4) {
//	//				Mat plateTempImg, plateGrayImg, plateThresholdImg;
//	//				int black = 0, white = 0;
//
//	//				plateTempImg = input(Rect(Point2i(int(iPoints[i][0]), int(iPoints[i][1])), Point2i(int(iPoints[j][0]), int(iPoints[j][1]))));
//	//				cvtColor(plateTempImg, plateGrayImg, CV_BGR2GRAY);
//	//				threshold(plateGrayImg, plateThresholdImg, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//	//				for (int m = 0; m < plateThresholdImg.cols; m++) {
//	//					for (int n = 0; n < plateThresholdImg.rows; n++) {
//	//						if ((int)plateThresholdImg.at<uchar>(Point(m, n)) == 0) black++;
//	//						else white++;
//	//					}
//	//				}
//
//	//				if (black > 0 && white / black >= 2 && plateTempImg.rows > 20)
//	//					plateImg = plateTempImg;
//	//			}
//	//		}
//	//	}
//	//}
//
//	//try {
//	//	imshow("Plate before", plateImg);
//	//}
//	//catch (Exception e) {
//	//	cout << "\nCannot find the plate " << endl;
//	//
//	//	waitKey(0);
//	//	return 0;
//	//}
//	*/
//
//	//imshow("Plate", plateImg);
//
//	waitKey(0);
//	return 0;
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
//	if (dy == 0 || dy == 0) return -1;
//	return dy / d;
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

#define PI 3.14159265

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

Vec2d interPoint(double a1, double b1, double c1, double a2, double b2, double c2);
Vec3d linearEquation(double x1, double y1, double x2, double y2);

int main() {
	string name;
	cin >> name;
	name = "../InputImage/" + name + ".jpg";
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

	vector<Mat> plates;
	int j = 0;
	for (int i = 0; i < totalPoly; i++) {
		Rect r = boundRect[i];
		double rate = r.width / (double)r.height;
		if ((rate > 1.0f) && r.height > 50) {
			rectangle(input, r, Scalar(0, 0, 255), 1, 8, 0);
			plates.push_back(input(r));

			CvBox2D b = boundBox[i];
			if (b.size.height < 100) continue;
			RotatedRect rRect = RotatedRect(b);
			Point2f vertices[4];
			rRect.points(vertices);
			for (int i = 0; i < 4; i++)
				line(input, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 1);

			Mat rMatrix = getRotationMatrix2D(b.center, b.angle, 1);
			Point2f center((plates[j].cols - 1) / 2.0, (plates[j].rows - 1) / 2.0);
			Mat rot = cv::getRotationMatrix2D(center, b.angle, 1.0);
			// determine bounding rectangle, center not relevant
			Rect bbox = RotatedRect(cv::Point2f(), plates[j].size(), b.angle).boundingRect();
			// adjust transformation matrix
			rot.at<double>(0, 2) += bbox.width / 2.0 - plates[j].cols / 2.0;
			rot.at<double>(1, 2) += bbox.height / 2.0 - plates[j].rows / 2.0;
			warpAffine(plates[j], plates[j], rot, bbox.size());

			j++;
		}
	}

	int index = 0;
	Mat charImgArray[9];
	Point2i topLeftArray[9];
	Mat plate_detected = plates[0];
	for (int i = 0; i < plates.size(); i++) {
		if(i==0)imshow(to_string(i), plates[i]);
		int tempIndex = 0;
		findCharacter(plates[i], charImgArray, tempIndex, topLeftArray);

		if (tempIndex > 7 && tempIndex < 10) {
			index = tempIndex;
			cout << "co ne";
			//imshow("Plate", plates[i]);
			/*plate_detected = plates[i];*/
			break;
		}
	}

	//-----------TEst-----------
	for (int i = 0; i < 1; i++) {
		// Canny
		int lowThreshold = 70;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;
		Mat edgeImg;
		Canny(plate_detected, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);

		// Hough
		vector<Vec4i> linesA, linesB;
		HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 60, 10);
		HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 60, 10);

		Mat Lines_StepA, Lines_StepB;
		cvtColor(edgeImg, Lines_StepA, COLOR_GRAY2BGR);
		cvtColor(edgeImg, Lines_StepB, COLOR_GRAY2BGR);
		cout << endl << " Endline " << linesA.size();
		for (size_t i = 0; i < linesA.size(); i++)
		{
			Vec4i l = linesA[i];
			line(Lines_StepA, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 255), 1, CV_AA);
		}
		for (size_t i = 0; i < linesB.size(); i++)
		{
			Vec4i l = linesB[i];
			line(Lines_StepB, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 255), 1, CV_AA);
		}
		imshow("Line_StepA", Lines_StepA);
		imshow("Line_StepB", Lines_StepB);

		// Find lines
		Mat lineImg(plate_detected.size(), CV_8UC3, Scalar(0, 0, 0));
		Mat lineImgH(plate_detected.size(), CV_8UC3, Scalar(0, 0, 0));
		Mat lineImgV(plate_detected.size(), CV_8UC3, Scalar(0, 0, 0));
		vector<Vec3d> linesH, linesV;
		int delta = 10;
		for (size_t i = 0; i < linesB.size(); i++) {
			Vec4i l = linesB[i];
			float w = abs(l[1] - l[3]);
			float h = abs(l[0] - l[2]);
			float tan = w / h;
			if (tan < 1) {
				line(lineImg, Point(l[0] - delta, l[1]), Point(l[2] + delta, l[3]), Scalar(0, 0, 255), 1, CV_AA);
				line(lineImgH, Point(l[0] - delta, l[1]), Point(l[2] + delta, l[3]), Scalar(0, 0, 255), 1, CV_AA);
				l[0] = l[0] - delta;
				l[2] = l[2] + delta;
				Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
				if ((constABC[0] != 0) | (constABC[1] != 0)) linesH.push_back(constABC);
				cout << "\nCanh Horizon: a=" << constABC[0] << " b=" << constABC[1] << " c=" << constABC[2];
			}
			if (tan > 1) {
				int y1, y2;
				if (l[1] > l[3]) {
					y1 = l[1] + delta; l[1] = y1;
					y2 = l[3] - delta; l[3] = y2;
				}
				else {
					y1 = l[1] - delta; l[1] = y1;
					y2 = l[3] + delta; l[3] = y2;
				}
				line(lineImg, Point(l[0], y1), Point(l[2], y2), Scalar(0, 0, 255), 1, CV_AA);
				line(lineImgV, Point(l[0], y1), Point(l[2], y2), Scalar(0, 0, 255), 1, CV_AA);
		
				Vec3d constABC = linearEquation(l[0], l[1], l[2], l[3]);
				if ((constABC[0] != 0) | (constABC[1] != 0)) linesV.push_back(constABC);
				cout << "\nCanh Vertical: a=" << constABC[0] << " b=" << constABC[1] << " c=" << constABC[2];
			}
		}

		// Find intersection
		vector<Vec2i> iPoints;
		Mat lineImgH_90(plate_detected.size(), CV_8UC3, Scalar(0, 0, 0));
		Mat lineImgV_90(plate_detected.size(), CV_8UC3, Scalar(0, 0, 0));
		Vec3d horiLine, horiLine2;
		Point lowest(0, 0), highest(0, plate_detected.rows);
		for (int i = 0; i < linesV.size(); i++) {
			for (int j = 0; j < linesH.size(); j++) {
				Vec2d interXY;
				interXY = interPoint(linesV[i][0], linesV[i][1], linesV[i][2], linesH[j][0], linesH[j][1], linesH[j][2]);
				int x = (int)interXY[0];
				int y = (int)interXY[1];
				if (x < 0 || y < 0) continue;
				if (x > input.cols || y > input.rows) continue;
				if (((int)lineImgH.at<cv::Vec3b>(y, x)[2] == 255) && ((int)lineImgV.at<cv::Vec3b>(y, x)[2] == 255)) {
					iPoints.push_back(Vec2i(x, y));
					line(lineImg, Point(x, y), Point(x, y), Scalar(0, 255, 0), 5, CV_AA);
				}
		
				double son = abs(linesV[i][0]*linesH[j][0]*1.0 + linesV[i][1]*linesH[j][1]*1.0);
				double mother = sqrt(pow(linesV[i][0] * 1.0, 2.0) + pow(linesV[i][1] * 1.0, 2.0))
								* sqrt(pow(linesH[j][0] * 1.0, 2.0) + pow(linesH[j][0] * 1.0, 2.0));
		
				double cos = (double)(son / mother);

				cout << "\nGiao diem: " << " y: " << y << " x: " << x;
				if (cos > 1) {
					cout << "\nGoc lol: " << cos << " y: " << y << " x: " << x << " a: " << linesH[j][0] 
						 << " b: " << linesH[j][1] << " c: " << linesH[j][2] << endl;
					line(lineImg, Point(x, y), Point(x, y), Scalar(0, 250, 0), 10, CV_AA);
				}
		
				if (cos < 0.08) {
					cout << "\nGoc vuong: " << cos << " y: " << y << " x: " << x << endl;
					line(lineImg, Point(x, y), Point(x, y), Scalar(255, 0, 0), 7, CV_AA);
					if (y > lowest.y) {
						//cout << y;
						horiLine = Vec3d(linesH[j][0], linesH[j][1], linesH[j][2]);
						lowest = Point(x, y);
					}
					if (y < highest.y) {
						horiLine2 = Vec3d(linesH[j][0], linesH[j][1], linesH[j][2]);
						highest = Point(x, y);
					}
				}
			}
		}

		line(lineImg, lowest, lowest, Scalar(255, 0, 255), 5, CV_AA);
		
		namedWindow("Vertical");
		imshow("Vertical", lineImgV);
		moveWindow("Vertical", 120, 50);

		namedWindow("Horizontal");
		imshow("Horizontal", lineImgH);
		moveWindow("Horizontal", 120, 270);

		namedWindow("V + H");
		imshow("V + H", lineImg);
		moveWindow("V + H", 500, 50);

		// Xet HoriLine
		double alpha = (double)(horiLine[0] / horiLine[1]);
		double rotate = atan(alpha) * 180 / PI;
		int plate_sign = (horiLine[0] > 0.0) ? 1 : 0; // 1:+, 0:-
		double Rotating_degree = (plate_sign == 0) ? (270.0 + rotate + 5) : (360.0 - rotate - 5);
		cout << "\nRotating degree: "<< rotate << endl;

		Point2f src_center(plate_detected.cols / 2.0F, plate_detected.rows / 2.0F);
		Mat rot_mat = getRotationMatrix2D(src_center, Rotating_degree, 1.0);
		Mat dst;
		warpAffine(plate_detected, dst, rot_mat, plate_detected.size());
		namedWindow("Rotated");
		imshow("Rotated", dst);
		moveWindow("Rotated", 250, 50);
	}

	//-----------TEst-----------

	cvClearMemStorage(contoursMem);
	cvClearMemStorage(polyMem);

	cvReleaseMemStorage(&contoursMem);
	cvReleaseMemStorage(&polyMem);

	imshow("contours", input);

	waitKey(0);

	//	if (waitKey(30) >= 0) break;
	//}

	return 0;
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
