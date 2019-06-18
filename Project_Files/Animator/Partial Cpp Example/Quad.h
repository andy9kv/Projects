
/*
* Andrew Morato
* Quad.h
* 02/23/17
*/

//Preprocessor Directives
#pragma once
#include "Poly.h"

class Quad: public Poly {

	/*Inherited:
		list<vec3> points
		vec3 color
	*/

public:
	//Constructors---------------
	Quad();
	Quad(vec3& color);
	Quad(std::list<vec3> &points);
	Quad(std::list<vec3> &points, vec3& color);
};