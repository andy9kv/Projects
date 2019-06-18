
/*
* Andrew Morato
* mat3.cpp
* 02/09/17
*/

#include "mat3.h"

//Constructors---------------

//Initializes to a matrix of all 0's
mat3::mat3() {
	data[0] = vec3::vec3();
	data[1] = vec3::vec3();
	data[2] = vec3::vec3();
}

//Initializes to a matrix with the vectors in the parameters
mat3::mat3(const vec3& row1, const vec3& row2, const vec3& row3) {
	data[0] = row1;
	data[1] = row2;
	data[2] = row3;
}

//Getters----------------

vec3 mat3::operator[](unsigned int index) const {
	assert(index >= 0 && index <= 2);
	return data[index];
}

vec3& mat3::operator[](unsigned int index) {
	assert(index >= 0 && index <= 2);
	vec3& v = data[index];
	return v;
}

//Returns the specifed column vector
vec3 mat3::getColumn(unsigned int index) const {
	assert(index >= 0 && index <= 2);
	vec3 col = vec3::vec3();
	for (int i = 0; i < 3; i = i + 1)
		col[i] = data[i][index];
	return col;
}

/// Returns the float at 'index', indexes are as follows:
//matrix = 0 1 2
//         3 4 5
//         6 7 8
float mat3::getValueAtIndex(int index) {
	assert(index >= 0 && index < 9);
	if (index < 3)
		return data[0].getValueAtIndex(index);
	if (index < 6)
		return data[1].getValueAtIndex(index % 3);
	return data[2].getValueAtIndex(index % 3);
}

//Methods----------------

//prints everything out nicely
void mat3::print() {
	for (int i = 0; i < 3; i = i + 1)
		data[i].print();
}

//Returns the transpose of the matrix
mat3 mat3::transpose() const {
	mat3 m = mat3();
	for (int i = 0; i < 3; i = i + 1)
		m[i] = getColumn(i);
	return m;
}

//Statics----------------

//Takes an angle in degrees and outputs a 3x3 rotation matrix
mat3 mat3::rotation2D(float angle) {
	float deg = 180;
	float rads = (angle * PI / deg);
	vec3 v1 = vec3(cos(rads), -sin(rads), 0);
	vec3 v2 = vec3(sin(rads), cos(rads), 0);
	vec3 v3 = vec3(0, 0, 1);
	mat3 rm = mat3(v1, v2, v3);
	return rm;
}

//Takes x & y displacement and outputs a 3x3 translation matrix
mat3 mat3::translation2D(float x, float y) {
	vec3 v1 = vec3(1, 0, x);
	vec3 v2 = vec3(0, 1, y);
	vec3 v3 = vec3(0, 0, 1);
	mat3 tm = mat3(v1, v2, v3);
	return tm;
}

//Takes x & y and outputs a 3x3 scale matrix
mat3 mat3::scale2D(float x, float y) {
	vec3 v1 = vec3(x, 0, 0);
	vec3 v2 = vec3(0, y, 0);
	vec3 v3 = vec3(0, 0, 1);
	mat3 sm = mat3(v1, v2, v3);
	return sm;
}

//Generates a 3x3 identity matrix
mat3 mat3::identity() {
	vec3 v1 = vec3(1, 0, 0);
	vec3 v2 = vec3(0, 1, 0);
	vec3 v3 = vec3(0, 0, 1);
	mat3 im = mat3(v1, v2, v3);
	return im;
}

//Friends----------------

//checks if m1 == m2
bool operator==(const mat3& m1, const mat3& m2) {
	return m1[0] == m2[0] && m1[1] == m2[1] && m1[2] == m2[2];
}

//checks if m1 != m2
bool operator!=(const mat3& m1, const mat3& m2) {
	return m1[0] != m2[0] && m1[1] != m2[1] && m1[2] != m2[2];
}

//matrix addition (m1 + m2)
mat3 operator+(const mat3& m1, const mat3& m2) {
	mat3 m = mat3();
	for (int i = 0; i < 3; i = i + 1)
		m[i] = m1[i] + m2[i];
	return m;
}

