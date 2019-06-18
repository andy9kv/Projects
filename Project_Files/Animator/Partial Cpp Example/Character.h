
/*
* Andrew Morato
* Character.h
* 02/23/17
*/

//Preprocessor Directives
#pragma once
#include "Poly.h"
#include "SceneGraph.h"
#include "Triangle.h"
#include "Quad.h"

class Character {

private:

	SceneGraph *animal;
	Node *rootTrans;

public:

	Character();
	void createSceneGraph();
	std::list<Poly> buildSceneGraph();
	void printCharsVertecies();

	Node* getRoot();
	Node* getRootTrans();
	void rotate(float angle);
	void scale(float x, float y);
	void translate(float x, float y);
};