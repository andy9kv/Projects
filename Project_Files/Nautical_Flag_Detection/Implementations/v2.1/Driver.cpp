
#include "stdafx.h"
#include "Driver.h"

int main() {

	//loads sample flags
	Mat sample1 = imread("Samples/Charlie_1.jpg");
	Mat sample2 = imread("Samples/Echo_1.png");
	Mat sample3 = imread("Samples/Echo_1.jpg");
	Mat sample4 = imread("Samples/Uniform_1.jpg");

	Identification::Identification(sample1, CHARLIE);
	Identification::Identification(sample2, ECHO);
	Identification::Identification(sample3, ECHO);
	Identification::Identification(sample4, UNIFORM);
	return 0;
}