int main() {

	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
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
		int lowThreshold = 70;
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
		vector<Vec4i> linesA, linesB;
		HoughLinesP(edgeImg, linesA, 1, CV_PI / 180, 70, 30, 50);
		HoughLinesP(edgeImg, linesB, 1, CV_PI / 180, 30, 50, 50);

		//----------Draw lines------------------
		Mat lineImg(input.size(), CV_8UC1, Scalar(0));
		int stretch = 0; //Number of pixel to stretch lines
		double tanV = 8;
		double tanH = 0.08;
		for (size_t i = 0; i < linesA.size(); i++) {
			Vec4i l = linesA[i];
			float w = abs(l[1] - l[3]);
			float h = abs(l[0] - l[2]);
			float tan = w / h;
			if (tan < 0.08) {
				line(lineImg, Point(l[0] - stretch, l[1]), Point(l[2] + stretch, l[3]), Scalar(255), 1, CV_AA);
			}
			if (tan > tanV) {
				line(lineImg, Point(l[0], l[1] - stretch), Point(l[2], l[3] + stretch), Scalar(255), 1, CV_AA);
			}
		}
		for (size_t i = 0; i < linesB.size(); i++) {
			Vec4i l = linesB[i];
			float w = abs(l[1] - l[3]);
			float h = abs(l[0] - l[2]);
			float tan = w / h;
			if (tan < 0.08) {
				line(lineImg, Point(l[0] - stretch, l[1]), Point(l[2] + stretch, l[3]), Scalar(255), 1, CV_AA);
			}
			if (tan > tanV) {
				line(lineImg, Point(l[0], l[1] - stretch), Point(l[2], l[3] + stretch), Scalar(255), 1, CV_AA);
			}
		}

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
		vhHisImg = Mat(480, 640, CV_8UC3, Scalar(0, 0, 0));
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
			if (vHis[i] >= vAvg) {
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
			if (hHis[i] >= hAvg) {
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
			if (hHis[i] >= hAvg) {
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
						//cout << iSum << endl;
						/*double iAvg = iSum / (abs(iPoints[i][0] - iPoints[j][0]) + abs(iPoints[i][1] - iPoints[j][1]));
						if (iSum > iMax) {
						selectedP1 = Point(iPoints[i]);
						selectedP2 = Point(iPoints[j]);
						iMax = iSum;
						}*/

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

						//double iAvg = (iSum / (xEnd - xStart + yEnd - yStart));

						/*int black = 0, white = 0;

						for (int m = yStart; m <= yEnd; m++) {
						for (int n = xStart; n <= xEnd; n++) {
						if ((int)binImg.at<uchar>(Point(m, n)) == 0) black++;
						else white++;
						}
						}

						if (black > 0 && 2*black <= white) {
						if (iAvg > iMax) {
						cout << black << " " << white << endl;
						selectedP1 = Point(iPoints[i]);
						selectedP2 = Point(iPoints[j]);
						iMax = iAvg;
						}
						}*/

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


		//imshow("Origin", input);
		imshow("Direction Histogram", vhHisImg);


		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}