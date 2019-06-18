
/*
This class has methods that recognize patterns in an image by
looking at areas with a certain color and finding surrounding
areas that should have certain colors based on the flag pattern
*/

#include "stdafx.h"
#include "Identification.h"

/*--------------------------------------------------------------------*/
/*----------------------------Constructors----------------------------*/
/*--------------------------------------------------------------------*/

/*loads an image and begins the identification process
for the specified flagType*/
Identification::Identification(Mat img, int flagType) {
	recognizedFlag = false;
	Mat show = control(img, flagType);
	//shows the recognized image
	if (recognizedFlag) {
		imshow("Flag Recognized", show);
		waitKey(4000);
	}
}

/*--------------------------------------------------------------------*/
/*---------------------Color Bordering Functions----------------------*/
/*-------------------(areas of a particular color)--------------------*/
/*--------------------------------------------------------------------*/

/*Draws border on all areas of color by scanning horizontally or
vertically based on bool 'dir'. true = horizontal, else vertical*/
vector<vector<imgPoint>> Identification::getBorders(Mat img, Scalar color, int dist) {
	imgPoint start = imgPoint(0, 0), end = imgPoint(0, 0);
	vector<imgPoint> startSet = vector<imgPoint>();
	vector<imgPoint> endSet = vector<imgPoint>();
	bool keepLooping = true;
	while(keepLooping) {
		//gets the next starting point in an image
		start = findNextOccurance(img, end, color);
		if (start.row == -1 || start.col == -1) { keepLooping = false; }
		else {
			end = getEnd(img, color, start, dist);
			//adds the start and end points to their sets
			startSet.push_back(start); endSet.push_back(end);
		}
	}
	//combines and returns the start and end sets
	vector<vector<imgPoint>> sets = vector<vector<imgPoint>>();
	sets.push_back(startSet); sets.push_back(endSet);
	return sets;
}

/*returns the last occurance of the same pixel down a straigt line
given the margin dist. dir == true is horizontal, false = vertical*/
imgPoint Identification::getEnd(Mat img, Scalar color, imgPoint start, int dist) {
	imgPoint point = start.getCopy(), prev = imgPoint(0, 0);
	//moves in a horizontal direction
	while (point.row < img.rows) {
		prev = point;
		point = getNext(img, color, point, dist);
		if (point.row == -1 || point.col == -1) { return prev; }
	}
	return imgPoint(0, 0);
}

/*returns the next continuous pixel (within the margin dist) in the direction 'dir'*/
imgPoint Identification::getNext(Mat img, Scalar color, imgPoint start, int dist) {
	for (int i = 1; i < (dist + 1); i++) {
		if (i + start.row >= img.rows) { return imgPoint(-1, -1); }
		if (isColorEqual(img, color, imgPoint(start.row + i, start.col)))
			return imgPoint(start.row + i, start.col);
	}
	return imgPoint(-1, -1);
}

/*Returns an image with the significant areas of color boxed*/
Mat Identification::boxArea(Mat img, Scalar color, int dist) {
	return drawBox(img, COLOR_BORDERING, getAreaBox(img, color, dist));
}

