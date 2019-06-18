
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

/*----------------------Create First Frame--------------------*/

 	//Stick Figure
	Poly l = Poly.stickFigure();

 	//Weapon
 	Vec w11 = new Vec(5, 2);
 	Vec w12 = new Vec(5, 3);
 	Vec w21 = new Vec(6, 3);
 	Vec w22 = new Vec(7, 3);
 	Vec w23 = new Vec(8, 3);
 	Vec[] wl = new Vec[]{ w11, w12, w21, w22, w23 };
 	Mat wml = new Mat(wl);
 	Poly w = new Poly(wml);
 	w.setLine(true);

 	//Projectile
 	Poly pr = Poly.dot();

 	//Creates nodes/scene graph
 	Node root = new Node("root");
 	Node figure1 = new Node(l, "attacker");
 		figure1.translate(0, -3);
 		root.appendChild(figure1); //child 0
 	Node figure2 = new Node(figure1);
 		figure2.setName("victim");
 		figure2.addTranslation(60, 0);
 		root.appendChild(figure2); //child 1
 	Node weapon = new Node(w, "weapon");
 		root.appendChild(weapon); //child 2
 	Node projectile = new Node(pr, "projectile");
 		projectile.translate(9, 3);
 		root.appendChild(projectile); //child 3
 	SceneGraph frame1 = new SceneGraph(root);


/*----------------------Create Second Frame-------------------*/

 	Flipbook fb = new Flipbook(frame1);
 	SceneGraph frame2 = fb.copyScene(0);
 	frame2.getRoot().getChild(3).addTranslation(52, 0);
 	fb.addScene(frame2, true);


/*----------------------------Animate--------------------------*/

	fb.animationDriver(5);
	//fb.showAnimationProcess(5);
}

}