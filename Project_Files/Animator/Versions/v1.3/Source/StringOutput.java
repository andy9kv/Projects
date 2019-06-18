
/*
* Andy Morato
* StringOutput class takes in a list of shapes, where each shape is
* a list of coordinates, and prints it out in the terminal
* -------
* Alternate Description: Essentially, takes an array of Poly's (AKA a
* built SceneGraph), which is no more than an array of arrays of
* numbers (points in the shapes), and returns a String represntation
* of the Poly array. This is done by creating strings that represnt
* rows (y = -1, 0, 1, etc) in a graph and putting a "*" where
* there would be a corresonding point in a graph. Then, all
* the strings are combined and returned as a represntation
* of a Scene
* -------
* Created December 20th, 2017
*/

public class StringOutput {

/*-----Instance Variables-----*/

private char fill;
private char empty;

/*--------------------------Constructors---------------------------*/

/*default constructor prints full & empty spots with *s and spaces*/
public StringOutput(){ this('*', ' '); }

/*constructor that allows setting the characters that will be printed
in the place of full and empty spots*/
public StringOutput(char fill, char empty) { 
	this.fill = fill;
	this.empty = empty;
 }


/*---------------------Scene-String Conversion---------------------*/

/*uses the helpers below to create and return a printable string from
the given scene (in the form of an array of shapes)*/
public String sceneToString(Poly[] shapes){
	//converts scene to a binary image
	boolean[][] img = sceneToImg(shapes, 2);
	//gets string from the binary image
	String display = imgToString(img, this.fill, this.empty);
	//returns the string
	return display;
}

/*converts a scene to a essentailly a binary image, matrix (2D array)
of booleans where true marks a spot to place a mark, and false
represents an empty spot. int pad marks the amount of empty rows
of cells to be printedaround the image*/
private boolean[][] sceneToImg(Poly[] shapes, int pad){
	int[] extreames = getExtreames(shapes);
	//gets appropriate dimensions
	int h = extreames[0];
	int w = extreames[2];
	int hBuffer = 0, wBuffer = 0;
	if(extreames[1] < 0) {
		hBuffer = extreames[1] * (-1);
		h = h - extreames[1];
	}
	if(extreames[3] < 0) {
		wBuffer = extreames[3] * (-1);
		w = w - extreames[3];
	}
	//makes the binary image (boolean matrix (2D array))
	boolean[][] img = new boolean[h + (pad*2) + 1][w + (pad*2) + 1];
	//goes through array of shapes and fills a spot for each point
	for(int i = 0; i < shapes.length; i++)
		//access all points in a shape
		for(int j = 0; j < shapes[i].getPoints().size(); j++){
			int x = (int)shapes[i].getPoints().getRow(j).getPoint(0);
			int y = (int)shapes[i].getPoints().getRow(j).getPoint(1);
			//sets each point in shapes to a full spot in img
			img[y + pad + hBuffer][x + pad + wBuffer] = true;
		}
	return img;
} 


/*Takes in essentailly a binary image, a matrix (2D array) where
booleans represent the spots to be filled out. Returns a string
with 'fill' placed in every true spot and 'empty' otherwise*/
private String imgToString(boolean[][] lines, char fill, char empty){
	String scene = "";
	for(int i = lines.length - 1; i >= 0; i--){
		/*adds each line to the string scene*/
		String line = "";
		for(int j = 0; j < lines[i].length; j++){
			if(lines[i][j]) { line = line + fill; }
			else { line = line + empty; }
		}
		scene = scene + line + "\n";
	}
	return scene;
}

/*----------------------Information Extraction---------------------*/

/*returns the highest/lowest/rightmost/leftmost points in the scene*/
private int[] getExtreames(Poly[] shapes){
	int highest = (int) shapes[0].getPoints().getExtreames(1);
	int lowest = (int) shapes[0].getPoints().getExtreames(3);
	int rightmost = (int) shapes[0].getPoints().getExtreames(0);
	int leftmost = (int) shapes[0].getPoints().getExtreames(2);
	for(int i = 1; i < shapes.length; i++){
		if(highest < shapes[i].getPoints().getExtreames(1))
			highest = (int) shapes[i].getPoints().getExtreames(1);
		if(lowest > shapes[i].getPoints().getExtreames(3))
			lowest = (int) shapes[i].getPoints().getExtreames(3);
		if(rightmost < shapes[i].getPoints().getExtreames(0))
			rightmost = (int) shapes[i].getPoints().getExtreames(0);
		if(leftmost > shapes[i].getPoints().getExtreames(2))
			leftmost = (int) shapes[i].getPoints().getExtreames(2);
	}
	int[] extreames = {highest, lowest, rightmost, leftmost};
	return extreames;
}


}