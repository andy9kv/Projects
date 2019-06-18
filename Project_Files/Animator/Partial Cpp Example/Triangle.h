
/*
* Andrew Morato
* Triangle.h
* 02/23/17
*/

//Preprocessor Directives
#pragma once
#include "Poly.h"

class Triangle: public Poly {

	/*Inherited:
	list<vec3> points
	vec3 color
	*/

public:
	//Constructors---------------
	Triangle();
	Triangle(vec3& color);
	Triangle(std::list<vec3> &points);
	Triangle(std::list<vec3> &points, vec3& color);
};