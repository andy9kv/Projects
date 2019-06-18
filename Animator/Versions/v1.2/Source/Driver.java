
/*
* Andy Morato
* This Driver class prints the scenes and animation to the 
* terminal. The plan is to do so in an application, but this
* alternative will satisfy for version 1.
* Created October 7th, 2017
*/

import java.util.ArrayList;

class Driver{

public static void main(String[] args) {

	//Shapes

 	//Pentagon
 	Vec p1 = new Vec(6, 0);
 	Vec p2 = new Vec(8, 0);
  	Vec p3 = new Vec(8, 2);
 	Vec p4 = new Vec(6, 2);
 	Vec p5 = new Vec(7, 3);
 	Vec[] vp = new Vec[]{p1, p2, p3, p4, p5};
 	Mat mp = new Mat(vp);
 	Poly p = new Poly(mp);

 	//Square
 	Vec s1 = new Vec(0, 0);
 	Vec s2 = new Vec(2, 0);
  	Vec s3 = new Vec(2, 2);
 	Vec s4 = new Vec(0, 2);
 	Vec[] vs = new Vec[]{s1, s2, s3, s4};
 	Mat ms = new Mat(vs);
 	Poly s = new Poly(ms);

 	//Triangle
 	Vec t1 = new Vec(3, 0);
 	Vec t2 = new Vec(5, 0);
  	Vec t3 = new Vec(4, 2);
 	Vec[] vt = new Vec[]{t1, t2, t3};
 	Mat mt = new Mat(vt);
 	Poly t = new Poly(mt);

 	//Line
 	Vec l1 = new Vec(3, -2);
 	Vec l2 = new Vec(5, -2);
 	Vec[] vl = new Vec[]{l1, l2};
 	Mat ml = new Mat(vl);
 	Poly l = new Poly(ml);
 	l.setLine(true);

 	//Nodes/Scene Graph

 	Node root = new Node("root", 1);
 	Node node1 = new Node("right child", 2);
 		node1.rotate(180);
 		root.appendChild(node1);
 	Node pentagon = new Node(p, "pentagon", 3);
 		pentagon.scale(2);
 		root.appendChild(pentagon);
 	//children of node1
 	Node upsideDown = new Node("upsideDown", 4);
 		upsideDown.rotate(180);
 		upsideDown.translate(-2, -2);
 		upsideDown.scale(5);
 		node1.appendChild(upsideDown);
 	Node triangle = new Node(t, "triangle", 5);
 		triangle.scale(2);
 		node1.appendChild(triangle);
 	//children of upsideDown
 	Node square = new Node(s, "square", 6);
 		upsideDown.appendChild(square);
 	Node line = new Node(l, "line", 7);
 		line.translate(3, 0);
 		upsideDown.appendChild(line);

 	SceneGraph tree = new SceneGraph(root);
 	tree.printScene();
 	//TerminalOutput to = new TerminalOutput(tree.buildScene());
}

/*Creates and returns the default Scene of a city's skyline*//*
public SceneGraph createDefaultScene(){
	Poly square = new Poly(Poly.square);
	Node head 
	return new SceneGraph();
}*/

}