
//Preprocessor Directives
#pragma once

//Macro's for color, BGR format rather than RGB
#define COLOR_WHITE Scalar(255, 255, 255)
#define COLOR_YELLOW Scalar(0, 255, 255)
#define COLOR_BLUE Scalar(255, 0, 0)
#define COLOR_GREEN Scalar(0, 255, 0)
#define COLOR_RED Scalar(0, 0, 255)
#define COLOR_BLACK Scalar(0, 0, 0)
//Purple bordering color
#define COLOR_BORDERING Scalar(255, 0, 255)
#define COLOR_BORDERING_H Scalar(255, 0, 254) 
#define COLOR_BORDERING_V Scalar(254, 0, 255)

//Macro's for sets of colors
#define COLORS_CHARLIE vector<int>{COLOR_BLUE, COLOR_RED, COLOR_WHITE}
#define COLORS_GOLF vector<int>{COLOR_BLUE, COLOR_YELLOW}
#define COLORS_INDIA vector<int>{COLOR_BLACK, COLOR_YELLOW}
#define COLORS_MIKE vector<int>{COLOR_BLUE, COLOR_WHITE}
#define COLORS_OSCAR vector<int>{COLOR_RED, COLOR_YELLOW}
#define COLORS_PAPA vector<int>{COLOR_BLUE, COLOR_WHITE}
#define COLORS_UNIFORM vector<int>{COLOR_RED, COLOR_WHITE}
#define COLORS_VICTOR vector<int>{COLOR_RED, COLOR_WHITE}
#define COLORS_YANKEE vector<int>{COLOR_YELLOW, COLOR_RED}
#define COLORS_ZULU vector<int>{COLOR_BLUE, COLOR_RED, COLOR_YELLOW, COLOR_BLACK}

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
	Mat static isolateColor(Mat, Scalar);
	Mat static isolateColors(Mat, vector<Scalar>);
	bool static sEquals(Scalar, Scalar);

private:
	Mat static computeGaussian(Mat, int);
	Mat static gaussianResize(Mat, int);
};