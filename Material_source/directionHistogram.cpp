int main() {
	Mat input;
	input = imread("input3.jpg");
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

	int min = 480;
	int max = 0;
	int sum = 0;
	for (int i = 0; i < 640; i++) {
		if (vHis[i] < min) min = vHis[i];
		if (vHis[i] > max) max = vHis[i];
		sum += vHis[i];
	}
	int avgMM = (max + min) / 2;
	int avgS = sum / 640;

	int hmin = 640;
	int hmax = 0;
	int hsum = 0;
	for (int i = 0; i < 480; i++) {
		if (hHis[i] < hmin) hmin = hHis[i];
		if (hHis[i] > hmax) hmax = hHis[i];
		hsum += hHis[i];
	}
	int havgMM = (max + min) / 2;
	int havgS = sum / 480;


	//ve duong thang
	for (int i = 0; i < 640; i++) {
		if (vHis[i] >= avgMM) {
			if(i > 0 && i < 639 && (vHis[i - 1] < avgMM || vHis[i + 1] < avgMM))
				line(vhHisImg, Point(i, 0), Point(i, 480), Scalar(0, 255, 255), 1);
		}
	}

	for (int i = 0; i < 480; i++) {
		if (hHis[i] >= havgMM) {
			if (i > 0 && i < 479 && (vHis[i - 1] < avgMM || vHis[i + 1] < avgMM))
				line(vhHisImg, Point(0, i), Point(640, i), Scalar(255, 0, 0), 1);
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
	line(vhHisImg, Point(0, 480 - avgMM), Point(639, 480 - avgMM), Scalar(0, 255, 0), 1);
	line(vhHisImg, Point(0, 480 - avgS), Point(639, 480 - avgS), Scalar(0, 0, 255), 1);

	line(vhHisImg, Point(havgMM, 0), Point(havgMM, 480), Scalar(0, 255, 0), 1);
	line(vhHisImg, Point(havgS, 0), Point(havgS, 480), Scalar(0, 0, 255), 1);


	imshow("Origin", input);
	imshow("Vertical Histigram", vhHisImg);

	waitKey(0);
	return 0;
}