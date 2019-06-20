
/*
This class has methods that recognize patterns in an image by
looking at areas with a certain color and finding surrounding
areas that should have certain colors based on the flag pattern
*/

#include "stdafx.h"
#include "Identification.h"

/*--------------------------------------------------------------------*/
/*---------------------Color Bordering Functions----------------------*/
/*-------------------(areas of a particular color)--------------------*/
/*--------------------------------------------------------------------*/

/*Draws border on all areas of color by scanning horizontally or
vertically based on bool 'dir'. true = horizontal, else vertical*/
Mat Identification::borderAreas(Mat img, Scalar color, int dist, bool dir) {
	Mat out = img.clone();
	imgPoint start = imgPoint(0, 0), end = imgPoint(0, 0);
	while(start.row != -1 && start.col != -1) {
		/*gets the next starting point in an image AKA start
		of an area of the desired color*/
		if (dir) { start = findNextOccurance(out, end, color); }
		else { start = findNextOccrVert(out, color, end); }
		if (start.row == -1 || start.col == -1) { return out; }
		/*finds the end of the strech and paints both start and
		end in the bordering color*/
		end = getEnd(out, color, start, dist, dir);
		Scalar paint;
		if (dir) { paint = /*COLOR_BORDERING_H*/COLOR_BLUE; }
		else { paint = /*COLOR_BORDERING_V*/COLOR_BLACK; }
		paintPixel(out, paint, start);
		paintPixel(out, paint, end);
	}
	return out;
}

/*returns the last occurance of the same pixel down a straigt line
given the margin dist. dir == true is horizontal, false = vertical*/
imgPoint Identification::getEnd(Mat img, Scalar color, imgPoint start, int dist, bool dir) {
	imgPoint point = start.getCopy(), prev = imgPoint(0, 0);
	//moves in a horizontal direction
	if (dir)
		while (point.row < img.rows) {
			prev = point;
			point = getNext(img, color, point, dist, dir);
			if (point.row == -1 || point.col == -1) { return prev; }
		}
	//moves in a vertical direction
	else
		while (point.col < img.cols) {
			prev = point;
			point = getNext(img, color, point, dist, dir);
			if (point.row == -1 || point.col == -1) { return prev; }
		}
	return point;
}

/*returns the next continuous pixel (within the margin dist) in the direction 'dir'*/
imgPoint Identification::getNext(Mat img, Scalar color, imgPoint start, int dist, bool dir) {
	if (dir) {
		for (int i = 1; i < (dist + 1); i++) {
			if (i + start.row >= img.rows) { return imgPoint(-1, -1); }
			if (isColorEqual(img, color, imgPoint(start.row + i, start.col)))
				return imgPoint(start.row + i, start.col);
		}
	}
	else {
		for (int j = 1; j < (dist + 1); j++) {
			if (j + start.col >= img.cols) { return imgPoint(-1, -1); }
			if (isColorEqual(img, color, imgPoint(start.row, start.col + j)))
				return imgPoint(start.row, start.col + j);
		}
	}
	return imgPoint(-1, -1);
}

/*returns an image with all areas of the specified color bordered based
on the macro COLOR_BORDERING. Compactness is a measure of how "far away"
a  bordering pixel is, allowing for things obstructing the image*/
Mat Identification::borderColorAreas(Mat img, Scalar color, int compactness) {
	Mat out = img.clone();
	Mat out1 = img.clone();
	out = borderAreas(out, color, compactness, DIR_HORIZONTAL);
	out1 = borderAreas(out1, color, compactness, DIR_VERTICAL);
	//boxArea(out, out);
	return out;
}

/*--------------------------------------------------------------------*/
/*-------------------Pattern Recognition Functions--------------------*/
/*----------------(matching shapes and areas of color)----------------*/
/*--------------------------------------------------------------------*/

/*Takes in an image with a color isolated and an image with
the areas of that color bordered and returns an image with
the bordered areas roughly isolated in the color isolated image*/
Mat Identification::boxArea(Mat isolated, Mat bordered) {
	//gets the coordinates of all the bordering pixels
	vector<imgPoint> hBorders = vector<imgPoint>(), vBorders = vector<imgPoint>();
	for (int j = 0; j < bordered.cols; j++)
		for (int i = 0; i < bordered.rows; i++) {
			if (isColorEqual(bordered, COLOR_BORDERING_H, imgPoint(i, j)))
				hBorders.push_back(imgPoint(i, j));
			else if (isColorEqual(bordered, COLOR_BORDERING_V, imgPoint(i, j)))
				vBorders.push_back(imgPoint(i, j));
		}
	//sorts the bordering pixels into sets of continuous borders
	vector<imgPoint> coordinates = classifyPoints(hBorders, vBorders);
	//averages the start and end points of the continuous sets
	//uses those averages to draw boxes around the areas
	return bordered;
}

/*groups the given set of bordering points into continuous points
and removes any outliers. Returns a list of these groups*/
vector<imgPoint> Identification::classifyPoints(vector<imgPoint> hPoints, vector<imgPoint> vPoints) {
	vector<vector<imgPoint>> sets = vector<vector<imgPoint>>();
	/*runs through all the points and checks to see if the current point can
	fit into any group in sets, if not a new group is created in sets*/
	for (vector<imgPoint>::iterator iter = hPoints.begin(); iter != hPoints.end(); ++iter) {
		//imgPoint(iter->col, iter->row).print();
		//waitKey(200);
	}
	return vector<imgPoint>();
}

