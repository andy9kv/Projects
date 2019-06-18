
/*
* Andy Morato
* Node class most notably holds a shape, transformations,
* and children so that a 'scene graph' tree can be possible.
* Only leaf nodes carry shapes.
* Created July 26th, 2017
*/

import java.util.ArrayList;

public class Node{

/*-----Instance Variables-----*/

private Poly shape;/*holds the shape*/
private String name; /*holds the name*/
private int uniqueID; /*holds an indentifier*/
private ArrayList<Node> children;/*holds the children nodes*/
private Node parent;/*holds the parent node*/
/*holds the transformation matricies*/
private Mat rot;
private Mat trans;
private Mat scale;

/*---------------------Constructors-----------------------*/

/*close to a default constructor, expect initializes
name and uniqueID to the specified name and uniqueID*/
public Node(String name, int uniqueID){ 
	this(new Poly(), name, uniqueID);
}

/*initializes the Poly to the specified Poly, name to the specified
name, uniqueID, and identity matrices for transformations*/
public Node(Poly shape, String name, int uniqueID){
	this.shape = new Poly(shape);
	this.name = new String(name);
	this.uniqueID = uniqueID;
	this.children = new ArrayList<Node>();
	initializeTransforms();
}

/*initializes a mostly empty node with the specifed children,
 specified name and specified uniqueID*/
public Node(ArrayList<Node> children, String name, int uniqueID){
	this.children = children;
	this.name = name;
	this.uniqueID = uniqueID;
	initializeTransforms();
}

/*copy constructor that copies all of the specified node
original's attributes except children and the parent*/
public Node(Node original){
	this(original.getPoly(), original.getName(), original.getID());
	this.children = original.getChildren();
	this.rot = original.getRotation();
	this.trans = original.getTranslation();
	this.scale = original.getScale();
}

/*---------------------Node Management--------------------*/

/*appends a child to the list of children and sets this
instance as the child's parent*/
public void appendChild(Node child){ 
	children.add(child);
	child.setParent(this);
}

/*appends several children to the list of children*/
public void appendChildren(ArrayList<Node> children){
	this.children.addAll(children);
}

/*removes the specified child from the list of children*/
public void removeChild(Node child){
	this.children.remove(child);
}

/*replaces the specified child 'replace' in the list of
children so that the new child 'add' maintains the position
of the previous child, relative to the other children*/
public void replaceChild(Node replace, Node add){
	int index = this.children.indexOf(replace);
	if(index != -1)//replace was in the list
		this.children.set(index, add);
}

/*removes all this node's children*/
public void clearChildren(){
	ArrayList<Node> children = new ArrayList<Node>();
	this.children = children; 
}

/*------------------Transformation Methods----------------*/

/*sets the rotation matrix to rotate the specified angle*/
public void rotate(double angle){
	this.rot = Mat.rotationMatrix(angle);
}

/*sets the translation matrix to move the specified distance*/
public void translate(double x, double y){
	this.trans = Mat.translationMatrix(x, y);
}

/*sets the scalar matrix to scale the specified amout*/
public void scale(double x, double y){
	this.scale = Mat.scaleMatrix(x, y);
}

/*overloaded version of the method above that sets the scalar
matrix to scale the specifed amount in both directions*/
public void scale(double scale){
	this.scale = Mat.scaleMatrix(scale, scale);
}

/*transforms the Poly in this node with the transformation
matrices in the specified parent node by calling the
transform method in the Poly class*/
public Node transform(Node parent){
	Poly transformed = shape.transform(parent);
	return new Node(transformed, this.name, this.uniqueID);
}

/*----------------------Other Methods---------------------*/

/*initializes all of the transformation matrices to the
identity matrix*/
public void initializeTransforms(){
	rot = Mat.identityMatrix();
	trans = Mat.identityMatrix();
	scale = Mat.identityMatrix();
}

/*returns a String representation of this node, containing
all relevant information*/
public String toString(){
	String node = "";
	//displays the name of the node, if applicable
	if(name.equals(""))
		node += "******~Node~******\n\n";
	else
		node += "******~" + getName() + "~******\n\n";
	/*displays the parent's name if the parent exists
	and has a name*/
	if(parent != null)
		if(!parent.getName().equals(""))
			node += "Parent: " + parent.getName() + "\n\n";
	//displays the uniqueID of the node
	node += "ID: #" + this.uniqueID + "\n\n";
	/*either displays a node's shape (if the node is a leaf)
	or a list of this node's children (if it is not a leaf)*/
	if(isLeaf())
		node += this.shape.toString();
	else {
		node += "Children (" + getNumberOfChildren() + "):\n";
		for(int i = 0; i < this.children.size(); i++){
			node += "    ";
			if(name.equals(""))
				node += "Some random no-name node\n";
			else
				node += getChild(i).getName() + "\n"; 
		}
	}
	//displays the transformations this node holds
	node += "\n----- Transforms -----\n\n";
	node += "Rotation:\n";
	node += this.rot.toString() + "\n";
	node += "Translation:\n";
	node += this.trans.toString() + "\n";
	node += "Scale:\n";
	node += this.scale.toString() + "\n";
	node += "--- End Transforms ---\n\n";
	//displays the name of the node, if applicable
	if(name.equals(""))
		node += "*******~End~*******\n\n";
	else
		node += "*****~end " + getName() + "~*****\n\n";
	return node;
}

/*prints the toString representation of this node*/
public void print(){ System.out.print(toString());}

/*------------------------Getters-------------------------*/

public Poly getPoly(){ return this.shape;}

/*returns true if this node carries a shape (leaf node)*/
public boolean isLeaf(){return children.isEmpty();}

/*returns the child at the specified index, if it exists,
else returns null*/
public Node getChild(int index){
	if(index < this.children.size())
		return children.get(index);
	return null;
}

public Node getParent(){ return this.parent;}

public ArrayList<Node> getChildren(){ return children;}

/*returns the number of this node's children*/
public int getNumberOfChildren(){ return children.size();}

public String getName(){ return this.name;}

public int getID(){ return this.uniqueID;}

/*transformation getters*/
public Mat getRotation(){ return this.rot;}
public Mat getTranslation(){ return this.trans;}
public Mat getScale(){ return this.scale;}

/*------------------------Setters-------------------------*/

public void setName(String name){ this.name = name;}

public void setID(int uniqueID){ this.uniqueID = uniqueID;}

public void setParent(Node parent){ this.parent = parent;}

public void setChildren(ArrayList<Node> children){
	this.children = children;
}

}