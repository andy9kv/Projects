
/*
* Andrew Morato
* Triangle.cpp
* 02/23/17
*/

#include "Triangle.h"

//Constructors------------------

/*NOTE: All constructors in the parent class, Polygon, create the smallest possible polygon, a triangle, therefore all the Triangle constructors simply call their equivalent Polygon constructors*/

/*Default Constructor which initializes a triangle and sets color to black*/
Triangle::Triangle() {Poly();}

/*Constructor that initializes a triangle and sets color to 'color'*/
Triangle::Triangle(vec3& color) {
	Poly();
	this->color = vec3(color[0], color[1], color[2]);
}

/*Constructor that sets the points to 'points' and color to black. If points' size < 3, calls the default constructor*/
Triangle::Triangle(std::list<vec3> &points) {
	assert(points.size() == 3);
	setPoints(points);
	color = vec3();
}

/*Constructor that sets the points to 'points' and color to 'color'. If points' size < 3, calls the default constructor*/
Triangle::Triangle(std::list<vec3> &points, vec3& color){
	assert(points.size() == 3);
	Poly(points, color);
}