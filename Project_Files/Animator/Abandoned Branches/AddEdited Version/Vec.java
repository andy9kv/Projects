
/*
* Andy Morato
* Vector class holds a list of points (doubles)
* Created July 6th, 2017
*/

public class Vec {

/*-----Instance Variables-----*/

private double[] points;


/*---------------------Constructors-----------------------*/

/*Default Constructor*/
public Vec() { this(0, 0, 0);}

/*initializes a length 3 vector with the specified values.
z is hard-coded a value of 1 to make transfromation
operations much easier (homogeneous coordinates)*/
public Vec(double x, double y){ this(x, y, 1);}

/*initializes a length 3 vector with the specified values*/
public Vec(double x, double y, double z){
	this.points = new double[]{x, y, z};;
}

/*creates a new vector with size points*/
public Vec(int size){ this.points = createVector(size);}

/*copies the given points*/
public Vec(double[] points){
	this.points = copyVector(points);
}

/*copy constructor, copies the only instance variable in
the specifed original to this new instance*/
public Vec(Vec original){
	this.points = copyVector(original.getPoints());
}

/*-------------Mathematical & Logic Functions------------*/

/*compares this with compareWith and
 returns true if equal, false if not*/
public boolean equals(Vec compareWith){
	for(int i = 0; i < this.points.length; i++)
		if(this.points[i] != compareWith.getPoint(i))
			return false;
	return true;
}

/*returns a new vector where:
    points[i] = this.points[i] + addTo.points[i]*/
public Vec add(Vec addTo){
	double[] sum = new double[this.points.length];
	for(int i = 0; i < this.points.length; i++)
		sum[i] = this.points[i] + addTo.getPoint(i);
	return new Vec(sum);
}	

/*returns a new vector where:
    points[i] = this.points[i] - takeAway.points[i]*/
public Vec subtract(Vec takeAway){
	double[] diff = new double[this.points.length];
	for(int i = 0; i < this.points.length; i++)
		diff[i] = this.points[i] - takeAway.getPoint(i);
	return new Vec(diff);
}	

/*returns a new vector where:
	points[i] = this.points[i] * scalar*/
public Vec scale(double scalar){
	double[] scaled = new double[this.points.length];
	for(int i = 0; i < this.points.length; i++)
		scaled[i] = this.points[i] * scalar;
	return new Vec(scaled);
}

/*-------------------Vector Functions--------------------*/

/*returns a double that is the sum of all:
	(this.points[i] * v.points[i])*/
public double dotProduct(Vec v){
	double dp = 0;
	for(int i = 0; i < this.points.length; i++)
		dp += (this.points[i] * v.getPoint(i));
	return dp;
}

/*returns the cross product between this.points & v.points*/
public Vec crossProduct(){
	return new Vec();
}


/*----------------------Other Methods--------------------*/

/*copies and returns the array of doubles in the parameter*/
public static double[] copyVector(double[] original){
	double[] copy = new double[original.length];
	for(int i = 0; i < original.length; i++)
		copy[i] = original[i];
	return copy;
}

/*creates a new vector with the specified size and
initializes all values to zero*/
private double[] createVector(int size){
	double[] creation = new double[size];
	for(int i = 0; i < size; i++)
		creation[i] = 0;
	return creation;
}

/*returns this vector represented as a String. Double values
are extended or compressed to exactly two deciaml places*/
public String toString(){
	String vec = "[" + String.format("%.2f", this.points[0]);
	for(int i = 1; i < this.points.length; i++)
		vec += ", " + String.format("%.2f", this.points[i]);
	vec += "]\n";
	return vec;
}

/*prints the toString representation of this vector*/
public void print(){ System.out.print(toString());}

/*prints the vector is a similar fashion as this.print()
but displayed vertically rather than horizontally*/
public void printVertical(){
	System.out.println("|" + points[0] + "|");
	System.out.println("|" + points[1] + "|");
	System.out.println("|" + points[2] + "|\n");
}


/*--------------------Getters & Setters-------------------*/

public double getPoint(int index){ return points[index];}

public double[] getPoints(){ return points;}

public void setPoint(double value, int index){
	points[index] = value;
}

public void setPoints(double[] points){
	this.points = points;
}

public int size(){ return this.points.length;}

}