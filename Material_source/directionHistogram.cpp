int main() {
	Mat input;
	String filename;
	cin >> filename;
	filename += ".jpg";
	input = imread(filename);
	if (!input.data) {
		return 1;
	}

	resize(input, input, Size(640, 480));

	//----------Noise filter---------------
	Size size(5, 5);
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

	int vHis[640] = { 0 };
	int hHis[480] = { 0 };

	for (int i = 0; i < edgeImg.cols; i++) {
		for (int j = 0; j < edgeImg.rows; j++) {
			if (edgeImg.at<uchar>(j, i) != 0)
				vHis[i]++;
		}
	}

	for (int i = 0; i < edgeImg.rows; i++) {
		for (int j = 0; j < edgeImg.cols; j++) {
			if (edgeImg.at<uchar>(i, j) != 0)
				hHis[i]++;
		}
	}

	Mat vhHisImg(480, 640, CV_8UC3, Scalar(0, 0, 0));
	cvtColor(edgeImg, vhHisImg, CV_GRAY2BGR);

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


	//ve duong thang
	int count = 0, start = 0;
	int countMax = 20;
	for (int i = 0; i < 640; i++) {
		if (vHis[i] >= vAvgMM) {
			//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
			if(count == 0) line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
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
		if (vHis[i] >= vAvgMM) {
			//if(i > 0 && i < 639 && (vHis[i - 1] < vAvgMM || vHis[i + 1] < vAvgMM))
			if (count == 0) line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
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
		if (hHis[i] >= hAvgMM) {
			//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
			if (count == 0) line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
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
		if (hHis[i] >= hAvgMM) {
			//if (i > 0 && i < 479 && (hHis[i - 1] < hAvgMM || hHis[i + 1] < hAvgMM))
			if (count == 0) line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
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


	imshow("Origin", input);
	imshow("Direction Histogram", vhHisImg);

	waitKey(0);
	return 0;
}