/*Returns the coordinates of a box surrounding an area of color*/
vector<imgPoint> Identification::getAreaBox(Mat img, Scalar color, int dist) {
	//gets the coordinates of all the bordering pixels
	vector<vector<imgPoint>> sets = getBorders(img, color, dist);
	vector<imgPoint> startSet = sets.at(0);
	vector<imgPoint> endSet = sets.at(1);
	//eliminates outliers (noise)
	startSet = elimColOutliers(startSet, dist);
	startSet = elimRowOutliers(startSet, dist);
	endSet = elimColOutliers(endSet, dist);
	endSet = elimRowOutliers(endSet, dist);
	//extracts a suitble frame from the list of border points
	vector<imgPoint> startEndpoints = getEndpoints(startSet);
	vector<imgPoint> endEndpoints = getEndpoints(endSet);
	imgPoint startHead = startEndpoints.at(0);
	imgPoint startFoot = startEndpoints.at(1);
	imgPoint endHead = endEndpoints.at(0);
	imgPoint endFoot = endEndpoints.at(1);
	//normalizes the frame points
	int avgHead = (startHead.row + endHead.row) / 2;
	startHead = imgPoint(avgHead, startHead.col);
	endHead = imgPoint(avgHead, endHead.col);
	int avgFoot = (startFoot.row + endFoot.row) / 2;
	startFoot = imgPoint(avgFoot, startFoot.col);
	endFoot = imgPoint(avgFoot, endFoot.col);
	//returns the diagonal frame points of the rectangular frame
	return {startHead, endFoot};
}

/*takes a list of points and returns it without horizontal outliers outside range dist*/
vector<imgPoint> Identification::elimColOutliers(vector<imgPoint> points, int dist) {
	int threashold = points.size() / 2, count = 0;
	//walks down the border
	for (int i = 1; i < points.size(); i++) {
		//continuous points
		if (points.at(i).col - points.at(i - 1).col <= dist) { count++; }
		//continuity was broken
		else {
			if (count < threashold) { 
				points.erase(points.begin(), points.begin() + i);
				return elimColOutliers(points, dist); 
			}
			points.erase(points.begin() + i, points.end());
			return points;
		}
	}
	return points;
}

/*takes a list of points and returns it without vertical outliers outside range dist*/
vector<imgPoint> Identification::elimRowOutliers(vector<imgPoint> points, int dist) {
	vector<int> erase = vector<int>();
	int size = points.size(), j = 0;
	while (j < size - 1) {
		//not relativly close points
		if (abs(points.at(j).row - points.at(j + 1).row) > dist) {
			points.erase(points.begin() + j + 1);
			size--;
		}
		j++;
	}
	return points;
}

/*returns the endpoints of the 'line'ish given shape of points, including the
minimum or maximum col point, dir == true is min, dir == false is max*/
vector<imgPoint> Identification::getEndpoints(vector<imgPoint> line, bool dir) {
	int row = line.at(0).col;
	int col = line.at(0).row;
	for (int i = 0; i < line.size(); i++) {
		if (dir) { if (line.at(i).row < col) { col = line.at(i).row; } }
		else { if (line.at(i).row > col) { col = line.at(i).row; } }
	}
	imgPoint start = imgPoint(row, col);
	row = line.at(line.size() - 1).col;
	imgPoint end = imgPoint(row, col);
	return { start, end };
}

/*returns the endpoints of the 'line'ish given shape of points, the
cols endpoints are the average of all the bordering cols*/
vector<imgPoint> Identification::getEndpoints(vector<imgPoint> line) {
	int row = line.at(0).col;
	int col = 0;
	for (int i = 0; i < line.size(); i++) {
		col = col + line.at(i).row;
	}
	col = col / line.size();
	imgPoint start = imgPoint(row, col);
	row = line.at(line.size() - 1).col;
	imgPoint end = imgPoint(row, col);
	return { start, end };
}

/*--------------------------------------------------------------------*/
/*-------------------Pattern Recognition Functions--------------------*/
/*----------------(matching shapes and areas of color)----------------*/
/*--------------------------------------------------------------------*/

/*returns the percentage of pixels of a particular color in the given area*/
int Identification::getPercentage(Mat img, Scalar color, vector<imgPoint> area) {
	//sets the frame limits
	int rowStart = area.at(0).row;
	int rowEnd = area.at(1).row;
	int colStart = area.at(0).col;
	int colEnd = area.at(1).col;
	//counts the total pixels and colored pixels
	double pixelCount = 0, colorCount = 0;
	for (int j = colStart; j <= colEnd; j++)
		for (int i = rowStart; i <= rowEnd; i++) {
			if (isColorEqual(img, color, imgPoint(j, i))) {colorCount++; }
			pixelCount++;
		}
	return (colorCount / pixelCount) * 100;
}

