
#include "stdafx.h"
#include "Driver.h"

int main() {

	//loads flags
	Mat c1 = imread("Images/Charlie/Charlie_1.jpg");
	Mat c2 = imread("Images/Charlie/Charlie_2.jpg");
	Mat c3 = imread("Images/Charlie/Charlie_3.jpg");
	Mat c4 = imread("Images/Charlie/Charlie_4.jpg");
	Mat c5 = imread("Images/Charlie/Charlie_5.jpg");
	Mat c6 = imread("Images/Charlie/Charlie_6.jpg");

	Mat i1 = Preprocessing::isolateColor(c1, COLOR_RED);
	Mat i2 = Preprocessing::isolateColor(c2, COLOR_RED);
	Mat i3 = Preprocessing::isolateColor(c3, COLOR_RED);
	Mat i4 = Preprocessing::isolateColor(c4, COLOR_RED);
	Mat i5 = Preprocessing::isolateColor(c5, COLOR_RED);
	Mat i6 = Preprocessing::isolateColor(c6, COLOR_RED);

	Mat l1 = Preprocessing::isolateEdges(i1);
	Mat l2 = Preprocessing::isolateEdges(i2);
	Mat l3 = Preprocessing::isolateEdges(i3);
	Mat l4 = Preprocessing::isolateEdges(i4);
	Mat l5 = Preprocessing::isolateEdges(i5);
	Mat l6 = Preprocessing::isolateEdges(i6);

	imshow("original", c1);
	imshow("laplace", l1);
	imshow("colors", i1);
	waitKey(10000);
	imshow("original", c2);
	imshow("laplace", l2);
	imshow("colors", i2);
	waitKey(10000);
	imshow("original", c3);
	imshow("laplace", l3);
	imshow("colors", i3);
	waitKey(10000);
	imshow("original", c4);
	imshow("laplace", l4);
	imshow("colors", i4);
	waitKey(10000);
	imshow("original", c5);
	imshow("laplace", l5);
	imshow("colors", i5);
	waitKey(10000);
	imshow("original", c6);
	imshow("laplace", l6);
	imshow("colors", i6);
	waitKey(10000);

	//isolates red
	Mat p1 = Preprocessing::isolateColor(c1, COLOR_RED);
	imshow("Processed", p1);
	//waitKey(4000);
	Mat b1 = Identification::borderColorAreas(p1, COLOR_RED, 5);
	imshow("Bordered", b1);
	waitKey(0);

	Mat p2 = Preprocessing::isolateColor(c2, COLOR_RED);
	imshow("Processed", p2);
	waitKey(4000);
	Mat b2 = Identification::borderColorAreas(p2, COLOR_RED, 2);
	imshow("Bordered", b2);
	waitKey(4000);

	Mat p3 = Preprocessing::isolateColor(c3, COLOR_RED);
	imshow("Processed", p3);
	waitKey(4000);
	Mat b3 = Identification::borderColorAreas(p3, COLOR_RED, 2);
	imshow("Bordered", b3);
	waitKey(4000);

	Mat p4 = Preprocessing::isolateColor(c4, COLOR_RED);
	imshow("Processed", p4);
	waitKey(4000);
	Mat b4 = Identification::borderColorAreas(p4, COLOR_RED, 2);
	imshow("Bordered", b4);
	waitKey(4000);

	Mat p5 = Preprocessing::isolateColor(c5, COLOR_RED);
	imshow("Processed", p5);
	waitKey(4000);
	Mat b5 = Identification::borderColorAreas(p5, COLOR_RED, 2);
	imshow("Bordered", b5);
	waitKey(4000);

	Mat p6 = Preprocessing::isolateColor(c6, COLOR_RED);
	imshow("Processed", p6);
	waitKey(4000);
	Mat b6 = Identification::borderColorAreas(p6, COLOR_RED, 2);
	imshow("Bordered", b6);
	waitKey(0);
	return 0;
}