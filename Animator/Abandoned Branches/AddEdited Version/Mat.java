
/*
* Andy Morato
* Matrix class holds a list of vectors
* Created July 6th, 2017
*/

import java.lang.Math;

public class Mat {

/*-----Instance Variables-----*/

private Vec[] vectors;

/*---------------------Constructors-----------------------*/

/*Default Constructor*/
public Mat(){ this(new Vec(), new Vec(), new Vec());}

/*initializes a length 3 matrix with the specified vectors*/
public Mat(Vec v1, Vec v2, Vec v3){
	this.vectors = createMatrix(v1, v2, v3);
}

/*creates a new matrix with size vectors*/
public Mat(int size){ this.vectors = new Vec[size];}

/*copies the vectors*/
public Mat(Vec[] vectors){
	this.vectors = copyMatrix(vectors);
}

/*copy constructor that copies the vectors*/
public Mat(Mat original){ this(original.getVectors());}

/*--------------Mathematical & Logic Functions------------*/

/*compares this with compareWith and returns 
true if equal, false if not*/
public boolean equals(Mat compareWith){
	for(int i = 0; i < this.vectors.length; i++)
		if(!(this.vectors[i].equals(compareWith.vectors[i])))
			return false;
	return true;
}

/*returns a new matrix where:
    points[i][j] = this.points[i][j] + addTo.points[i][j]*/
public Mat add(Mat addTo){
	Vec[] sum = new Vec[this.vectors.length];
	for(int i = 0; i < this.vectors.length; i++)
		sum[i] = this.vectors[i].add(addTo.getRow(i));
	return new Mat(sum);
}

/*returns a new matrix where:
    points[i][j] = this.points[i][j] - takeAway.points[i][j]*/
public Mat subtract(Mat takeAway){
	Vec[] diff = new Vec[this.vectors.length];
	for(int i = 0; i < this.vectors.length; i++)
		diff[i] = this.vectors[i].subtract(takeAway.getRow(i));
	return new Mat(diff);
}

/*returns a new matrix where:
	points[i][j] = this.points[i][j] * scalar*/
public Mat scale(double scalar){
	Vec[] scaled = new Vec[this.vectors.length];
	for(int i = 0; i < this.vectors.length; i++)
		scaled[i] = this.vectors[i].scale(scalar);
	return new Mat(scaled);
}

/*-------------------Matrix Functions--------------------*/

/*Carries out matrix multiplication if the matricies are
of compatible size. NOTE: Currently, this operation is only
supported for matricies of size 3*/
public Mat multiply(Mat multiplyWith){
	Mat product = new Mat(3);
	for(int i = 0; i < 3; i++){
		Vec v = new Vec(vectors[i].dotProduct(multiplyWith.getColumn(0)), vectors[i].dotProduct(multiplyWith.getColumn(1)), vectors[i].dotProduct(multiplyWith.getColumn(2)));
		product.setRow(v, i);
	}
	return product;
}

/*Multiplies this matrix with the specified vector if they
are of appropriate size. Multiplication happens of the form
matrix * vector rather than vector * matrix. NOTE: Currently,
this operation is only supported for matricies of size 3*/
public Vec multiply(Vec multiplyWith){
	double d1 = vectors[0].dotProduct(multiplyWith);
	double d2 = vectors[1].dotProduct(multiplyWith);
	double d3 = vectors[2].dotProduct(multiplyWith);
	return new Vec(d1, d2, d3);
}

/*Returns the transpose of this matrix*/
public Mat transpose(){
	Vec[] tp = new Vec[this.vectors[0].size()];
	for(int i = 0; i < tp.length; i++)
		tp[i] = new Vec(Vec.copyVector(getColumn(i).getPoints()));
	return new Mat(tp);
}

/*---------------Transformation Functions-----------------*/
/*These methods produce transformation matrices to multiply
with and transform any appropriate sized matrix accordingly.
The sizes of the transformation matricies are 3 x 3*/

/*produces and returns an identity matrix of appropriate size*/
public static Mat identityMatrix(){
	Vec v1 = new Vec(1, 0, 0);
	Vec v2 = new Vec(0, 1, 0);
	Vec v3 = new Vec(0, 0, 1);
	return new Mat(v1, v2, v3);
}

/*produces and returns a 2D rotation matrix with the 
specified rotation*/
public static Mat rotationMatrix(double angle){
	double rads = Math.toRadians(angle);
	Vec v1 = new Vec(Math.cos(rads), -(Math.sin(rads)), 0);
	Vec v2 = new Vec(Math.sin(rads), Math.cos(rads), 0);
	Vec v3 = new Vec(0, 0, 1);
	return new Mat(v1, v2, v3);
}

/*produces and returns a 2D translation matrix with the 
specified translation*/
public static Mat translationMatrix(double x, double y){
	Vec v1 = new Vec(1, 0, x);
	Vec v2 = new Vec(0, 1, y);
	Vec v3 = new Vec(0, 0, 1);
	return new Mat(v1, v2, v3);
}

/*produces and returns a 2D scale matrix with the 
specified scale*/
public static Mat scaleMatrix(double x, double y){
	Vec v1 = new Vec(x, 0, 0);
	Vec v2 = new Vec(0, y, 0);
	Vec v3 = new Vec(0, 0, 1);
	return new Mat(v1, v2, v3);
}

/*and overloaded version of the previous method,
scaleMatrix(). Rather than accept two parameters, it
duplicates one as both.*/
public static Mat scaleMatrix(double scale){
	return scaleMatrix(scale, scale);
}


/*----------------------Other Methods---------------------*/

/*copies and returns the array of vectors in the parameter*/
public static Vec[] copyMatrix(Vec[] original){
	Vec[] copy = new Vec[original.length];
	for(int i = 0; i < original.length; i++)
		copy[i] = new Vec(Vec.copyVector(original[i].getPoints()));
	return copy;
}

/*returns a length 3 matrix with the specified vectors*/
private Vec[] createMatrix(Vec v1, Vec v2, Vec v3){ 
	Vec[] vectors = new Vec[3];
	vectors[0] = new Vec(Vec.copyVector(v1.getPoints()));
	vectors[1] = new Vec(Vec.copyVector(v2.getPoints()));
	vectors[2] = new Vec(Vec.copyVector(v3.getPoints()));
	return vectors;
}

/*returns this matrix represented as a String*/
public String toString(){
	String mat = "";
	for(int i = 0; i < this.vectors.length; i++)
		mat += this.vectors[i].toString();
	return mat;
}

/*prints the toString representation of this matrix*/
public void print(){ System.out.print(toString());}

/*appends a row (vector) to the matrix, costly 0(n)
procedure for simply adding a vector to the matrix*/
public void append(Vec row){
	Vec[] appended = new Vec[vectors.length + 1];
	for(int i = 0; i < vectors.length; i++)
		appended[i] = vectors[i];
	appended[vectors.length] = row;
	this.vectors = appended;
}


/*--------------------Getters & Setters-------------------*/

public Vec[] getVectors(){ return vectors;}

/*Returns the vector at the specified index*/
public Vec getRow(int row){ return vectors[row];}

/*Returns the column of values at the specified index*/
public Vec getColumn(int col){
	Vec column = new Vec(this.vectors.length);
	for(int i = 0; i < this.vectors.length; i++)
		column.setPoint(vectors[i].getPoint(col), i);
	return column;
}

/*Returns the index in this matrix in the following from:
	0  1  2
	3  4  5
	6  7  8*/
public double getValueAtIndex(int index){
	int row = index % vectors.length;
	int col = index / vectors.length;
	return this.vectors[row].getPoint(col);
}

public void setRow(Vec row, int index){ vectors[index] = row;}

public int size(){ return vectors.length;}
}