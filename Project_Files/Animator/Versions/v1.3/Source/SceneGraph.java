
/*
* Andy Morato
* Scene Graph class holds a 'scene' (basically several polygons
* transformed in a specified way). A scene is represented in a 
* tree like fashion, with a root node. As specified in the Node
* class, each node has transformations, such that when a scene
* is 'built' the transformations of any node are applied to all
* its decendents, ending with fully transformed leaves(shapes).
* This process is carried out in the buildScene method and its
* helper functions.
* Created July 31th, 2017
*/

import java.util.ArrayList;

public class SceneGraph{

/*-----Instance Variables-----*/

private Node root;
/*'parent' data structure of this scene graph. This is
changed when this scene graph become part of a Flipbook.
Used to request uniqueIDs for new nodes*/
private Flipbook flipbook;


/*---------------------Constructors-----------------------*/

/*constructor that sets the specifed root as this instance's
root node*/
public SceneGraph(Node root){ this.root = new Node(root);}

/*copy constructor that copies the entrie tree stemming
from the root*/
public SceneGraph(SceneGraph original){
	this(original.copyTree(null, original.getRoot()));
}

/*---------------------Tree Management--------------------*/
/*These node management functions are not in the Node class
because they escape the scope of a single node, requiring
action from the node's parents and children*/

/*appropriately appends the specified node n to the children
of the specified parent*/
public void addChild(Node parent, Node n){
	parent.appendChild(n);
	n.setParent(parent);
}

/*inserts n between the specified parent and its children*/
public void insertNode(Node parent, Node n){
	//sets parent's children as n's children
	n.setChildren(parent.getChildren());
	parent.clearChildren();//clears parent's children
	parent.appendChild(n);//sets n as parent's child
	//sets all former parent's children's parent to n
	for(int i = 0; i < n.getNumberOfChildren(); i++)
		n.getChild(i).setParent(n);
}

/*replaces the specified node replace (and any children
it may have) with the specified node add*/
public void replaceSubtree(Node replace, Node add){
	/*if replace is the root, add becomes the new root*/
	if(replace.getParent() == null)
		this.root = add;
	else
		replace.getParent().replaceChild(replace, add);
}

/*removes the specified node n from its parent's children
and appends it's children to its parent's children. Does
nothing if n is the root of the tree*/
public void deleteNode(Node n){
	if(n.getParent() != null){//if n is not the root
		/*sets n's parent as the parent for n's children*/
		for(int i = 0; i < n.getNumberOfChildren(); i++)
			n.getChild(i).setParent(n.getParent());
		/*appends n's children to n's parents*/
		n.getParent().appendChildren(n.getChildren());
		/*removes n from its parent's children*/
		n.getParent().removeChild(n);
	}
}


/*-----------------Scene Building Methods-----------------*/
/*The following methods properly compute a scene by going
to each leaf (shape) and multiplying it, in-order, with
the transformations in all of its ancestor nodes, returning
an array of fully transformed polygons (scene). The Time
Complexity of this entire operation is O(nlogn), this
algorithm goes through each node in the tree in order to
reach find each leaf (O(n)), then 'moves up' to the given
root in the tree(O(logn))*/

/*inderectly calls a private helper method that carries out
the work of building a scene from a Scene Graph from the root
of the entire tree. The helper method needs two root node
markers in order to recurse through one and keep the other as
an indication to stop*/
public Poly[] buildScene(){
	return buildScene(this.root);
}

/*calls a private helper method that carries out the work
of building a scene from a Scene Graph. The helper method
needs two root node markers in order to recurse through
one and keep the other as an indication to stop. The node
in the parameters allows the building of a scene from a
particular point in a tree, rather than the entirety*/
public Poly[] buildScene(Node root){
	return buildScene(root, root, new ArrayList<Poly>());
}

/*returns a list of transfomed polygons that are descendents
of the specified root node. (The root could be a non-root
node if only a particular part of the scene graph is edited).
This method finds each leaf that stems from the specified
root and calls a helper method to properly transform that
leaf, then adds it to the list of polygons to be returned*/
private Poly[] buildScene(Node n, Node root, ArrayList<Poly> transformed){
	/*finds each leaf stemming from the specified node n*/
	for(int i = 0; i < n.getNumberOfChildren(); i++)
		buildScene(n.getChild(i), root, transformed);
	/*if the specified node n is a leaf, it gets properly
	transformed with its anscestors*/
	if(n.getNumberOfChildren() == 0){
		Poly p = new Poly(n.getPoly());
		transformed.add(buildLeaf(p, n, root));
	}
	return toArray(transformed);
}

/*helper method that properly multiplies the specified
polygon 'shape' with node ancestor's transformation matrices.
Then, this method is recursivly called with ancestor's parent
as the new ancestor, unless ancestor is already the specified
root. In that case, leaf's transformed polygon is returned*/
private Poly buildLeaf(Poly shape, Node ancestor, Node root){
	shape = shape.transform(ancestor);
	if(ancestor.getID() == root.getID())
		return shape;
	return buildLeaf(shape, ancestor.getParent(), root);
}

/*----------------------Other Methods---------------------*/

/*prints the built scene, simply by calling print on every
polygon in the array of transformed shapes that buildScene
yeilds*/
public void printScene(){
	Poly[] print = buildScene(this.root);
	for(int i = 0; i < print.length; i++)
		print[i].print();
}

/*returns an ArrayList of type Poly as array of type Poly.
for some reason, casting ArrayList.toArray() as a Poly[]
produces a runtime exception*/
private Poly[] toArray(ArrayList<Poly> shapes){
	Poly[] polygons = new Poly[shapes.size()];
	for(int i = 0; i < shapes.size(); i++)
		polygons[i] = shapes.get(i);
	return polygons;
}

/*returns all the nodes in this SceneGraph as an array of nodes*/
public Node[] toArray(){
	LinkedList<Node> l = toList(this.root);
	Node[] sceneArray = new Node[l.length];
	for(int i = 0; i < l.length; i++) { 
		sceneArray[i] = (Node) l.get(i).data; 
	}
	return sceneArray;
}

/*returns all the nodes in this SceneGraph as a Linked List
of nodes.*/
private LinkedList<Node> toList(Node subtree){
	LinkedList<Node> l = new LinkedList<Node>(subtree);
	for(int i = 0; i < subtree.getNumberOfChildren(); i++){
		LinkedList<Node> childList = toList(subtree.getChild(i));
		l.append(childList.head);
	}
	return l;
}

/*returns a copy of the same tree or subtree stemming
from the specified node root*/
private Node copyTree(Node parent, Node root){
	Node copy = new Node(root);
	copy.clearChildren();
	if(parent != null)
		copy.setParent(parent);
	for(int i = 0; i < root.getNumberOfChildren(); i++)
		copy.appendChild(copyTree(copy, root.getChild(i)));
	return copy;
}

/*goes through the Scene Graph (tree) and assigns a unique
ID to each node via Flipbook's requestID method*/
public void assignIDs(Node root){
	root.setID(flipbook.requestID());
	for(int i = 0; i < root.getNumberOfChildren(); i++)
		assignIDs(root.getChild(i));
}

/*--------------------Getters & Setters-------------------*/

public Node getRoot(){ return this.root;}
public void setRoot(Node root){ this.root = root;}

/*sets the flipbook. Called when a flipbook is adding a 
scene graph*/
public void setFlipbook(Flipbook flipbook){
	this.flipbook = flipbook;
}

/*traverses the tree and returns the node with the same ID.
begins the search at subtree n*/
public Node getNodeByID(int uniqueID, Node n){
	if(n.getID() == uniqueID) { return n; }
	/*recurses through all the nodes in the tree 
	until a match is found. Returns the found match*/
	for(int i = 0; i < n.getNumberOfChildren(); i++) {
		Node match = getNodeByID(uniqueID, n.getChild(i));
		if(match != null) { return match; }
	}
	return null;
}

}