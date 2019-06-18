
/*
* Andy Morato
* This Flipbook class represents a collection of scenes/frames
* that can be iterated through and displayed in quick, regular
* time intervals (framerate) to produce some kind of animation.
* Created August 3rd, 2017
*/

import java.lang.Thread;

public class Flipbook {

/*-----Instance Variables-----*/

private LinkedList<SceneGraph> scenes;
private LinkedList<SceneGraph> originalFrames;/*keeps track
of the frames that were manually added to the flipbook*/
int uniqueIDTracker;/*keeps track of all different nodes
in the Flipbook, used to assign uniqueID's to new nodes*/

/*---------------------Constructors-----------------------*/

/*constructor that initializes a list of scenes containing
only the start scene*/
public Flipbook(SceneGraph start){ 
	this.originalFrames = new LinkedList<SceneGraph>();
	this.uniqueIDTracker = 0;
	addScene(start, false);
}


/*-------------------Flipbook Management------------------*/

/*appends a new scene to the end of the list of scenes.
Sets the scene's flipbook to this instance and assigns
uniqueID's to each node in the SceneGraph depending on
the boolean edited (if the scene is full of new nodes or
simply edited (transformed) old nodes)*/
public void addScene(SceneGraph scene, boolean edited){
	this.originalFrames.append(scene);
	scene.setFlipbook(this);
	if(edited == false)
		scene.assignIDs(scene.getRoot());
}

/*calls bridgeScenes between any two consecutive scenes in the
flipbook of scenes and inserts the resulting scene in between
them. Computes a single iteration on the whole flipbook*/
private void autoBridgeScenes(){
	for(int i = 0; i < scenes.length - 1; i += 2){
		/*creates scene bridge between SceneGraphs at positions
		i and i + 1 in the flipbook*/
		SceneGraph bridge = bridgeScenes(i, i + 1);
		scenes.insert(bridge, i);
	}
}

/*takes the indices of two scenes in the flipbook, finds the
differences between each node (using their uniqueIDs), and returns
a new scene with the differences split (essentially midpoints)*/
private SceneGraph bridgeScenes(int i, int j){
	//gets the scenes that will be explored
	SceneGraph scene1 = (SceneGraph) scenes.get(i).data;
	SceneGraph scene2 = (SceneGraph) scenes.get(j).data;
	SceneGraph briged = new SceneGraph(scene2);
	/*gets an array of all nodes and scene1 and compares their
	transformations to the equivalent (by uniqueID) nodes of scene2*/
	Node[] s1 = scene1.toArray();
	for(int k = 0; k < s1.length; k++){
		Node n = s1[k];
		Node eqv = briged.getNodeByID(n.getID(), briged.getRoot());
		/*gets the difference between both Node's transformations*/
		double r = (eqv.getRot() - n.getRot()) / 2;
		double tx = (eqv.getTrans()[0] - n.getTrans()[0]) / 2;
		double ty = (eqv.getTrans()[1] - n.getTrans()[1]) / 2;
		double sx = (eqv.getScalar()[0] - n.getScalar()[0]) / 2;
		double sy = (eqv.getScalar()[1] - n.getScalar()[1]) / 2;
		/*sets the buffers for the values (ex: if point a was at
		x = 6 and now is at x = 16, the difference is 10, however
		the true midpoint is not at 10/2 = 5, it is at 11, which
		is 5 + 6 (6 is the buffer)*/
		r = r + n.getRot();
		sx = sx + n.getScalar()[0];
		sy = sy + n.getScalar()[0];
		tx = tx + n.getTrans()[0];
		ty = ty + n.getTrans()[1];
		/*sets the transformations for the bridged scene, either
		the original transformation values or the split difference*/
 		if(r != 0) { eqv.rotate(r); }
 		if(tx != 0 && ty != 0) { eqv.translate(tx, ty); }
 		else if(tx == 0 && ty != 0){
 			eqv.translate(n.getTrans()[0], ty);
 		}
 		else if(tx != 0 && ty == 0){
 			eqv.translate(tx, n.getTrans()[1]);
 		}
 		if(sx != 0 && sy != 0) { eqv.scale(sx, sy); }
 		else if(sx == 0 && sy != 0){eqv.scale(n.getScalar()[0], sy);}
 		else if(sx != 0 && sy == 0){eqv.scale(sx, n.getScalar()[1]);}
	}
	return briged;
}


/*----------------------Animation Functions--------------------*/

/*displays 3 scenes in the flipbook per second (fps = default 5)*/
private void playAnimation(){ playAnimation(5);}

/*displays each scene in the flipbook for the amount of
time specified by the given framerate (frames per second). 
Does not clear the screen after each animation if the frame
rate (fps) is less than 1*/
private void playAnimation(int fps){
	StringOutput strOut = new StringOutput();
	playAnimation(fps, strOut);
}

/*displays each scene in the flipbook for the amount of
time specified by the given framerate (frames per second).
Also, takes a StringOutput, which determines how exactly
the scenes will be represented by strings (terminal print).
Does not clear the screen after each animation if the frame
rate (fps) is less than 1*/
private void playAnimation(int fps, StringOutput strOut){
	int milliseconds = 1500;
	if(fps > 0) { milliseconds = 1000 / fps; }
	int i = 0;
	SceneGraph scene = (SceneGraph) this.scenes.get(i).data;
	//loops through the list of scenes
	while(scene != null){
		//displays each scene
		System.out.println(strOut.sceneToString(scene.buildScene()));
		sleep(milliseconds);
		if(fps > 0) { clearScreen(); }
		if(scenes.get(i + 1) == null) { scene = null; }
		else { scene = (SceneGraph) scenes.get(i + 1).data; }
		i++;
	}
}

/*method that bridges scenes n times (calls autoBridgeScenes())
then plays the resulting animation*/
public void animationDriver(int n){
	System.out.println("animating.....\n");
	this.scenes = copyScenes(originalFrames);
	sleep(1500);
	for(int i = 0; i < n; i++) { autoBridgeScenes(); }
	playAnimation();
}

/*method that bridges scenes n times (calls autoBridgeScenes())
and plays each resulting iterative animation. Essentially shows
the process for iterativly building the animation*/
public void showAnimationProcess(int n){
	//shows the original frames before automatic bridging
	this.scenes = copyScenes(originalFrames);
	System.out.println("original frames:\n");
	playAnimation(0);
	sleep(3000);
	clearScreen();
	//shows the animation process, including the final result
	for(int i = 0; i < n; i++){
		System.out.println("animating iteration number: "
		                   + (i + 1) + ".....\n");
		sleep(1500);
		autoBridgeScenes();
		playAnimation();
	}
}

//ADD FUNCTIONALITY TO RUN THE AUTO-BRIDGE-SCENE FUNCTION UNTIL
//EACH POINT IS BETWEEN 1 OR 2 (SOME MARGIN) AWAY FROM EACH OTHER
//AND THEN ENABLE THAT PROCESS TO RUN BETWEEN EACH ORIGINAL FRAME
//IN THE FLIPBOOK. ALSO, UNRELATED, BUT CHANGE, WHEREVER I CAN,
//TO LINKED LIST NEXT OF O(n) RATHER THAT THE STUPID get(i) THAT
//IS CURRENTLY IN USE, WHY DID I EVER DECIDE TO DO THAT SMH

/*----------------------Animation Helpers--------------------*/

/*sleeps the system (waits) for the time given*/
public static void sleep(int milliseconds){ 
	try {
    	Thread.sleep(milliseconds);
	} catch(InterruptedException ex) {
    	Thread.currentThread().interrupt();
	}
}

public static void clearScreen() {  
    System.out.print("\033[H\033[2J");  
    System.out.flush();  
}  

/*----------------------Other Methods--------------------*/

/*returns a unique (never used before) ID in the form of
an int for the scene graph class to assign to new nodes,
then increments the unique ID tracker*/
public int requestID() { return uniqueIDTracker++; }

/*returns a copy of the specified scene in the original 
flipbook. returns null if the index is out of bounds*/
public SceneGraph copyScene(int index){
	SceneGraph copy = (SceneGraph) originalFrames.get(index).data;
	return new SceneGraph(copy);
}

/*returns a copy of a linked list holding SceneGraphs*/
private LinkedList<SceneGraph> copyScenes(LinkedList<SceneGraph> l){
	LinkedList<SceneGraph> copy = new LinkedList<SceneGraph>();
	copy.length = l.length;
	copy.setHead(new SceneGraph((SceneGraph) l.getData(0)));
	for(int i = 1; i < l.length; i++){
		copy.setNext(i-1, new SceneGraph((SceneGraph) l.getData(i)));
	}
	return copy;
}

/*--------------------Getters & Setters-------------------*/

/*returns the requested scene of the orginal frames*/
public SceneGraph get(int index){
	return (SceneGraph) originalFrames.get(index).data;
}

}