//matrix subtraction (m1 - m2)
mat3 operator-(const mat3& m1, const mat3& m2) {
	mat3 m = mat3();
	for (int i = 0; i < 3; i = i + 1)
		m[i] = m1[i] - m2[i];
	return m;
}

//scalar multiplication (m * c)
mat3 operator*(const mat3& m, float c) {
	mat3 p = mat3();
	for (int i = 0; i < 3; i = i + 1)
		p[i] = m[i] * c;
	return p;
}

//scalar multiplication (c * m)
mat3 operator*(float c, const mat3& m) {
	mat3 p = mat3();
	for (int i = 0; i < 3; i = i + 1)
		p[i] = c * m[i];
	return p;
}

//scalar division (m/c)
mat3 operator/(const mat3& m, float c) {
	assert(c != 0);
	mat3 q = mat3();
	for (int i = 0; i < 3; i = i + 1)
		q[i] = m[i]/c;
	return q;
}

//matrix multiplication (m1 * m2)
mat3 operator*(const mat3& m1, const mat3& m2) {
	mat3 p = mat3();
	for (int i = 0; i < 3; i = i + 1)
		p[i] = vec3(m1[i]*m2.getColumn(0), m1[i] * m2.getColumn(1), m1[i] * m2.getColumn(2));
	return p;
}

//matrix vector multiplication (m * v)
//assume v is a column vector (ie. a 3x1 matrix)
vec3 operator*(const mat3& m, const vec3& v) {
	vec3 p = vec3(m[0] * v, m[1] * v, m[2] * v);
	return p;
}

//matrix vector multiplication (v * m)
//assume v is a row vector (ie. a 1x3 matrix)
vec3 operator*(const vec3& v, const mat3& m) {
	vec3 p = vec3(v*m.getColumn(0), v*m.getColumn(1), v*m.getColumn(2));
	return p;
}

/*Testing----------------
* NOTE: Every method except for length() and the cross product operation 
* in vec3.cpp are used through the methods of mat3.cpp, therefore the only
* explicit tests for vec3 will be for the two methods
*/

