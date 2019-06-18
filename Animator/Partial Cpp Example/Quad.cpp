
/*
* Andrew Morato
* Quad.cpp
* 02/23/17
*/

#include "Quad.h"

//Constructors------------------

/*Default Constructor that creates a quadrilateral and sets color to black*/
Quad::Quad(){
	Poly();/*Initializes a polygon with (0,0,0),(1,0,0),(0,1,0) & sets color to black*/
	points.push_back(vec3(1, 1, Z_VALUE));
}

Quad::Quad(vec3& color){
	Poly();/*Initializes a polygon with (0,0,0),(1,0,0),(0,1,0) & sets color to 'color'*/
	points.push_back(vec3(1, 1, Z_VALUE));
	this->color = vec3(color[0], color[1], color[2]);
}

/*Constructor sets points to 'points' and sets color to black, ensures a Quad has 4 vertecies*/
Quad::Quad(std::list<vec3> &points){
	assert(points.size() == 4);
	setPoints(points);
	color = vec3();
}

/*Constructor sets points to 'points' and sets color to 'color', ensures a Quad has 4 vertecies*/
Quad::Quad(std::list<vec3> &points, vec3& color){
	assert(points.size() == 4);
	Poly(points, color);
}