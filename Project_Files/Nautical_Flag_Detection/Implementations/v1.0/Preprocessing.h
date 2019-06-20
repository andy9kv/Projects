
//Preprocessor Directives
#pragma once

#define COLOR_WHITE 0
#define COLOR_YELLOW 1
#define COLOR_BLUE 2
#define COLOR_RED 3
#define COLOR_BLACK 4

#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

class Preprocessing {

public:
	Mat static isolateEdges(Mat);
	Mat static isolateEdges(Mat, int);
	Mat static isolateColor(Mat, int);

private:
	Mat static computeGaussian(Mat, int);
	Mat static gaussianResize(Mat, int);

};