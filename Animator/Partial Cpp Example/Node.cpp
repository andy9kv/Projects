
/*
* Andrew Morato
* Node.cpp
* 02/23/17
*/

#include "Node.h"

//Constructors---------------

/*Default constructor that initializes Polygon p*/
Node::Node() {
	p = new Poly();
	fillT();
}


/*Constructer that sets Polygon p to 'p'*/
Node::Node(Poly p) {
	this->p = new Poly(p.getPoints(), p.getColor(), p.isThisAPolyline());
	fillT();
	isThisAShape = true;
}


/*Constructor that sets children to 'children'*/
Node::Node(std::list<Node*> children) {
	fillT();
	this->children = children;
}

//Methods--------------------

std::string Node::getName() { return name; }
void Node::setName(std::string name) { this->name = name; }

/*Returns the Node at index's position in the list of children. Counting starts at 0*/
Node Node::getChild(int index) {
	assert(index >= 0 && index < children.size());
	std::list<Node*>::iterator iter = children.begin();
	/*Not a great way to get to an element at an index but works*/
	for (int i = 0; iter != children.end(); ++iter) {
		if (i == index)
			break;
		i++;
	}
	return (**iter);
}

std::list<Node*> Node::getChildren() {return children;}
Poly Node::getPolygon() { return *p; }

std::list<mat3> Node::getTrans() { return t; }

//returns the translation matrix at specified index
mat3 Node::getTrans(int index) {
	std::list<mat3>::iterator iter= t.begin();
	if (index == 1 || index == 2)/*iterates the list once if index is at least 1*/
		iter++;
	if (index == 2)/*iterates the list again if index is 2*/
		iter++;
	return (*iter);
}

//uses arc cosine and puts the result back into degrees
float Node::getRotationAngle() {
	float cosVal = getTrans(0).getValueAtIndex(0); 
	return (acos(cosVal)/PI) * 180;
}
float Node::getXScalar() { return getTrans(1).getValueAtIndex(0); }
float Node::getYScalar() { return getTrans(1).getValueAtIndex(4); }
float Node::getXTranslation() { return getTrans(2).getValueAtIndex(2); }
float Node::getYTranslation() { return getTrans(2).getValueAtIndex(5); }


Node* Node::getParent() { return parent; }
void Node::setParent(Node *n) { parent = n; }

int Node::howManyChildren() { return children.size(); }
bool Node::tellMeIfThisIsAShape() { return isThisAShape; }

/*Appends a node to the list of children*/
void Node::appendChild(Node *n) {children.push_back(n);}

/*Clears the children of node 'n'*/
void Node::clearChildren() {
	std::list<Node*> newList;
	children = newList;
}

/*fills list t with identity matricies*/
void Node::fillT(){
	t.push_back(mat3::identity());
	t.push_back(mat3::identity());
	t.push_back(mat3::identity());
}

/*Prints polygon (points & color) and transformations*/
void Node::print() {
	(*p).print();//Prints polygon
	//Prints Transformations
	std::list<mat3>::iterator iter = t.begin();
	std::cout << "Rotation: ---" << std::endl; (*iter).print();
	iter++; std::cout << "Scaler: ---" << std::endl; (*iter).print();
	iter++; std::cout << "Translation: ---" << std::endl; (*iter).print();
}

void Node::suicide() {delete(p);} /*Kills only potential memory leak*/

//Transformation Methods-----

/*edits the third element of list t with rotation r*/
void Node::rotate(float angle) {
	editTransformation(mat3::rotation2D(angle), 0);
}

/*edits the second element of list t with scaler s*/
void Node::scale(float x, float y) { 
	editTransformation(mat3::scale2D(x, y), 1);
}

/*edits the first element of list t with translation t*/
void Node::translate(float x, float y) { 
	editTransformation(mat3::translation2D(x, y), 2);
}

/*Edits the element at 'index' in the list of matricies to 'm'*/
void Node::editTransformation(mat3 m, int index) {
	std::list<mat3>::iterator iter= t.begin();
	if (index == 1 || index == 2)/*iterates the list once if index is at least 1*/
		iter++;
	if (index == 2)/*iterates the list again if index is 2*/
		iter++;
	/*multiplies m with the current transformation, combining them*/
	(*iter) = (*iter) * m;
}

/*Goes through each vertex in this->p and multiplies it with the 3 transformation matricies in 't', creating a new polygon, which it returns*/
Node* Node::transform(std::list<mat3> t) {
	std::list<vec3> newPolygon;
	for (int i = 0; i < (*p).howManyPoints(); i++) {
		std::list<mat3>::iterator iter = t.begin();
		vec3 newVec = (*iter) * (*p).getPoint(i); iter++;
		newVec = (*iter) * newVec; iter++;
		newVec = (*iter) * newVec;
		newPolygon.push_back(newVec);
	}
	Poly p1 = Poly(newPolygon, (*p).getColor(), (*p).isThisAPolyline());
	return new Node(p1);
}
