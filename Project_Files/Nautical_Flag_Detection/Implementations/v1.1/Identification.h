
//Preprocessor Directives
#pragma once
#include "Preprocessing.h"

//Direction Macro's
#define DIR_RIGHT_DOWN 0
#define DIR_DOWN_LEFT 1
#define DIR_LEFT_UP 2
#define DIR_UP_RIGHT 3
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

/*Class that defines directional pairs*/
class dirPair {
public:
	int dir1;
	int dir2;
	int curr;
	dirPair(int d1, int d2) { dir1 = d1; dir2 = d2; curr = dir1;}
	/*gets the next direction of search*/
	int nextDir() { 
		if (curr == dir1) { curr = dir2; }
		else {curr = dir1; }
		return curr; 
	}
};

class Identification {

private:
	//bordering
	Mat static borderHoriz(Mat, Scalar, imgPoint, int);
	Mat static borderVert(Mat, Scalar, imgPoint, int);
	imgPoint static getEnd(Mat, Scalar, imgPoint, int, bool);
	//helpers
	bool static isColorEqual(Mat, Scalar, imgPoint);
	bool static isSurrounded(Mat, Scalar, imgPoint, int);
	void static paintPixel(Mat, Scalar, imgPoint);
	vector<dirPair> static buildDirections();

public:
	Mat static borderColorAreas(Mat, Scalar, int);
	Mat static borderArea(Mat, imgPoint, Scalar, int);
	imgPoint static findNextBorder(Mat, imgPoint, int, int);
	imgPoint static findNextOccurance(Mat, imgPoint, Scalar);
	imgPoint static findLastOccurance(Mat, Scalar);
	Mat static drawCircle(Mat, imgPoint, Scalar);
};