
/*
This class has methods that remove noise and unecessary aspects
in an image
*/

#include "stdafx.h"
#include "Preprocessing.h"

/*--------------------------------------------------------------------*/
/*--------------Methods to isolates the edges in an image-------------*/
/*--------------------------------------------------------------------*/

/*Returns an image with the edges of img highlighted. The parameter
int clairity specifies how visible the edges are.*/
Mat Preprocessing::isolateEdges(Mat img, int clarity) {
	Mat blurred = computeGaussian(img, clarity);
	//ensures blurred is of proper size
	resize(blurred, blurred, Size(img.cols, img.rows));
	//cout << blurred.rows << "a;lskdj" << blurred.cols << endl;
	//cout << img.rows << "a;lskdj" << img.cols << endl;
	subtract(img, blurred, blurred, noArray(), -1);
	return blurred;
}

/*Returns an image with the edges of img highlighted. Calls the other
highlihgtEdges with a default value of 3.*/
Mat Preprocessing::isolateEdges(Mat img) {
	return isolateEdges(img, 3);
}

/*Computes the Gaussian algorithm n times on img and returns the
resulting image of equal size as the original image*/
Mat Preprocessing::computeGaussian(Mat img, int n) {
	//computes the Gaussian Blur n times
	Mat out = img;
	for (int i = 0; i < n; i++)
		pyrDown(out, out, Size(), BORDER_DEFAULT);
	return gaussianResize(out, n);
}

/*A helper method for computeGaussian, returns the blurred image resized
n times*/
Mat Preprocessing::gaussianResize(Mat blurred, int n) {
	//resizes blurred n times
	Mat out = blurred;
	for (int i = 0; i < n; i++)
		pyrUp(out, out, Size(), BORDER_DEFAULT);
	return out;
}

/*--------------------------------------------------------------------*/
/*----------------Methods to isolate color in an image----------------*/
/*--------------------------------------------------------------------*/

/*Returns an image with the given color isolated*/
Mat Preprocessing::isolateColor(Mat img, Scalar color) {
	Mat dest;
	Mat out = img.clone();
	out.setTo(COLOR_GREEN);
	if (sEquals(color, COLOR_WHITE)) {
		inRange(img, Scalar(170, 170, 170), Scalar(255, 255, 255), dest);
		out.setTo(COLOR_WHITE, dest);
	}
	else if (sEquals(color, COLOR_YELLOW)) {
		inRange(img, Scalar(0, 155, 155), Scalar(25, 255, 255), dest);
		out.setTo(COLOR_YELLOW, dest);
	}
	else if (sEquals(color, COLOR_BLUE)){
		inRange(img, Scalar(50, 0, 0), Scalar(255, 30, 30), dest);
		out.setTo(COLOR_BLUE, dest);
	}
	else if (sEquals(color, COLOR_RED)){
		inRange(img, Scalar(0, 0, 50), Scalar(30, 30, 255), dest);
		out.setTo(COLOR_RED, dest);
	}
	else if (sEquals(color, COLOR_BLACK)){
		inRange(img, Scalar(0, 0, 0), Scalar(30, 30, 30), dest);
		out.setTo(COLOR_BLACK, dest);
	}
	return out;
}

/*Returns an image with the given colors isolated. Colors are given in
the form of an array flag*/
Mat Preprocessing::isolateColors(Mat img, vector<Scalar> flag) {
	Mat out = img.clone();
	out.setTo(0);
	for (int i = 0; i < flag.size(); i++)
		add(isolateColor(img, flag[i]), out, out, noArray());
	return out;
}

/*--------------------------------------------------------------------*/
/*-------------------------Misc Helper Methods------------------------*/
/*--------------------------------------------------------------------*/

/*Compares two Scalars for equality*/
bool Preprocessing::sEquals(Scalar s1, Scalar s2) {
	if (s1.val[0] == s2.val[0] && s1.val[1] == s2.val[1] && s1.val[2] == s2.val[2])
		return true;
	return false;
}