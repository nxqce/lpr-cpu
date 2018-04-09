#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

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
	cvtColor(tl, tl, CV_BGR2GRAY);
	threshold(tl, tl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(tr, tr, CV_BGR2GRAY);
	threshold(tr, tr, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(bl, bl, CV_BGR2GRAY);
	threshold(bl, bl, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(br, br, CV_BGR2GRAY);
	threshold(br, br, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

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
	cvtColor(r2, r2, CV_BGR2GRAY);
	threshold(r2, r2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(r3, r3, CV_BGR2GRAY);
	threshold(r3, r3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

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
	cvtColor(t, t, CV_BGR2GRAY);
	threshold(t, t, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

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
	cvtColor(m2, m2, CV_BGR2GRAY);
	threshold(m2, m2, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvtColor(m3, m3, CV_BGR2GRAY);
	threshold(m3, m3, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

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

	cvtColor(input, input, CV_BGR2GRAY);
	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

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

	cout << endl << "This is character " << detectedChar << endl;
	return detectedChar;
}

//int main(int argc, char** argv) {
//	Mat A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
//	A = imread("../InputImage/allCharacterNumber/TestAC_T.jpg", 1);
//	B = imread("../InputImage/allCharacterNumber/TestBC_T.jpg", 1);
//	C = imread("../InputImage/allCharacterNumber/TestCC_T.jpg", 1);
//	D = imread("../InputImage/allCharacterNumber/TestDC_T.jpg", 1);
//	E = imread("../InputImage/allCharacterNumber/TestEC_T.jpg", 1);
//	F = imread("../InputImage/allCharacterNumber/TestFC_T.jpg", 1);
//	G = imread("../InputImage/allCharacterNumber/TestGC_T.jpg", 1);
//	H = imread("../InputImage/allCharacterNumber/TestHC_T.jpg", 1);
//	I = imread("../InputImage/allCharacterNumber/TestIC_T.jpg", 1);
//	J = imread("../InputImage/allCharacterNumber/TestJC_T.jpg", 1);
//	K = imread("../InputImage/allCharacterNumber/TestKC_T.jpg", 1);
//	L = imread("../InputImage/allCharacterNumber/TestLC_T.jpg", 1);
//	M = imread("../InputImage/allCharacterNumber/TestMC_T.jpg", 1);
//	N = imread("../InputImage/allCharacterNumber/TestNC_T.jpg", 1);
//	O = imread("../InputImage/allCharacterNumber/TestOC_T.jpg", 1);
//	P = imread("../InputImage/allCharacterNumber/TestPC_T.jpg", 1);
//	Q = imread("../InputImage/allCharacterNumber/TestQC_T.jpg", 1);
//	R = imread("../InputImage/allCharacterNumber/TestRC_T.jpg", 1);
//	S = imread("../InputImage/allCharacterNumber/TestSC_T.jpg", 1);
//	T = imread("../InputImage/allCharacterNumber/TestTC_T.jpg", 1);
//	U = imread("../InputImage/allCharacterNumber/TestUC_T.jpg", 1);
//	V = imread("../InputImage/allCharacterNumber/TestVC_T.jpg", 1);
//	W = imread("../InputImage/allCharacterNumber/TestWC_T.jpg", 1);
//	X = imread("../InputImage/allCharacterNumber/TestXC_T.jpg", 1);
//	Y = imread("../InputImage/allCharacterNumber/TestYC_T.jpg", 1);
//	Z = imread("../InputImage/allCharacterNumber/TestZC_T.jpg", 1);
//
//	/*cout << " A " << checkCorner(A) << endl;
//	cout << " B " << checkCorner(B) << endl;
//	cout << " C " << checkCorner(C) << endl;
//	cout << " D " << checkCorner(D) << endl;
//	cout << " E " << checkCorner(E) << endl;
//	cout << " F " << checkCorner(F) << endl;
//	cout << " G " << checkCorner(G) << endl;
//	cout << " H " << checkCorner(H) << endl;
//	cout << " I " << checkCorner(I) << endl;
//	cout << " J " << checkCorner(J) << endl;
//	cout << " K " << checkCorner(K) << endl;
//	cout << " L " << checkCorner(L) << endl;
//	cout << " M " << checkCorner(M) << endl;
//	cout << " N " << checkCorner(N) << endl;
//	cout << " O " << checkCorner(O) << endl;
//	cout << " P " << checkCorner(P) << endl;
//	cout << " Q " << checkCorner(Q) << endl;
//	cout << " R " << checkCorner(R) << endl;
//	cout << " S " << checkCorner(S) << endl;
//	cout << " T " << checkCorner(T) << endl;
//	cout << " U " << checkCorner(U) << endl;
//	cout << " V " << checkCorner(V) << endl;
//	cout << " W " << checkCorner(W) << endl;
//	cout << " X " << checkCorner(X) << endl;
//	cout << " Y " << checkCorner(Y) << endl;
//	cout << " Z " << checkCorner(Z) << endl;*/
//	std::cout << "Demo character regconition" << endl;
//	/*checkMidEdge(B);
//	checkMidEdge(C);
//	checkMidEdge(D);
//	checkMidEdge(E);
//	checkMidEdge(G);
//	checkMidEdge(H);
//	checkMidEdge(K);
//	checkMidEdge(M);
//	checkMidEdge(N);
//	checkMidEdge(S);
//	checkMidEdge(U);
//	checkMidEdge(Z);
//	checkMidEdge(X);*/
//
//	//detectChar(Q);
//	Mat Character[26] = { A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z };
//	char id = 'A';
//	for (int i = 0; i < 26; i++) {
//		cout << "Character identity: " << id;
//		detectChar(Character[i]);
//		id++;
//	}
//	//checkTopEdge(Y, "1100");
//	//checkInner(B);
//	//checkMN(N);
//	//checkMN(M);
//
//	waitKey(0);
//	system("pause");
//	return 1;
//}



