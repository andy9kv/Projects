
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

/*Draws border on all areas of color by scanning horizontally*/
Mat Identification::borderHoriz(Mat img, Scalar color, imgPoint point, int dist) {
	Mat out = img.clone();
	imgPoint start = imgPoint(0, 0);
	while(start.row != 1 && start.col != 1) {
			imgPoint start = findNextOccurance(img, start, color);
			paintPixel(out, color, start);
			//get pixel after start

		}
}

/*returns the last occurance of the same pixel down a straigt line
given the margin dist. dir == true is horizontal, false = vertical*/
imgPoint Identification::getEnd(Mat img, Scalar color, imgPoint start, int dist, bool dir) {
	imgPoint point = start;
	imgPoint last = imgPoint(-1, -1);
	//moves in a horizontal direction
	if (dir)
		while (point.row < img.rows) {

		}
	//moves in a vertical direction
	else
		while (point.col < img.cols) {

		}
	return last;
}

Mat static borderVert(Mat, Scalar, imgPoint, int);

/*Returns the next bordering point of an area in an image. Returns an
imgPoint with values of negative one (-1) if no such point exists. This
method searches for pixels of the same color that are at most 'dist' apart.
The parameter 'dir' specifies from which direction the previous point was
so that pixels will be searched for along the border*/
imgPoint Identification::findNextBorder(Mat img, imgPoint prev, int dir, int dist) {
	//gets the color that is being searched for
	Vec3b previous = img.at<Vec3b>(prev.col, prev.row);
	Scalar color = Scalar(previous.val[0], previous.val[1], previous.val[2]);
	/*Search for the border first right, then below*/
	if (dir == DIR_RIGHT_DOWN) {
		for (int j = prev.col; j <= prev.col + dist; j++)
			for (int i = prev.row + dist; i >= prev.row; i--) {
				if (!(i == prev.row && j == prev.col))
					if (isColorEqual(img, color, imgPoint(i, j)))
						if (!isSurrounded(img, color, imgPoint(i, j), SUR_LIMIT))
							return imgPoint(i, j);
			}
	}
	/*Search for the border first below, then left*/
	else if (dir == DIR_DOWN_LEFT) {
		for (int i = prev.row; i >= prev.row - dist; i--)
			for (int j = prev.col + dist; j >= prev.col; j--) {
				if (!(i == prev.row && j == prev.col))
					if (isColorEqual(img, color, imgPoint(i, j)))
						if (!isSurrounded(img, color, imgPoint(i, j), SUR_LIMIT))
							return imgPoint(i, j);
			}
	}
	/*Search for the border first left, then above*/
	else if (dir == DIR_LEFT_UP) {
		for (int j = prev.col; j >= prev.col - dist; j--)
			for (int i = prev.row - dist; i <= prev.row; i++) {
				if (!(i == prev.row && j == prev.col))
					if (isColorEqual(img, color, imgPoint(i, j)))
						if (!isSurrounded(img, color, imgPoint(i, j), SUR_LIMIT))
							return imgPoint(i, j);
			}
	}
	/*Search for the border first above, then right*/
	else {
		for (int i = prev.row; i <= prev.row + dist; i++)
			for (int j = prev.col - dist; j <= prev.col; j++) {
				if (!(i == prev.row && j == prev.col))
					if (isColorEqual(img, color, imgPoint(i, j)))
						if (!isSurrounded(img, color, imgPoint(i, j), SUR_LIMIT))
							return imgPoint(i, j);
			}
	}
	return imgPoint(-1, -1);
}

/*Returns an image with purple edges drawn around an area of the specified
color, the first pixel of that area is given by parameter 'start'. The parameter
'compactness' specifies how "far away" adjacent bordering pixels can be*/
Mat Identification::borderArea(Mat img, imgPoint start, Scalar color, int compactness) {
	/*control variables*/
		Mat out = img.clone();
		//sets the staring point of the colored area
		imgPoint origin = start;
		imgPoint point = origin.getCopy();
		imgPoint prev = imgPoint(-1, -1);
		//directional control variables
		vector<dirPair> directions = buildDirections();
		bool directionalImprovement = true;
		int direction = DIR_RIGHT_DOWN, pairMarker = 0, i = 0;
	/*borders an image by finding the next bordering pixel in the
	continuing direction and marks the found border*/
	while (point.row != -1 && point.col != -1) {
		prev = point;
		point = findNextBorder(img, point, direction, compactness);
		/*if there is no continuous (within the margin) border in
		the current search direction, changes direction until a
		bordering pixel can be found*/
		while (point.row == -1 || point.col == -1) {
			/*if the direction was previously changed and there
			is a possiblity there are new bordering pixels in
			the other direction of the same directional pair*/
			if (directionalImprovement) {
				//gets the other direction in the same directional pair
				direction = directions[pairMarker].nextDir();
				/*marks that there were no bordering pixels found in
				the previous direction*/
				directionalImprovement = false;
			}
			/*if the direction was just changed and there was no
			new bordering pixel found in the previous one (i.e. in
			the other direction of this directional pair) then the
			directional pair is changed*/
			else {
				pairMarker++;
				if (pairMarker == directions.size()) { pairMarker = 0; }
				//gets a new direction
				direction = directions[pairMarker].nextDir();
				directionalImprovement = true;
			}
			point = findNextBorder(img, prev, direction, compactness);
		}
		point.print();
		cout << direction << endl;
		imshow("test", drawCircle(out, point, COLOR_BLUE)); waitKey(200);
		//paints the border pixel purple
		paintPixel(out, COLOR_BORDERING, point);
		if ((abs(point.row - origin.row) <= compactness) &&
			(abs(point.col - origin.col) <= compactness) &&
			(i > 3)) {
			paintPixel(out, COLOR_BORDERING, origin);
			return out;
		}
		i++;
	}
	//colors the last visited pixel purple regardless of pixel value
	if (point.row == -1)
		paintPixel(out, COLOR_BORDERING, prev);
	return out;
}

/*returns an image with all significant areas of the specified color bordered
in BGR[255, 0, 255] (purple). Compactness is a measure of how "far away" a 
bordering pixel is, allowing for things obstructing the image*/
Mat Identification::borderColorAreas(Mat img, Scalar color, int compactness) {
	Mat out = img.clone();
	imgPoint startingPoint = imgPoint(0, 0);
	/*while new areas of color are still found*/
	while (findNextOccurance(out, startingPoint, color).row != -1) {
		startingPoint = findNextOccurance(out, startingPoint, color);
		out = borderArea(out, startingPoint, color, compactness);
		startingPoint = findLastOccurance(out, COLOR_BORDERING);
	}
	return out;
}

/*--------------------------------------------------------------------*/
/*--------------------------Misc Helper Methods-----------------------*/
/*--------------------------------------------------------------------*/

/*Finds the first occurance of a color in an image, traversing the
image from left to right, beginning at the specified point. Returns
an imgPoint with coordinates (-1, -1) if no such pixel with the
specified color exists. Complexity: O(ij)*/
imgPoint Identification::findNextOccurance(Mat img, imgPoint point, Scalar color) {
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

/*returns a set of directional pairs to use in bordering an image*/
vector<dirPair> Identification::buildDirections() {
	vector<dirPair> directions = {dirPair(DIR_RIGHT_DOWN, DIR_DOWN_LEFT),
	                              dirPair(DIR_DOWN_LEFT, DIR_LEFT_UP),
	                              dirPair(DIR_LEFT_UP, DIR_UP_RIGHT),
	                              dirPair(DIR_UP_RIGHT, DIR_RIGHT_DOWN)};
	return directions;
}