/*takes in a rectangular area (diagonal coordinate points) and returns an area
of the same size, moved in the diretion of dir by exactly 1 area height/width*/
vector<imgPoint> Identification::getNextArea(vector<imgPoint> area, int dir) {
	//sets area variables
	int r1 = area.at(0).row;
	int r2 = area.at(1).row;
	int c1 = area.at(0).col;
	int c2 = area.at(1).col;
	int height = r2 - r1;
	int width = c2 - c1;
	//moves that area 1 spot above
	if (dir == DIR_ABOVE) {
		r1 = r1 - height;
		r2 = r2 - height;
	}
	//moves that area 1 spot to the right
	else if (dir == DIR_RIGHT) {
		c1 = c1 + width;
		c2 = c2 + width;
	}
	//moves that area 1 spot below
	else if (dir == DIR_BELOW) {
		r1 = r1 + height;
		r2 = r2 + height;
	}
	//moves that area 1 spot to the left
	else if (dir == DIR_LEFT) {
		c1 = c1 - width;
		c2 = c2 - width;
	}
	imgPoint topLeft = imgPoint(r1, c1);
	imgPoint bottomRight = imgPoint(r2, c2);
	return { topLeft, bottomRight };
}

/*returns an image filtered by the specified color/range*/
Mat Identification::isolateRange(Mat img, Scalar color, int range) {
	Scalar min = Scalar(0, 0, 0);
	Scalar max = color;
	//edits the bounds based on range
	if (color[0] == 0) { max[0] = range / 3; }
	else { min[0] = range; }
	if (color[1] == 0) { max[1] = range / 3; }
	else { min[1] = range; }
	if (color[2] == 0) { max[2] = range / 3; }
	else { min[2] = range; }
	cout << "min " << min[0] << " " << min[1] << " " << min[2] << endl;
	cout << "max " << max[0] << " " << max[1] << " " << max[2] << endl;
	return Preprocessing::isolateColor(img, color, max, min);
}

/*takes a list of percentages of colored pixels in an area and a min
percent requriement and returns if that requeirement is met*/
bool Identification::isEnoughPixels(vector<int> percents, int percent) {
	cout << "-----------------------------------------------------------" << endl;
	for (int i = 0; i < percents.size(); i++) {
		cout << "percent found: " << percents.at(i) << " minimum: " << percent << endl;
		if (percents.at(i) < percent)
			return false;
	}
	return true;
}

/*returns an set of filtered images (heavily filtered - > not 
too heavily filtered*/
vector<Mat> Identification::getFilters(Mat img, Scalar color) {
	int filtersAmount = FILTER_TIMES;
	vector<Mat> filters = vector<Mat>();
	//obtains the different filters
	for (int i = 1; i < filtersAmount + 1; i++) {
		filters.push_back(isolateRange(img, color, i * 10));
	}
	return filters;
}

/*--------------------------------------------------------------------*/
/*---------------------Flag Identification Methods--------------------*/
/*--------------------------------------------------------------------*/

//manages the control flow of the program
Mat Identification::control(Mat img, int flagType) {
	int dist = PIXEL_DIST_ALLOWANCE;
	//produces a list of filters to recognize the flag in
	vector<Mat> filters = getFilters(img, COLOR_RED);
	Mat otherFilters;
	if (flagType == CHARLIE) 
		otherFilters= Preprocessing::isolateColors(img, COLORS_CHARLIE_R);
	else if (flagType == ECHO)
		otherFilters = Preprocessing::isolateColors(img, COLORS_ECHO_R);
	else
		otherFilters = Preprocessing::isolateColors(img, COLORS_UNIFORM_R);
	//imshow("filters", otherFilters); waitKey(2000);
	for (int i = 0; i < filters.size(); i++) {
		filters[i] = Preprocessing::addImg(filters[i], otherFilters);
		//imshow("filters", filters[i]); waitKey(500);
	}
	/*applies the recognition process to the filtered images*/
	for (int i = filters.size() - 1; i >= 0; i--) {
		Mat recognized;
		if (flagType == CHARLIE) { recognized = Charlie(img, filters[i], dist); }
		if (flagType == ECHO) { recognized = Echo(img, filters[i], dist); }
		if (flagType == UNIFORM) { recognized = Uniform(img, filters[i], dist); }
		//checks for a recognized flag
		if (recognizedFlag) { return recognized; }
	}
	return img;
}

