
/*
* Andrew Morato
* Character.cpp
* 02/23/17
*/

#include "Character.h"

/*This Character is a Jaguar*/

/*Constructer initializes the scene graph and puts all the transformation nodes/shape nodes in place*/
Character::Character(){
	animal = new SceneGraph();
	createSceneGraph();
}

//Methods------------------------

/*The Character is a Jaguar*/
void Character::createSceneGraph() {

	/*instantiates the RootTransformations node, any transformation (rotation, scaling, or translation) applied to the entire animal goes here*/
	rootTrans = new Node();
	(*rootTrans).scale(2, 2);
	(*rootTrans).translate(-1, -1);
	(*rootTrans).setName("rootTrans");
	animal->addChild(getRoot(), rootTrans);


	/*children of the RootTransformations node (all other nodes)----------------------------*/

	/*Adds head & head's children. Any transformation to the entire head (face and both ears) goes here*/
	Node *head = new Node();
	(*head).translate(2, 0.7);//moves entire head 2.3 right and 1 above
	(*head).setName("head");
	(*animal).addChild(rootTrans, head);


	/*children of the head node---------------*/

		//Red Hexagon, face
		std::list<vec3> hex;
		hex.push_back(vec3()); hex.push_back(vec3(0.5, 0.25, 0));
		hex.push_back(vec3(0.5, 0.75, 0)); hex.push_back(vec3(0, 1, 0));
		hex.push_back(vec3(-0.5, 0.75, 0)); hex.push_back(vec3(-0.5, 0.25, 0));
		Poly *hexagon = new Poly(hex, vec3(1, 0 ,0));
		Node *face = new Node(*hexagon);
		(*face).setName("face");
		(*animal).addChild(head, face);

		//Purpule Triangle, ear1
		std::list<vec3> tri;
		tri.push_back(vec3()); tri.push_back(vec3(0.25, 0, 0)); tri.push_back(vec3(0.125, 0.25, 0));
		Poly *triangle = new Poly(tri, vec3(1, 0, 1));
		Node *ear1 = new Node(*triangle);
		(*ear1).translate(0.27, 0.85); (*ear1).rotate(-22);
		(*ear1).setName("ear1");
		(*animal).addChild(head, ear1);

		//Purpule Triangle, ear2
		Node *ear2 = new Node(*triangle);
		(*ear2).translate(-0.5, 0.75); (*ear2).rotate(25);
		(*ear2).setName("ear2");
		(*animal).addChild(head, ear2);



	/*Adds Upper Legs & Upper Legs's children. Any transformation to both the upper and lower legs goes here*/
	Node *uLegs = new Node();
	(*uLegs).translate(0, -0.5);//moves all the leg limbs down by 2
	(*uLegs).setName("legs");
	(*animal).addChild(rootTrans, uLegs);

	/*children of the uLegs node----------------*/

		//Adds 4 upper legs
		std::list<vec3> quad2;
		quad2.push_back(vec3()); quad2.push_back(vec3(0.3, 0, 0));
		quad2.push_back(vec3(0.3, 0.7, 0)); quad2.push_back(vec3(0, 0.7, 0));
		Poly *q1 = new Poly(quad2, vec3(0, 1, 0));//Green Legs
		//Adds 4 upper leg nodes, each with q1
		Node *ul1 = new Node(*q1);
		Node *ul2 = new Node(*q1); (*ul2).translate(0.5, 0);
		Node *ul3 = new Node(*q1); (*ul3).translate(1, 0);
		Node *ul4 = new Node(*q1); (*ul4).translate(1.5, 0);
		//Adds the upper legs to the character (scene graph)
		(*ul1).setName("upperLeg1"); (*animal).addChild(uLegs, ul1);
		(*ul2).setName("upperLeg2"); (*animal).addChild(uLegs, ul2);
		(*ul3).setName("upperLeg3"); (*animal).addChild(uLegs, ul3);
		(*ul4).setName("upperLeg4"); (*animal).addChild(uLegs, ul4);


		/*children of each of the upper legs----------------*/

			std::list<vec3> quad3;
			quad3.push_back(vec3()); quad3.push_back(vec3(0.3, 0, 0));
			quad3.push_back(vec3(0.3, 0.7, 0)); quad3.push_back(vec3(0, 0.7, 0));
			Poly *q3 = new Poly(quad3, vec3(0, 1, 1));//Turqouise Legs
			/*each node llx is lower leg number x, correspoinding to the upper legs ulx*/
			Node *ll1 = new Node(*q3); (*ll1).translate(-0.2, -0.4); (*ll1).rotate(-15);
			Node *ll2 = new Node(*q3); (*ll2).translate(-0.2, -0.4); (*ll2).rotate(-15);
			Node *ll3 = new Node(*q3); (*ll3).translate(-0.2, -0.4); (*ll3).rotate(-15);
			Node *ll4 = new Node(*q3); (*ll4).translate(-0.2, -0.4); (*ll4).rotate(-15);
			//Adds the lower legs to the character (scene graph)
			(*ll1).setName("lowerLeg1"); (*animal).addChild(ul1, ll1);
			(*ll2).setName("lowerLeg2"); (*animal).addChild(ul2, ll2);
			(*ll3).setName("lowerLeg3"); (*animal).addChild(ul3, ll3);
			(*ll4).setName("lowerLeg4"); (*animal).addChild(ul4, ll4);


	/*Adds a torso. The transformations for the torso should only go in it's parent's node, the rootTrans node*/
		std::list<vec3> quad1;
		quad1.push_back(vec3()); quad1.push_back(vec3(2, 0, 0));
		quad1.push_back(vec3(2, 0.8, 0)); quad1.push_back(vec3(0, 0.8, 0));
		Poly *q = new Poly(quad1, vec3(0, 0, 1));//blue torso
		Node *torso = new Node(*q);
		(*torso).setName("torso");
		(*animal).addChild(rootTrans, torso);


	/*Adds tail, The tail has no children*/
		std::list<vec3> polyline;
		polyline.push_back(vec3(0, 0.5, 0)); polyline.push_back(vec3(-0.6, 0.5, 0));
		polyline.push_back(vec3(-1.1, 0.8, 0)); polyline.push_back(vec3(-0.6, 1.4, 0));
		polyline.push_back(vec3(-0.5, 1.5, 0)); polyline.push_back(vec3(-0.55, 1.3, 0));
		polyline.push_back(vec3(-1, 0.8, 0)); polyline.push_back(vec3(-0.5, 0.6, 0));
		Poly *pl = new Poly(polyline, vec3(0, 0, 0));//black tail
		(*pl).setPolyline();
		Node *tail = new Node(*pl);
		(*tail).setName("tail");
		(*animal).addChild(rootTrans, tail);

	/*This line is for this class to know which SceneGraph to build when buildSceneGraph is called*/
	this->animal = animal;//sets jaguar to global animal
}

/*Builds the SceneGraph, carrying out the transformations, and returns a list of the transformed polygons*/
std::list<Poly> Character::buildSceneGraph(){
	(*animal).buildGraph();
	return (*animal).getTransformedPolys();
}

/*Prints out the characters's vertecies*/
void Character::printCharsVertecies() {(*animal).printPolygons();}

Node* Character::getRoot() { return animal->getRoot(); }
Node* Character::getRootTrans() { return rootTrans; }

//transformation methods
void Character::rotate(float angle) {(*rootTrans).rotate(angle);}
void Character::scale(float x, float y) {(*rootTrans).scale(x, y);}
void Character::translate(float x, float y) {(*rootTrans).translate(x, y);}