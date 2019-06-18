
/*
* Andy Morato
* This Flipbook class represents a collection of scenes/frames
* that can be iterated through and displayed in quick, regular
* time intervals (framerate) to produce some kind of animation.
* Created August 3rd, 2017
*/

import java.util.ArrayList;

public class Flipbook {

/*-----Instance Variables-----*/

private ArrayList<SceneGraph> scenes;
private double framerate; /*measure of frames per second*/
int uniqueIDTracker;/*keeps track of all different nodes
in the Flipbook, used to assign uniqueID's to new nodes*/

/*---------------------Constructors-----------------------*/

/*Default constructor, initializes an empty list of scenes
and sets the framerate to a default 3 frames per second*/
public Flipbook(){ 
	this.scenes = new ArrayList<SceneGraph>();
	this.framerate = 3;
	this.scenes = 0;
}

/*-------------------Flipbook Management------------------*/

/*rather than build a new scene from scratch to add to the
global list of scenes, it is preferable to simply edit the
previous scene on the list. In programmer's terms, this
means copying the previous scene on the list and replacing
the subtree below which there have been edits (i.e. new
shapes, different transformations, etc.), and adding that
new scene to global list scenes. This requires receiving a
subtree node (edited) and the old, obsolete corresponding
subtree (unedited) in the parameters, replacing the old
node with the edited one in the graph, and adding the new,
complete scene to the global list of scenes*/
public void addEditedScene(Node unedited, Node edited){
	/*if there exists a previous scene*/
	if(this.scenes.size() <= 0)
		return;
	SceneGraph previous = scenes.get(scenes.size() - 1);
	int pos = previous.getPosition(unedited);
	SceneGraph copy = new SceneGraph(previous);
	Node replace = copy.findByPosition(pos);
	copy.replaceSubtree(replace, edited);
}

/*adds a new scene to the flipook (global list of
scenes); assigning a uniqueID to each node int the scene,
and setting the given scene's flipbook to this instance*/
public void addNewScene(SceneGraph scene){
	scene.assignIDs(scene.getRoot());
	this.scenes.add(scene);
	scene.setFlipbook(this);
}

/*creates a Scene Graph and inserts it to the ArrayList
immediately after the SceneGraph at the given position.
The inserted scene is a "bridge" or a "smoothing out" of
the Flipbook between the previous and the next scenes.
This is accomplished by first finding the nodes that exist
in the previous and next scenes, then adding the same
nodes to the inserted scene, with all its vertices in
between (midpoint) the correspoding vertices in the 
prevous scene and the next scene. In other words, this
method inserts a new scene in the Flipbook that smooths
out the scenes around it. For the moment, this method
does nothing to address any created or deleted vertices,
as well as any shapes with new vertices in the previous
or next scenes. The basic parts of this process are:
    1) find the nodes that exist in the previous and
	   next scenes
    2) create a new scene such that its vertices are
       the midpoints of the coresponding vertices of
       the previous and next scenes*/
public void autoBrigeScenes(int position){
	/*if the position is out of bounds or the last
	element of the list*/
	if(this.scenes.size() < (position - 1))
		return;
	SceneGraph insert = new SceneGraph(scenes.get(position));


}

/*----------------------Helper Methods--------------------*/

/*helper to autoBridgeScenes, returns a list of uniqueID's of
nodes that exist both in given scenes a & b. Searches both */
private int[] findMutualExistence(SceneGraph a, SceneGraph b){
	
}

/*----------------------Other Methods--------------------*/

/*returns a unique (never used before) ID in the form of
an int for the scene graph class to assign to new nodes,
then increments the unique ID tracker*/
public int requestID(){
	return uniqueIDTracker++;
}

/*--------------------Getters & Setters-------------------*/

/*returns the frames per second of the animation*/
public doulbe getFramerate(){ return this.framerate;}

/*sets the framerate. fps = frames per second*/
public void setFramerate(double fps){ framerate = fps;}

}