/*Using many helper methods, attempts to recognize the flag Charlie*/
Mat Identification::Charlie(Mat org, Mat filtered, int dist) {

	Scalar red = COLOR_RED;
	Scalar blue = COLOR_BLUE;
	Scalar white = COLOR_WHITE;

	vector<int> percentages = vector<int>();

	//middle stripe
	vector<imgPoint> area3 = getAreaBox(filtered, red, dist);
	percentages.push_back(getPercentage(filtered, red, area3));
	imshow("flag", boxArea(filtered, red, dist)); waitKey(2000);

	//left middle stripe
	vector<imgPoint> area2 = getNextArea(area3, DIR_LEFT);
	percentages.push_back(getPercentage(filtered, white, area2));

	//left stripe
	vector<imgPoint> area1 = getNextArea(area2, DIR_LEFT);
	percentages.push_back(getPercentage(filtered, blue, area1));

	//right middle stripe
	vector<imgPoint> area4 = getNextArea(area3, DIR_RIGHT);
	percentages.push_back(getPercentage(filtered, white, area4));

	//right stripe
	vector<imgPoint> area5 = getNextArea(area4, DIR_RIGHT);
	percentages.push_back(getPercentage(filtered, blue, area5));

	/*checks if the percentages of color in the surrounding areas are
	enough. i.e. if the pattern has been sufficiently recognized*/
	if (isEnoughPixels(percentages, MIN_PERCENT)) {
		recognizedFlag = true;
		return drawBorders(org, COLOR_BORDERING, { area1.at(0), area5.at(1) });
	}
	else { recognizedFlag = false; }
	return org;
}

/*Using many helper methods, attempts to recognize the flag Echo*/
Mat Identification::Echo(Mat org, Mat filtered, int dist) {

	Scalar red = COLOR_RED;
	Scalar blue = COLOR_BLUE;

	vector<int> percentages = vector<int>();

	//lower red stripe
	vector<imgPoint> area2 = getAreaBox(filtered, red, dist);

	percentages.push_back(getPercentage(filtered, red, area2));
	imshow("flag", boxArea(filtered, red, dist)); waitKey(2000);

	//upper blue stripe
	vector<imgPoint> area1 = getNextArea(area2, DIR_ABOVE);
	percentages.push_back(getPercentage(filtered, blue, area1));

	/*checks if the percentages of color in the surrounding areas are
	enough. i.e. if the pattern has been sufficiently recognized*/
	if (isEnoughPixels(percentages, MIN_PERCENT)) {
		recognizedFlag = true;
		return drawBorders(org, COLOR_BORDERING, { area1.at(0), area2.at(1) });
	}
	else { recognizedFlag = false; }
	return org;
}

