#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "PCIE.h"
#include <ctime>


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

	//printf("== Terasic: PCIe Demo Program ==\r\n");

	lib_handle = PCIE_Load();
	if (!lib_handle) {
		printf("PCIE_Load failed!\r\n");
		return 0;
	}

	//hPCIE = PCIE_Open(0, 0, 0);
	hPCIE = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
	if (!hPCIE) {
		printf("PCIE_Open failed\r\n");
		return 0;
	}

	String filename;
	cin >> filename;
	filename += ".jpg";

	for (int i = 0; i < 10; i++){
		

		Mat input = imread(filename);
		//imshow("input", input);

		resize(input, input, Size(640, 480));

		// enb = 0;
		BOOL bPass;
		int Mask = 0;
		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
		//printf("Disable flag is sent\n");

		//Convert to grayscale image
		Mat grayImg;
		cvtColor(input, grayImg, CV_BGR2GRAY);

		//imshow("Gray", grayImg);
		// TEST_DMA_MEMORY(hPCIE, input);
		clock_t start_write_img = clock();
		writeImgToDmaMem0(hPCIE, grayImg);
		clock_t end_write_img = clock();
		//printf("The gray image is sent\n");

		//Enable
		// int enb = 1;
		Mask = 4;
		//printf("Mask = %d\n", Mask);

		clock_t start_before_en = clock();
		bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR, (DWORD)Mask);
		clock_t end_write_en = clock();
		if (bPass) printf("Enable flag is sent\n");
		clock_t start_after_en = clock();

		//Wait for FPGA Edge detection done
		bPass = TRUE;
		DWORD Status;

		do {
			bPass = PCIE_Read32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_BUTTON_ADDR, &Status);
			printf("%d\n", Status);
			if (!bPass) printf("Read failed\n");
		} while (Status % 2 == 1);
		//printf("%d - FPGA Done\n", Status);

		// enb = 0;
		// Mask = 0;
		// bPass = PCIE_Write32(hPCIE, DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_LED_ADDR,(DWORD)Mask);
		// printf("Disable flag is sent\n");

		//Read the edge image
		Mat output(480, 640, CV_8UC1);
		clock_t start_read_img = clock();
		readImgFromDmaMem1(hPCIE, &output);
		//printf("Edge image is read\n");
		//imwrite("edge_" + filename, output);

		//---------------
		clock_t start_before_gau_atom = clock();
		Size size(5, 5);
		GaussianBlur(grayImg, grayImg, size, 0);

		//----------Edge dectect by Canny------
		int lowThreshold = 50;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;
		//imshow("Input", input);
		Mat edgeImg;
		Canny(grayImg, edgeImg, lowThreshold, lowThreshold*ratio, kernel_size);
		clock_t end_after_canny_atom = clock();
		//----------------------------

		clock_t end = clock();
		double elapsed_beEn_enProg = double(end - start_before_en) / CLOCKS_PER_SEC;
		double elapsed_beEn_enEn = double(end_write_en - start_before_en) / CLOCKS_PER_SEC;
		double elapsed_beDMA_enDMA = double(end_write_img - start_write_img) / CLOCKS_PER_SEC;
		double elapsed_enEn_enProg = double(end - start_after_en) / CLOCKS_PER_SEC;
		double elapsed_beGau_enCan_atom = double(end_after_canny_atom - start_before_gau_atom) / CLOCKS_PER_SEC;
		double elapsed_beRead_beGau = double(start_before_gau_atom - start_read_img) / CLOCKS_PER_SEC;
		double elapsed_FPGA_canny = double(start_read_img - start_after_en) / CLOCKS_PER_SEC;

		cout << "elapsed_beEn_enProg: " << elapsed_beEn_enProg << endl;
		cout << "elapsed_beEn_enEn: " << elapsed_beEn_enEn << endl;
		cout << "elapsed_beDMA_enDMA: " << elapsed_beDMA_enDMA << endl;
		cout << "elapsed_enEn_enProg: " << elapsed_enEn_enProg << endl;
		cout << "elapsed_beGau_enCan_atom: " << elapsed_beGau_enCan_atom << endl;
		cout << "elapsed_beRead_beGau: " << elapsed_beRead_beGau << endl;
		cout << "elapsed_FPGA_canny: " << elapsed_FPGA_canny << endl;

		imshow("Output", output);
	}
	PCIE_Close(hPCIE);
	PCIE_Unload(lib_handle);

	waitKey(0);
	return 0;
}