/*--------------------------------------------------------------------*/
/*--------------------------Misc Helper Methods-----------------------*/
/*--------------------------------------------------------------------*/

/*Finds the first occurance of a color in an image, traversing the
image from first left to right, then above to below, beginning at the
specified point. Returns an imgPoint with coordinates (-1, -1) if no
such pixel with the specified color exists. Complexity: O(ij)*/
imgPoint Identification::findNextOccurance(Mat img, imgPoint point, Scalar color) {
	//begins at the pixel after the given starting point
	if (!(point.row + 1 < img.rows)) {
		if (!(point.col + 1 < img.cols))
			return imgPoint(-1, -1);
		point = imgPoint(0, point.col + 1);
	}
	point = imgPoint(point.row + 1, point.col);
	//begins searching for the next occurance of 'color'
	int i = point.row;
	for (int j = point.col; j < img.cols; j++) {
		if (j > point.col) { i = 0;}
		while (i < img.rows) {
			if (isColorEqual(img, color, imgPoint(i, j)))
				return imgPoint(i, j);
			i++;
		}
	}
	return imgPoint(-1, -1);
}

/*Finds the first occurance of a color in an image, traversing the
image from first above to below, then left to right, beginning at the
specified point. Returns an imgPoint with coordinates (-1, -1) if no
such pixel with the specified color exists. Complexity: O(ij)*/
imgPoint Identification::findNextOccrVert(Mat img, Scalar color, imgPoint point) {
	//begins at the pixel after the given starting point
	if (!(point.col + 1 < img.cols)) {
		if (!(point.row + 1 < img.rows))
			return imgPoint(-1, -1);
		point = imgPoint(point.row + 1, 0);
	}
	point = imgPoint(point.row, point.col + 1);
	//begins searching for the next occurance of 'color'
	int j = point.col;
	for (int i = point.row; i < img.rows; i++) {
		if (i > point.row) { j = 0; }
		while (j < img.cols) {
			if (isColorEqual(img, color, imgPoint(i, j)))
				return imgPoint(i, j);
			j++;
		}
	}
	return imgPoint(-1, -1);
}

/*Returns true if the pixel at point is the same color as the
specified color. Complexity: O(1)*/
bool Identification::isColorEqual(Mat img, Scalar color, imgPoint point) {
	//Grayscale Image (1 channel)
	if (img.channels() == 1) {
		uchar curr = img.at<uchar>(point.col, point.row);
		if (color.val[0] == curr)
			return true;
	}
	//RGB Image (3 channels)
	else if (img.channels() == 3) {
		Vec3b currPix = img.at<Vec3b>(point.col, point.row);
		Scalar currVal = Scalar(currPix.val[0], currPix.val[1], currPix.val[2]);
		if (Preprocessing::sEquals(color, currVal))
			return true;
	}
	return false;
}

/*Returns true if the given point is surronded by pixels of the specified
color by the distance 'dist', false otherwise*/
bool Identification::isSurrounded(Mat img, Scalar color, imgPoint point, int dist) {
	for (int i = point.row - dist; i <= point.row + dist; i++)
		for (int j = point.col - dist; j <= point.col + dist; j++)
			if (!(isColorEqual(img, color, imgPoint(i, j))))
				return false;
	return true;
}

/*Returns the location of the last pixel in the image of the specified
color. Seach begins at the lower right corner and progresses first
left and then above*/
imgPoint Identification::findLastOccurance(Mat img, Scalar color) {
	for (int j = img.cols - 1; j >= 0; j--)
		for (int i = img.rows - 1; i >= 0; i--)
			if (isColorEqual(img, color, imgPoint(i, j))) {
				if (i + 1 < img.rows)
					return imgPoint(i + 1, j);
				else if (j + 1 < img.cols)
					return imgPoint(0, j + 1);
				//end of the image
				return imgPoint(-1, -1);
			}
	//If no pixel of the specified color was found
	return imgPoint(-1, -1);
}

/*Testing method to pinpoint a location in an image*/
Mat Identification::drawCircle(Mat img, imgPoint point, Scalar color) {
	Mat out = img.clone();
	Vec3b drawColor = Vec3b(color.val[0], color.val[1], color.val[2]);
	//draws a red circle around point
	if (point.row > 0 && point.col > 0 && point.row < img.rows - 1
		&& point.col < img.cols - 1) {
		out.at<Vec3b>(point.col, point.row + 1) = drawColor;
		out.at<Vec3b>(point.col, point.row - 1) = drawColor;
		out.at<Vec3b>(point.col + 1, point.row) = drawColor;
		out.at<Vec3b>(point.col + 1, point.row + 1) = drawColor;
		out.at<Vec3b>(point.col + 1, point.row - 1) = drawColor;
		out.at<Vec3b>(point.col - 1, point.row) = drawColor;
		out.at<Vec3b>(point.col - 1, point.row + 1) = drawColor;
		out.at<Vec3b>(point.col - 1, point.row - 1) = drawColor;
	}
	out.at<Vec3b>(point.col, point.row) = drawColor;
	return out;
}

/*paints the given pixel the given color in the given image if possible*/
void Identification::paintPixel(Mat img, Scalar color, imgPoint point) {
	if (point.row < img.rows && point.row >= 0 &&
		point.col < img.cols && point.col >= 0) {
		Scalar paint = color;
		Vec3b paintColor = Vec3b(paint.val[0], paint.val[1], paint.val[2]);
		img.at<Vec3b>(point.col, point.row) = paintColor;
	}
}