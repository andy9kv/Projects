
/*
* Andrew Morato
* Poly.h
* 02/23/17
*/

//Preprocessor Directives
#pragma once
#include "mat3.h"
#include <list>
#define Z_VALUE 1

class Poly {

protected:
	/*points holds the vertecies of the polygon*/
	std::list<vec3> points;
	bool isPolyline = false; /*false if it is a polygon*/
	vec3 color;/*holds the RGB value*/
	/*helper method that sets 'points', called from constructor*/
	void setPoints(std::list<vec3>& points);

public:
	//Constructors-----------------
	Poly();
	Poly(vec3& color);
	Poly(std::list<vec3>& points);
	Poly(std::list<vec3>& points, vec3& color);
	Poly(std::list<vec3>& points, vec3& color, bool isPolyline);

	//Methods----------------------
	bool isThisAPolyline();
	void setPolyline();
	int howManyPoints();
	vec3 getColor();
	void setColor(vec3 color);
	vec3 getPoint(int index);
	std::list<vec3> getPoints();
	void setPoint(int index, vec3& point);
	void print();

};