/*Using many helper methods, attempts to recognize the flag Uniform*/
Mat Identification::Uniform(Mat org, Mat filtered, int dist) {

	Scalar red = COLOR_RED;
	Scalar white = COLOR_WHITE;

	vector<int> percentages = vector<int>();

	//upper left box
	vector<imgPoint> area1 = getAreaBox(filtered, red, dist);
	percentages.push_back(getPercentage(filtered, red, area1));
	imshow("flag", boxArea(filtered, red, dist)); waitKey(2000);

	//upper right box
	vector<imgPoint> area2 = getNextArea(area1, DIR_RIGHT);
	percentages.push_back(getPercentage(filtered, white, area2));

	//lower left box
	vector<imgPoint> area3 = getNextArea(area1, DIR_BELOW);
	percentages.push_back(getPercentage(filtered, white, area3));

	//lower right box
	vector<imgPoint> area4 = getNextArea(area3, DIR_RIGHT);
	percentages.push_back(getPercentage(filtered, red, area4));

	/*checks if the percentages of color in the surrounding areas are
	enough. i.e. if the pattern has been sufficiently recognized*/
	if (isEnoughPixels(percentages, MIN_PERCENT)) {
		recognizedFlag = true;
		return drawBorders(org, COLOR_BORDERING, { area1.at(0), area4.at(1) });
	}
	else { recognizedFlag = false; }
	return org;
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
	for (int j = point.col; j < img.rows; j++) {
		if (j > point.col) { i = 0; }
		while (i < img.cols) {
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

/*--------------------------------------------------------------------*/
/*-------------------------Image Drawing Methods----------------------*/
/*--------------------------------------------------------------------*/

/*paints the given pixel the given color in the given image if possible*/
void Identification::paintPixel(Mat img, Scalar color, imgPoint point) {
	if (point.row < img.rows && point.row >= 0 &&
		point.col < img.cols && point.col >= 0) {
		Scalar paint = color;
		Vec3b paintColor = Vec3b(paint.val[0], paint.val[1], paint.val[2]);
		img.at<Vec3b>(point.col, point.row) = paintColor;
	}
}

/*Returns an image with the location highlighted*/
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

/*Returns an image with a line drawn between points a and b*/
Mat Identification::drawLine(Mat img, Scalar color, imgPoint a, imgPoint b) {
	Mat out = img.clone();
	Vec3b drawColor = Vec3b(color.val[0], color.val[1], color.val[2]);
	//vertical line
	if (a.row == b.row) {
		int first = a.col;
		int last = b.col;
		if (first > last) {
			first = last;
			last = a.col;
		}	
		for (int j = first; j <= last; j++)
			out.at<Vec3b>(a.row, j) = drawColor;
	}
	//horizontal line
	else if (a.col == b.col) {
		int first = a.row;
		int last = b.row;
		if (first > last) {
			first = last;
			last = a.row;
		}
		for (int i = first; i <= last; i++)
			out.at<Vec3b>(i, a.col) = drawColor;
	}
	//line neither vertical or horizontal
	return out;
}

/*Returns an image with a box drawn around the area*/
Mat Identification::drawBox(Mat img, Scalar color, vector<imgPoint> area) {
	//sets area variables
	int r1 = area.at(0).row;
	int r2 = area.at(1).row;
	int c1 = area.at(0).col;
	int c2 = area.at(1).col;
	imgPoint topLeft = imgPoint(r1, c1);
	imgPoint topRight = imgPoint(r1, c2);
	imgPoint bottomRight = imgPoint(r2, c2);
	imgPoint bottomLeft = imgPoint(r2, c1);
	//draws box
	Mat out = drawLine(img, color, topLeft, topRight);
	out = drawLine(out, color, topRight, bottomRight);
	out = drawLine(out, color, bottomRight, bottomLeft);
	out = drawLine(out, color, bottomLeft, topLeft);
	return out;
}

/*Returns an image with a thick box drawn around the area*/
Mat Identification::drawBorders(Mat img, Scalar color, vector<imgPoint> area) {
	Mat out = drawBox(img, color, area);
	imgPoint topLeft = imgPoint(area.at(0).row - 1, area.at(0).col - 1);
	imgPoint bottomRight = imgPoint(area.at(1).row + 1, area.at(1).col + 1);
	out = drawBox(out, color, {topLeft, bottomRight});
	return out;
}