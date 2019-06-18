
/*
* Andrew Morato
* Poly.cpp
* 02/23/17
*/

#include "Poly.h"

//Constructors------------------

/*Default Constructor that creates the smallest possible polygon, a 3-vertex polygon(triangle), and sets color to black*/
Poly::Poly() {
	points.push_back(vec3(0, 0, Z_VALUE));
	points.push_back(vec3(1, 0, Z_VALUE));
	points.push_back(vec3(0, 1, Z_VALUE));
	color = vec3();
}

/*Constructor that creates a 3-vertex polygon, sets color to 'color'*/
Poly::Poly(vec3& color) {
	Poly();
	this->color = vec3(color[0], color[1], color[2]);
}

/*Constructor sets points to 'points', ensures a polygon is possible*/
Poly::Poly(std::list<vec3>& points) {
	assert(points.size() > 2);
	setPoints(points);
	color = vec3();
}

/*Constructor sets points to 'points' and sets color to 'color'. Also ensures that a polygon is possible*/
Poly::Poly(std::list<vec3>& points, vec3& color) {
	assert(points.size() > 2);
	setPoints(points);
	this->color = vec3(color[0], color[1], color[2]);
}

/*Constructor sets points to 'points', sets color to 'color', and sets if it is a polyline. Also ensures that a polygon is possible*/
Poly::Poly(std::list<vec3>& points, vec3& color, bool isPolyline) {
	assert(points.size() > 2);
	setPoints(points);
	this->color = vec3(color[0], color[1], color[2]);
	this->isPolyline = isPolyline;
}

//Methods----------------------

/*Return true if this polygon is actually a polyline*/
bool Poly::isThisAPolyline() {return isPolyline;}
void Poly::setPolyline() { isPolyline = true; }

/*Returns the number of vertecies in the Polygon*/
int Poly::howManyPoints() {return points.size();}

/*Returns the color*/
vec3 Poly::getColor() {return color;}

/*Sets the color*/
void Poly::setColor(vec3 color) {
	this->color = vec3(color[0], color[1], color[2]);
}

/*Returns the point at index's position in the polygon. Counting starts at 0*/
vec3 Poly::getPoint(int index){
	assert(index >= 0 && index < points.size());
	std::list<vec3>::iterator iter = points.begin();
	/*Not a great way to get to an element at an index but works*/
	for (int i = 0; iter != points.end(); ++iter) {
		if (i == index)
			break;
		i++;
	}
	return (*iter);
}

/*Returns the list of points in the polygon*/
std::list<vec3> Poly::getPoints() {return points;}

/*Sets 'point' at the index's position in the polygon. Counting starts at 0*/
void Poly::setPoint(int index, vec3& point) {
	assert(index >= 0 && index < points.size());
	std::list<vec3>::iterator iter = points.begin();
	/*Not a great way to get to an element at an index but works*/
	for (int i = 0; iter != points.end(); ++iter) {
		if (i == index) {
			points.insert(iter, point);
			points.erase(iter);
			break;
		}
		i++;
	}
}

/*Helper method that initializes 'points' to the parameter points*/
void Poly::setPoints(std::list<vec3>& points) {
	this->points.clear();
	std::list<vec3>::iterator iter = points.begin();
	while (iter != points.end()) {
		/*Currently, the z value is hard coded to Z_VALUE which is 1 because it enables the 2d transformations to work properly in the math libraries*/
		this->points.push_back(vec3((*iter)[0], (*iter)[1], Z_VALUE));
		iter++;
	}
}

/*Simple method that prints the vectors and the color of the polygon*/
void Poly::print() {
	std::cout << "Color: "; color.print();
	std::list<vec3>::iterator iter = points.begin();
	std::cout << "Points:" << std::endl;
	for (iter; iter != points.end(); ++iter)
		(*iter).print();
	std::cout << std::endl;
}