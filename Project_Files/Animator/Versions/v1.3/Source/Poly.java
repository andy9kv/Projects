
/*
* Andy Morato
* Polygon class holds shapes/figures
* Created July 20th, 2017
*/

public class Poly{

/*-----Instance Variables-----*/

private Mat points; /*holds the points in the shape*/
private Vec color; /*holds the (A)RGB value*/
private boolean line; /*true if line and not polygon*/

/*---------------------Constructors-----------------------*/

/*Default Constructor, creates a black triangle with all
 points set at the origin, sets line to default false*/
public Poly(){ this(new Mat(), new Vec(), false);}

/*sets points to the specified points and sets the color
to the default black, line to default false*/
public Poly(Mat points){
	this(points, new Vec(), false);
}

/*sets points and color to the specified points and color,
sets boolean line to default false*/
public Poly(Mat points, Vec color){
	this(points, color, false);
}

/*sets points and line to the specified points and line*/
public Poly(Mat points, boolean line){
	this(points, new Vec(), line);
}

/*sets points, color, and boolean line to the specified
points, color, and sets line to default false*/
public Poly(Mat points, Vec color, boolean line){
	this.points = new Mat(points);
	this.color = new Vec(color);
	this.line = line;
}

/*copy constructor that copies all instance variables of
the specified Poly original and sets it as the variables
for this new instance*/
public Poly(Poly original){
	this(original.getPoints(), original.getColor(), original.isLine());
}

/*----------------------Other Methods---------------------*/

/*returns a String representation of this Poly, contatining
the number of vertices, if this shape is a line or polygon,
each vector in points, and the color of this Poly*/
public String toString(){
	String poly = "";
	/*displays whether the shape is a line or polygon*/
	if(isLine())
		poly += "-------Line Specs-------\n\n";
	else
		poly += "------Polygon Specs------\n\n";
	/*displays the number of vertices, vertices & color*/
	poly += "--Vertices (" + points.size() +")--\n\n";
	poly += this.points.toString() + "\n";
	poly += "--Color--\n\n";
	poly += this.color.toString() + "\n";
	poly += "--------End Specs--------\n\n";
	return poly;
}

/*print the toString representation of this Poly*/
public void print(){ System.out.print(toString());}

/*adds (appends) a vertex/point to this Poly*/
public void add(Vec point){
	this.points.append(point);
}

/*returns a new, transformed polygon by using a helper
method to multiply all of its vertices with the specified
node's transformation matrices in the given order*/
public Poly transform(Node trans){
	return transform(trans.getRotation(), trans.getTranslation(),
	                 trans.getScale());
}

/*returns a new, transformed polygon by multiplying all
of its vertices with the specified transformation matrices
in the RST order*/
private Poly transform(Mat rot, Mat trans, Mat scale){
	Mat tv = new Mat(size());//tv = transformed vertices
	for(int i = 0; i < size(); i++){
		Vec v = rot.multiply(this.points.getRow(i));
		v = scale.multiply(v);
		v = trans.multiply(v);
		tv.setRow(v, i);
	}
	return new Poly(tv, new Vec(this.color), this.line);
}

/*--------------------Getters & Setters-------------------*/

public Vec getPoint(int index){
	return this.points.getRow(index);
}

public Mat getPoints(){return this.points;}

public Vec getColor(){return this.color;}

public boolean isLine(){return this.line;}

public int size(){return this.points.size();}

public void setPoint(Vec value, int index){
	this.points.setRow(value, index);
}

public void setColor(Vec color){this.color = color;}

public void setLine(boolean line){this.line = line;}

/*----------------------Basic Shapes----------------------*/
/*static methods that return basic shapes, mostly with sides
of length 1 and with the bottom-most, left-most point at the
origin. All other Poly fields are left as default*/

public static Poly dot(){
	return new Poly(new Mat(new Vec[]{new Vec(0, 0)}), true);
}

public static Poly line(){
	return new Poly(new Mat(new Vec[]{new Vec(0, 0),
		new Vec(2, 0)}), true);
}

public static Poly triangle(){
	return new Poly(new Mat(new Vec[]{new Vec(0, 0), new Vec(1, 0),
		new Vec(1, 1)}));
}

public static Poly square(){
	return new Poly(new Mat(new Vec[]{new Vec(0, 0), new Vec(1, 0),
 		new Vec(1, 1), new Vec(0, 1)}));
}

public static Poly pentagon(){
	return new Poly(new Mat(new Vec[]{new Vec(0, 0), new Vec(1, 0),
		new Vec(1, 1), new Vec(2, 0), new Vec(0, 1)}));
}

public static Poly hexagon(){
	return new Poly(new Mat(new Vec[]{new Vec(0, 0), new Vec(1, 0),
		new Vec(1, 1), new Vec(2, 1), new Vec(2, 0),
		new Vec(0, 1)}));
}

/*----------------------Generic Figures----------------------*/
/*static methods that return generic figures with the
left-most, bottom-most point at the origin*/

public static Poly stickFigure(){
	//head
 	Vec h1 = new Vec(1, 10); Vec h2 = new Vec(2, 10);
 	Vec h3 = new Vec(3, 10); Vec h4 = new Vec(1, 9);
 	Vec h5 = new Vec(2, 9); Vec h6 = new Vec(3, 9);
 	Vec h7 = new Vec(1, 8); Vec h8 = new Vec(2, 8);
 	Vec h9 = new Vec(3, 8);
 	//body
 	Vec b1 = new Vec(2, 7); Vec b2 = new Vec(2, 6);
 	Vec b3 = new Vec(2, 5); Vec b4 = new Vec(2, 4);
 	Vec b5 = new Vec(2, 3); Vec b6 = new Vec(2, 2);
 	//arms
 	Vec la1 = new Vec(0, 5); Vec la2 = new Vec(1, 5);
 	Vec ra1 = new Vec(3, 5); Vec ra2 = new Vec(4, 5);
 	//legs
 	Vec ll1 = new Vec(1, 1); Vec ll2 = new Vec(0, 0);
 	Vec rl1 = new Vec(3, 1); Vec rl2 = new Vec(4, 0);
 	//figure array
 	Vec[] v = new Vec[]{h1, h2, h3, h4, h5, h6, h7, h8, h9, b1, b2,
 		b3, b4, b5, b6, la1, la2, ra1, ra2, ll1, ll2, rl1, rl2};
 	return new Poly(new Mat(v), true); 
}

}