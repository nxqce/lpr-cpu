#pragma once
#include "characterAlphabet.cpp"
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

String checkCorner(Mat input);
String checkRightEdge(Mat input);
String checkTopEdge(Mat input, String type);
String checkMidEdgeRight(Mat input);
String checkInner(Mat input);
String checkMN(Mat input);
String checkMidEdge(Mat input);
String detectChar(Mat input);



