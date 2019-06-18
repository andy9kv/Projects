
/*
* Andrew Morato
* SceneGraph.cpp
* 02/23/17
*/

#include "SceneGraph.h"

//Constructors---------------

/*Default constructor that sets root to a node with the identity matrix as it's only transformation*/
SceneGraph::SceneGraph() {
	root = new Node();//makes root identity matrix
}

//Methods--------------------

Node* SceneGraph::getRoot() {return root;}

std::list<Poly> SceneGraph::getTransformedPolys() { return transPolys; }

/*Adds child 'n' to the node 'addTo'. Great for adding leafs*/
void SceneGraph::addChild(Node *addTo, Node *n) { 
	(*addTo).appendChild(n);
	(*n).setParent(addTo);
}

/*Creates a new Node n, sets addTo's children as n's children, removes all of addTo's chilren, sets the parent of all of addTo's children to n, and finally makes n addTo's only child*/
void SceneGraph::insertNode(Node *addTo) {
	Node *n = new Node(addTo->getChildren());
	addTo->clearChildren();
	for (std::list<Node*>::iterator iter = n->getChildren().begin(); iter != n->getChildren().end(); iter++)//sets the parents
		(*iter)->setParent(n);
	addTo->appendChild(n);
}

/*Deletes a node if it is not the root. If the node had children, the children are transfered to the parent*/
void SceneGraph::deleteNode(Node *n) {
	assert(n->getParent() == NULL);//if root
	Node parent = *(n->getParent());
	for (int i = 0; i < (*n).howManyChildren(); i++) {
		/*The children's grandfather is now their parent*/
		n->getChild(i).setParent(parent.getParent());
		parent.appendChild(&(*n).getChild(i));
	}
	(*n).suicide();//kills the node to be deleted before forgetting about its pointer
}

/*Does a recursive traversal of the tree, multiplying each of the polygon's vertecies at each leaf node with the transformations in that node, then in the parent nodes, all the way up to the identity matrix in the root. This is achieved with a PostOrder Traversal, following the following pseudo code for a Binary Tree (This is not a binary tree but the general idea is the same):
	poTraverse(Node)
		poTraverse(left child)
		poTraverse(right child)
		//Execute action*/
void SceneGraph::buildGraph() {
	transPolys.clear();//clears the transforms list
	buildGraph(root);
}

/*helper method for building the graph*/
void SceneGraph::buildGraph(Node *n) {
	(*n).print();
	//recursivly calls buildGraph on all children in the list
	for (int i = 0; i < (*n).howManyChildren(); i++)
		buildGraph(&(*n).getChild(i));
	if ((*n).tellMeIfThisIsAShape())
		transformLeavesFromBelow(n, n);
}

/*helper method for building the graph that ensures the leaves below or at the particular node get multiplied by the transformations this node 'n' has*/
void SceneGraph::transformLeavesFromBelow(Node *parent, Node *n) {
		Node *transN = (*n).transform((*parent).getTrans());
		/*end the recursion & add the transformed polygon to the list. if the parent is NULL, this is the root*/
		if ((*parent).getParent() == NULL)
			transPolys.push_back((*transN).getPolygon());/*adds finished Poly to the list of finished Polys*/
		else /*keep recursing to the parent*/
			transformLeavesFromBelow((*parent).getParent(), transN);
}

/*Prints the list of polygons in the scene graph*/
void SceneGraph::printPolygons() {
	std::cout << "---------- Polygons ----------" << std::endl;
	std::list<Poly>::iterator iter = transPolys.begin();
	//if(transPolys.begin() == transPolys.end())
		//std::cout << "Beads?" << std::endl;
	while (iter != transPolys.end()) {
		//std::cout << "Bees?" << std::endl;
		(*iter).print();
		iter++;
	}
	std::cout << "------------ End ------------" << std::endl;
}
