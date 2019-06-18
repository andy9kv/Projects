
/*
* Andrew Morato
* SceneGraph.h
* 02/23/17
*/

//Preprocessor Directives
#pragma once
#include "Node.h"

class SceneGraph {

private:
	Node *root;
	std::list<Poly> transPolys; /*holds finished transformed group of Polygons*/
	//Helper Methods-------------
	void buildGraph(Node *n);
	void transformLeavesFromBelow(Node *parent, Node *n);

public:
	//Constructors---------------
	SceneGraph();

	//Methods--------------------
	Node* getRoot();
	std::list<Poly> getTransformedPolys();
	void addChild(Node *addTo, Node *n);
	void insertNode(Node *addTo);
	void deleteNode(Node *n);
	void buildGraph();
	void printPolygons();
};
