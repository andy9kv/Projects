
/*
* Andrew Morato
* Node.h
* 02/23/17
*/

//Preprocessor Directives
#pragma once
#include "Poly.h"
#include <string>

class Node {

private:
	std::string name;
	Poly *p; //holds the shape
	std::list<mat3> t; //hold the transformations
	std::list<Node*> children;//holds the children
	Node *parent = NULL;
	bool isThisAShape = false;

public:
	//Constructors---------------
	Node();
	Node(Poly p);
	Node(std::list<Node*> children);

	//Methods--------------------
	std::string getName();
	void setName(std::string name);
	Node getChild(int index);
	std::list<Node*> getChildren();
	Poly getPolygon();
	std::list<mat3> getTrans();
	mat3 getTrans(int index);
	float getRotationAngle();
	float getXScalar();
	float getYScalar();
	float getXTranslation();
	float getYTranslation();
	Node* getParent();
	void setParent(Node *n);
	int howManyChildren();
	bool tellMeIfThisIsAShape();
	void appendChild(Node *n);
	void clearChildren();
	void fillT();
	void print();
	void suicide();

	//Transformation Methods-----
	/*The following methods adds/edits transformations to list t of transformations*/
	void rotate(float angle);
	void scale(float x, float y);
	void translate(float x, float y);
	void editTransformation(mat3 m, int index);
	Node* transform(std::list<mat3> t);
};