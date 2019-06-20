
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
	/*computes the amount of pixels the final image should have. Gets
	the y value in an y x y square image, increses the size of the
	image n times, and resizes the final product to a perfect square*/
	int pixels = blurred.total();
	//resizes blurred n times
	Mat out = blurred;
	for (int i = 0; i < n; i++) {
		pyrUp(out, out, Size(), BORDER_DEFAULT);
		pixels = pixels * 4;
	}
	//ensures blurred is of proper size
	resize(out, out, Size(sqrt(pixels), sqrt(pixels)));
	return out;
}

/*--------------------------------------------------------------------*/
/*----------------Methods to isolate color in an image----------------*/
/*--------------------------------------------------------------------*/

/*Returns an image with the given color as white and the rest as black*/
Mat Preprocessing::isolateColor(Mat img, int color) {
	Mat out;
	if (color == COLOR_WHITE)
		inRange(img, Scalar(215, 215, 215), Scalar(255, 255, 255), out);
	else if (color == COLOR_YELLOW)
		inRange(img, Scalar(0, 40, 0), Scalar(50, 255, 50), out);
	else if (color == COLOR_BLUE)
		inRange(img, Scalar(40, 0, 0), Scalar(255, 50, 50), out);
	else if (color == COLOR_RED)
		inRange(img, Scalar(0, 0, 40), Scalar(50, 50, 255), out);
	else if (color == COLOR_BLACK)
		inRange(img, Scalar(0, 0, 0), Scalar(25, 25, 25), out);
	return out;
}

/*--------------------------------------------------------------------*/
/*----------------Methods to locate a flag in an image----------------*/
/*--------------------------------------------------------------------*/