/*
int main() {
	vec3 v1 = vec3::vec3(1,2,3);
	vec3 v2 = vec3::vec3(4,5,6);
	vec3 v3 = vec3::vec3(7,8,9);
	mat3 m1 = mat3::mat3(v1,v2,v3);

	vec3 v4 = vec3::vec3(12, 14, 16);
	vec3 v5 = vec3::vec3(18, 20, 22);
	vec3 v6 = vec3::vec3(24, 26, 28);
	mat3 m2 = mat3::mat3(v4, v5, v6);

	vec3 v7 = vec3(2, 3, 4);
	float divideBy = 2;
	float multiplyBy = 3;

	std::cout << "-----vec3 Tests-----" << std::endl << std::endl;

	std::cout << "Vector 1:" << std::endl;
	v1.print();
	std::cout << std::endl << "Vector 2:" << std::endl;
	v2.print(); std::cout << std::endl;

	//Length Function Test
	std::cout << "The Length of the vector [2, 3, 4] should be 5.38516 and is calculated to be: " << v7.length() << std::endl;
	//Cross Product Test
	std::cout << "The Cross Product of Vector 1 and Vector 2 should be: [-3, 6, -3] and is calculated to be: ";
	(v1 % v2).print(); std::cout << std::endl;

	std::cout << "-----mat3 Tests-----" << std::endl << std::endl;

	std::cout << "Matrix 1:" << std::endl;
	m1.print();
	std::cout << std::endl << "Matrix 2:" << std::endl;
	m2.print(); std::cout << std::endl;

	//getColumn Test
	std::cout << "Column 1 of Matrix 1 is [2, 5, 8] and is calculated as: ";
	m1.getColumn(1).print(); std::cout << std::endl;

	//Transpose Test
	std::cout << "The Transpose of Matrix 1 should be:" << std::endl << "[1, 4, 7]" << std::endl << "[2, 5, 8]" << std::endl << "[3, 6, 9]" << std::endl << "and is calculated as: " << std::endl;
	m1.transpose().print(); std::cout << std::endl;

	//Addition Test
	std::cout << "The Sum of Matrix 1 and Matrix 2 should be:" << std::endl << "[13, 16, 19]" << std::endl << "[22, 25, 28]" << std::endl << "[31, 34, 37]" << std::endl << "and is calculated as: " << std::endl;
	(m1+m2).print(); std::cout << std::endl;

	//Subtraction Test
	std::cout << "The Diference of Matrix 2 and Matrix 1 should be:" << std::endl << "[11, 12, 13]" << std::endl << "[14, 15, 16]" << std::endl << "[17, 18, 19]" << std::endl << "and is calculated as: " << std::endl;
	(m2 - m1).print(); std::cout << std::endl;

	//Scalar Multiplication Tests
	std::cout << "The Scalar Product of Matrix 1 and 3 should be:" << std::endl << "[3, 6, 9]" << std::endl << "[12, 15, 18]" << std::endl << "[21, 24, 27]" << std::endl << "and is calculated as: " << std::endl;
	(m1 * multiplyBy).print(); std::cout << std::endl;

	std::cout << "The Scalar Product of 3 and Matrix 1 should be(the same as above):" << std::endl << "[3, 6, 9]" << std::endl << "[12, 15, 18]" << std::endl << "[21, 24, 27]" << std::endl << "and is calculated as: " << std::endl;
	(multiplyBy * m1).print(); std::cout << std::endl;

	//Scalar Division Test
	std::cout << "The Scalar Quotient of Matrix 2 and 2 should be:" << std::endl << "[6, 7, 8]" << std::endl << "[9, 10, 11]" << std::endl << "[12, 13, 14]" << std::endl << "and is calculated as: " << std::endl;
	(m2 / divideBy).print(); std::cout << std::endl;

	//Matrix Multiplication Test
	std::cout << "The Product of Matrix 1 and Matrix 2 should be:" << std::endl << "[120, 132, 144]" << std::endl << "[282, 312, 342]" << std::endl << "[444, 492, 540]" << std::endl << "and is calculated as: " << std::endl;
	(m1 * m2).print(); std::cout << std::endl;

	//Vector Multiplication Tests
	std::cout << "The (Column) Vector Product of Matrix 1 and Vector 1 should be [14, 32, 50] and is calculated as: ";
	(m1 * v1).print(); std::cout << std::endl;

	std::cout << "The (Row) Vector Product of Matrix 1 and Vector 1 should be [30, 36, 42] and is calculated as: ";
	(v1 * m1).print(); std::cout << std::endl;

	//Rotation Test
	std::cout << "With angle 20 (in degrees), the Rotation Matrix produced should be:" << std::endl << "[0.9397, -0.3420, 0.0000]" << std::endl << "[0.3420, 0.9397, 0.0000]" << std::endl << "[0.0000, 0.0000, 1.0000]" << std::endl << "and is calculated as: " << std::endl;
	(rotation2D(20)).print(); std::cout << std::endl;

	//Scale Test
	std::cout << "With x = 5 and y = 7, the Scale Matrix produced should be:" << std::endl << "[5, 0, 0]" << std::endl << "[0, 7, 0]" << std::endl << "[0, 0, 1]" << std::endl << "and is calculated as: " << std::endl;
	(scale2D(5, 7)).print(); std::cout << std::endl;

	//Translation Test
	std::cout << "With x = 5 and y = 7, the Translation Matrix produced should be:" << std::endl << "[1, 0, 5]" << std::endl << "[0, 1, 7]" << std::endl << "[0, 0, 1]" << std::endl << "and is calculated as: " << std::endl;
	(translation2D(5, 7)).print(); std::cout << std::endl;

	//Scale Test
	std::cout << "The Identity Matrix multiplied with anything, in this case m1, should produce the same thing(m1):" << std::endl << "[1, 2, 3]" << std::endl << "[4, 5, 6]" << std::endl << "[7, 8, 9]" << std::endl << "It is calculated as: " << std::endl;
	(identity() * m1).print(); std::cout << std::endl;

	system("PAUSE");
}
*/