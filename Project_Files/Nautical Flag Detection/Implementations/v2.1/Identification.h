
//Preprocessor Directives
#pragma once
#include "Preprocessing.h"

//Directional Macros
#define DIR_HORIZONTAL true
#define DIR_VERTICAL false
#define DIR_ABOVE 0
#define DIR_RIGHT 1
#define DIR_BELOW 2
#define DIR_LEFT 3

//Setting Macros
#define SUR_LIMIT 1
#define MIN_PERCENT 45
#define FILTER_TIMES 15
#define PIXEL_DIST_ALLOWANCE 2

//Flag Macros
#define CHARLIE 0
#define ECHO 1
#define UNIFORM 2

/*Class that defines a point in an image*/
class imgPoint {
public:
	int row;
	int col;
	imgPoint(int x, int y) {
		row = x;
		col = y;
	}
	imgPoint getCopy() {return imgPoint(row, col);}
	void print() {cout << "row: " << row << " col: " << col << endl;}
};

class Identification {

private:
	bool recognizedFlag;

//bordering
	vector<vector<imgPoint>> static getBorders(Mat, Scalar, int);
	imgPoint static getEnd(Mat, Scalar, imgPoint, int);
	imgPoint static getNext(Mat, Scalar, imgPoint, int);
	vector<imgPoint> static elimRowOutliers(vector<imgPoint>, int);
	vector<imgPoint> static elimColOutliers(vector<imgPoint>, int);
	vector<imgPoint> static getEndpoints(vector<imgPoint>, bool);
	vector<imgPoint> static getEndpoints(vector<imgPoint>);
	Mat static boxArea(Mat, Scalar, int);
	vector<imgPoint> static getAreaBox(Mat, Scalar, int);

//pattern matching
	int static getPercentage(Mat, Scalar, vector<imgPoint>);
	vector<imgPoint> static getNextArea(vector<imgPoint>, int);
	Mat static isolateRange(Mat, Scalar, int);
	bool static isEnoughPixels(vector<int>, int);
	vector<Mat> static getFilters(Mat, Scalar);

//flag identification methods
	Mat control(Mat, int);
	Mat Charlie(Mat, Mat, int);
	Mat Echo(Mat, Mat, int);
	Mat Uniform(Mat, Mat, int);

//helpers
	bool static isColorEqual(Mat, Scalar, imgPoint);
	bool static isSurrounded(Mat, Scalar, imgPoint, int);
	imgPoint static findNextOccurance(Mat, imgPoint, Scalar);
	imgPoint static findNextOccrVert(Mat, Scalar, imgPoint);
	imgPoint static findLastOccurance(Mat, Scalar);

//image drawing
	void static paintPixel(Mat, Scalar, imgPoint);
	Mat static drawCircle(Mat, imgPoint, Scalar);
	Mat static drawLine(Mat, Scalar, imgPoint, imgPoint);
	Mat static drawBox(Mat, Scalar, vector<imgPoint>);
	Mat static drawBorders(Mat, Scalar, vector<imgPoint>);

public:

	//constructors
	Identification(Mat, int);
};