#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
/*
int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "PCIE.h"


#define DEMO_PCIE_USER_BAR			PCIE_BAR0
#define DEMO_PCIE_IO_LED_ADDR		0x00
#define DEMO_PCIE_IO_BUTTON_ADDR	0x20
#define DEMO_PCIE_FIFO_WRITE_ADDR	0x40
#define DEMO_PCIE_FIFO_STATUS_ADDR	0x60
#define DEMO_PCIE_FIFO_READ_ADDR	0x80
#define DEMO_PCIE_MEM_ADDR			0x80000
#define DEMO_PCIE_MEM_ADDR_1		0x180000

#define MEM_SIZE			(300*1024) //128KB
#define FIFO_SIZE			(16*1024) // 2KBx8


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

	// write test pattern
	if (bPass) {
		bPass = PCIE_DmaWrite(hPCIe, LocalAddr, pWrite, nTestSize);
		if (!bPass)
			sprintf(szError, "DMA Memory:PCIE_DmaWrite failed\r\n");
	}
	printf("Write to RAM 0 successfully!\n");


	// free resource
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

int main() {
	
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

	hPCIE = PCIE_Open(0, 0, 0);
	if (!hPCIE) {
		printf("PCIE_Open failed\r\n");
		return 0;
	}
	
	String filename;
	cin >> filename;
	filename += ".jpg";

	Mat input = imread(filename);
	//imshow("input", input);

	resize(input, input, Size(640, 480));

	// enb = 0;
	BOOL bPass;
	int Mask = 0;
	bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR,(DWORD)Mask);
	printf("Disable flag is sent\n");

	//Convert to grayscale image
	Mat grayImg;
	cvtColor(input, grayImg, CV_BGR2GRAY);

	//imshow("Gray", grayImg);
	// TEST_DMA_MEMORY(hPCIE, input);
	writeImgToDmaMem0(hPCIE, grayImg);
	printf("The gray image is sent\n");

	//Enable
	// int enb = 1;
	Mask = 4;
	printf("Mask = %d\n", Mask);

	bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR,(DWORD)Mask);
	if(bPass) printf("Enable flag is sent\n");

	//Wait for FPGA Edge detection done
	bPass = TRUE;
	DWORD Status;

	do {
		bPass = PCIE_Read32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_BUTTON_ADDR,&Status);
		printf("%d\n", Status);
		if(!bPass) printf("Read failed\n");
	} while(Status % 2 == 1);
	printf("%d - FPGA Done\n", Status);

	// enb = 0;
	// Mask = 0;
	// bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR,(DWORD)Mask);
	// printf("Disable flag is sent\n");

	//Read the edge image
	Mat output(480, 640,  CV_8UC1);
	readImgFromDmaMem1(hPCIE, &output);
	printf("Edge image is read\n");
	imwrite("edge_" + filename, output);
	//imshow("Output", output);




	PCIE_Close(hPCIE);

	waitKey(0);


	PCIE_Unload(lib_handle);
	return 0;
}
