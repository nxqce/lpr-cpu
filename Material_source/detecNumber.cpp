String checkCorner(Mat input) {
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

String checkMidEdge(Mat input) {
	String result;

	cvtColor(input, input, CV_BGR2GRAY);
	threshold(input, input, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("input", input);

	int black = 0, white = 0, damm = 0;
	for (int l = 0; l < 6; l++) {
		for (int c = 0; c < 3; c++) {
			white = 0;
			black = 0;
			if ((l == 0) ||  (l == 5));
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
