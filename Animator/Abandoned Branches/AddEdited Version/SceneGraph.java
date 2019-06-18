
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
changed when this scene graph become part of a Flipbook*/
private Flipbook flipbook;

/*---------------------Constructors-----------------------*/

/*Default Constructor that sets a new Node as the root*/
public SceneGraph(){ this(new Node());}

/*constructor that sets the specifed root as this instance's
root node*/
public SceneGraph(Node root){ this.root = new Node(root);}

/*copy constructor that copies the entrie tree stemming
from the root*/
public SceneGraph(SceneGraph original){
	SceneGraph copy = new SceneGraph();
	copy.setRoot(copyTree(null, original.getRoot()));
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
	if(replace.getParent == null)
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

/*this method returns the node at the specified position.
Position in an measure of "what number node this is".
Counting begins at the root, increases with the root's
children, then continues with the leftmost 'grandchild'
to the rightmost 'grandchild', etc etc*/
public Node findByPosition(int position){
	if(position == 0)
		return this.root;
	return findByPosition(this.root.getChildren(), position-1);
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

/*calls a private helper method that carries out the work
of building a scene from a Scene Graph. The helper method
needs two root node markers in order to recurse through
one and keep the other as an indication to stop*/
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
	if(ancestor == root)
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

/*returns a copy of the same tree or subtree stemming
from the specified node root*/
private Node copyTree(Node parent, Node root){
	Node copy = new Node(root);
	if(parent != null)
		copy.setParent(parent);
	for(int i = 0; i < root.getNumberOfChildren(); i++)
		copy.appendChild(copyTree(copy, root.getChild(i)));
	return copy;
}

/*helps findByPosition by checking all the children of a
particular level (specified by the children of the nodes
in an array of sibiling nodes). Keeps on recursing down
the tree if the specified position is not reached and
returns the node at the specified position when reached*/
private Node findByPosition(ArrayList<Node> sibilings, int position){
	//if the target node is in the siblings array
	if(position < sibilings.size())
		return sibilings.get(position);
	/*if not, does the same process (recurses) with the
	children of the siblings*/
	/*creates an array of the children of the sibilings*/
	ArrayList<Node> children = new ArrayList<Node>();
	for(int i = 0; i < sibilings.size(); i++){
		int size = sibilings.get(i).getNumberOfChildren();
		//loops through the children of the sibilings
		for(int j = 0; j < size; j++)
			children.add(sibilings.get(i).getChild(j));
	}
	/*recurses with the new children array, and with an
	updated position*/
	return findByPosition(children, position-siblings.size());
}

/*helps getPosition by checking all the children of a
particular level (specified by the children of the nodes
in an array of sibiling nodes). Keeps on recursing down
the tree if the specified node find is not found, returns
it's position in its respective level if it is found*/
private int checkLevel(ArrayList<Node> sibilings, Node find){
	int position = 0;
	ArrayList<Node> children = new ArrayList<Node>();
	//loops through the sibilings
	for(int i = 0; i < sibilings.size(); i++){
		int size = sibilings.get(i).getNumberOfChildren();
		//loops through the children of the sibilings
		for(int j = 0; j < size; j++){
			position++;
			if(find == sibilings.get(i).getChild(j))
				return position
			/*adds the children, in order & one at the
			time, to the a new sibilings list to loop
			through in a recursive call of this method
			just in case the target node is not found
			in this level of the tree*/
			children.add(sibilings.get(i).getChild(j));
		}
	}
	/*recurses down the next level of the tree if the
	target node was not found at the previous level of
	the tree*/
	return position + checkLevel(children, find);
}

/*goes through the Scene Graph (tree) and assigns a unique
ID to each node via Flipbook's requestID method*/
public void assignIDs(Node root){
	root.setId(flipbook.requestID());
	for(int i = 0; i < root.getNumberOfChildren(); i++)
		assignIDs(root.getChild(i));
}

/*--------------------Getters & Setters-------------------*/

public Node getRoot(){ return this.root;}

/*returns the position of the specified node in this
scene graph. Position in a measure of "what number node
this is". Counting begins at the root, increases with
the root's children, then continues with the leftmost
'grandchild' to the rightmost 'grandchild', etc etc*/
public int getPosition(Node find){
	if(this.root == find)
		return 0;
	return checkLevel(root.getChildren(), find);
}

public void setRoot(Node root){ this.root = root;}

/*sets the flipbook. Called when a flipbook is adding a 
scene graph*/
public void setFlipbook(Flipbook flipbook){
	this.flipbook = flipbook;
}

}