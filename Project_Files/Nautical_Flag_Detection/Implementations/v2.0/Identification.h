
//Preprocessor Directives
#pragma once
#include "Preprocessing.h"

//Direction Macro's
#define DIR_HORIZONTAL true
#define DIR_VERTICAL false
#define SUR_LIMIT 1

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
	//pattern matching
	Mat static boxArea(Mat, Mat);
	vector<imgPoint> static classifyPoints(vector<imgPoint>, vector<imgPoint>);
	//bordering
	Mat static borderAreas(Mat, Scalar, int, bool);
	imgPoint static getEnd(Mat, Scalar, imgPoint, int, bool);
	imgPoint static getNext(Mat, Scalar, imgPoint, int, bool);
	//helpers
	bool static isColorEqual(Mat, Scalar, imgPoint);
	bool static isSurrounded(Mat, Scalar, imgPoint, int);
	void static paintPixel(Mat, Scalar, imgPoint);

public:
	Mat static borderColorAreas(Mat, Scalar, int);
	imgPoint static findNextOccurance(Mat, imgPoint, Scalar);
	imgPoint static findNextOccrVert(Mat, Scalar, imgPoint);
	imgPoint static findLastOccurance(Mat, Scalar);
	Mat static drawCircle(Mat, imgPoint, Scalar);
};