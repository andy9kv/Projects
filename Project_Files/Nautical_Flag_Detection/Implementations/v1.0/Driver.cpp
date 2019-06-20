
#include "stdafx.h"
#include "Driver.h"

int main() {
	Mat img1 = imread("Images/Charlie/Charlie_1.jpg");
	Mat img2 = imread("Images/Charlie/Charlie_2.jpg");
	Mat img3 = imread("Images/Charlie/Charlie_3.jpg");
	Mat img4 = imread("Images/Charlie/Charlie_4.jpg");
	Mat img5 = imread("Images/Charlie/Charlie_5.jpg");
	Mat img6 = imread("Images/Charlie/Charlie_6.jpg");
	Mat red1 = Preprocessing::isolateColor(img1, COLOR_RED);
	Mat red2 = Preprocessing::isolateColor(img2, COLOR_RED);
	Mat red3 = Preprocessing::isolateColor(img3, COLOR_RED);
	Mat red4 = Preprocessing::isolateColor(img4, COLOR_RED);
	Mat red5 = Preprocessing::isolateColor(img5, COLOR_RED);
	Mat red6 = Preprocessing::isolateColor(img6, COLOR_RED);
	//imwrite("test.jpg", red1);
	imshow("original", img6);
	imshow("red", red6);
	waitKey(0);
	return 0;
}