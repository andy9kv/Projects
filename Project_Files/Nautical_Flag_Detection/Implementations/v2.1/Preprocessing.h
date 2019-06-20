
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

//Macro's for sets of colors
#define COLORS_CHARLIE {COLOR_BLUE, COLOR_RED, COLOR_WHITE}
#define COLORS_ECHO {COLOR_BLUE, COLOR_RED}
#define COLORS_GOLF {COLOR_BLUE, COLOR_YELLOW}
#define COLORS_INDIA {COLOR_BLACK, COLOR_YELLOW}
#define COLORS_MIKE {COLOR_BLUE, COLOR_WHITE}
#define COLORS_OSCAR {COLOR_RED, COLOR_YELLOW}
#define COLORS_PAPA {COLOR_BLUE, COLOR_WHITE}
#define COLORS_UNIFORM {COLOR_RED, COLOR_WHITE}
#define COLORS_VICTOR {COLOR_RED, COLOR_WHITE}
#define COLORS_YANKEE v{COLOR_YELLOW, COLOR_RED}
#define COLORS_ZULU {COLOR_BLUE, COLOR_RED, COLOR_YELLOW, COLOR_BLACK}

#define COLORS_CHARLIE_R {COLOR_BLUE, COLOR_WHITE}
#define COLORS_ECHO_R {COLOR_BLUE}
#define COLORS_UNIFORM_R {COLOR_WHITE}

#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

class Preprocessing {

public:

	//edge isolation
	Mat static isolateEdges(Mat);
	Mat static isolateEdges(Mat, int);

	//color isolation
	Mat static isolateColor(Mat, Scalar);
	Mat static isolateColor(Mat, Scalar, Scalar, Scalar);
	Mat static isolateColors(Mat, vector<Scalar>);

	//image management helpers
	bool static sEquals(Scalar, Scalar);
	bool static mEquals(Mat, Mat);
	Mat static addImg(Mat a, Mat b);

private:

	//edge isolation helpers
	Mat static computeGaussian(Mat, int);
	Mat static gaussianResize(Mat, int);
};