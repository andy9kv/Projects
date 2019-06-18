
/*
* Andy Morato
* TerminalOutput class takes in a list of shapes, where each shape is
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

public class TerminalOutput {

/*--------------------------Constructors----------------------------*/

public TerminalOutput(Poly[] shapes) { 
	//converts scene to a binary image
	boolean[][] img = sceneToImg(shapes);
	//gets string from the binary image
	String display = imgToString(img, ' ', '*');
	//print string
	System.out.println(display);
 }

/*---------------------Scene-String Conversion----------------------*/

/*Converts a scene to a essentailly a binary image, matrix (2D array)
of booleans where true marks a spot to place a mark, and false
represents an empty spot*/
private boolean[][] sceneToImg(Poly[] shapes){
	int[] extreames = getExtreames(shapes);
	//gets appropriate dimensions
	int height = extreames[0];
	int width = extreames[2];
	int heightBuffer = 0, widthBuffer = 0;
	if(extreames[1] < 0) {
		heightBuffer = extreames[1] * (-1);
		height = height - extreames[1];
	}
	if(extreames[3] < 0) {
		widthBuffer = extreames[3] * (-1);
		width = width - extreames[3];
	}
	System.out.println("--------------------------------");
	System.out.println("height (y): " + height);
	System.out.println("width (x): " + width);
	System.out.println("hBuff: " + heightBuffer);
	System.out.println("wBuff: " + widthBuffer);
	System.out.println("--------------------------------");
	//makes the binary image (boolean matrix (2D array))
	boolean[][] img = new boolean[width][height];
	//goes through array of shapes and fills a spot for each point
	for(int i = 0; i < shapes.length; i++)
		//access all points in a shape
		for(int j = 0; j < shapes[i].getPoints().size(); j++){
			int x = (int) shapes[i].getPoints().getRow(j).getPoint(0);
			int y = (int) shapes[i].getPoints().getRow(j).getPoint(1);
			System.out.println("y: " + y + " + " + heightBuffer);
			System.out.println("x: " + x + " + " + widthBuffer);
			//sets each point in shapes to a full spot in img
			img[x + widthBuffer][y + heightBuffer] = true;
		}
	return img;
} 


/*Takes in essentailly a binary image, a matrix (2D array) where
booleans represent the spots to be filled out. Returns a string
with 'fill' placed in every true spot and 'empty' otherwise*/
private String imgToString(boolean[][] lines, char fill, char empty){
	String scene = "";
	for(int i = 0; i < lines.length; i++){
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

/*----------------------Information Extraction----------------------*/

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