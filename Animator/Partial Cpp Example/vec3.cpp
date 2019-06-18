
/*
* Andrew Morato
* vec3.cpp
* 02/09/17
*/

#include "vec3.h"

//Constructors---------------

//constructor which initializes data to 0
vec3::vec3() {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
}

//constructer which initializes data to parameters
vec3::vec3(float x, float y, float z) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

//Getters/Setters---------------

float vec3::operator[](unsigned int index) const {
	assert(index >= 0 && index <= 2);
	return data[index];
}

float& vec3::operator[](unsigned int index) {
	assert(index >= 0 && index <= 2);
	float& r = data[index];
	return r;
}

float vec3::getValueAtIndex(int index) {
	assert(index >= 0 && index <= 2);
	if (index == 0)
		return data[0];
	if (index == 1)
		return data[1];
	return data[2];
}

//Methods----------------

//simply prints the value of the components of the vector
void vec3::print() {
	using namespace std;
	cout << std::fixed;
	cout << std::setprecision(4);
	cout << "[" << data[0] << ", " << data[1] << ", " << data[2] << "]" << endl;
}

//computes the length i.e. |x| = sqrt(v1^2 + v2^2 + v3^2)
float vec3::length() const {
	return sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
}

//Friends---------------

//checks if v1 == v2
bool operator==(const vec3& v1, const vec3& v2) {
	if ((abs(v1[0] - v2[0]) < EPSILON) && (abs(v1[1] - v2[1]) < EPSILON) && (abs(v1[2] - v2[2]) < EPSILON))
		return true;
	return false;
}

//checks if v1 != v2
bool operator!=(const vec3& v1, const vec3& v2) {
	if ((abs(v1[0] - v2[0]) < EPSILON) && (abs(v1[1] - v2[1]) < EPSILON) && (abs(v1[2] - v2[2]) < EPSILON))
		return false;
	return true;
}

//vector addition (v1 + v2)
vec3 operator+(const vec3& v1, const vec3& v2) {
	vec3 v = vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
	return v;
}

//vector subtraction (v1 - v2)
vec3 operator-(const vec3& v1, const vec3& v2) {
	vec3 v = vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
	return v;
}

//scalar multiplication (v * c)
vec3 operator*(const vec3& v, float c) {
	vec3 v1 = vec3(v[0] * c, v[1] * c, v[2] * c);
	return v1;
}

//scalar multiplication (c * v)
vec3 operator*(float c, const vec3& v) {
	vec3 v1 = vec3(v[0] * c, v[1] * c, v[2] * c);
	return v1;
}

//scalar divsion (v/c)
vec3 operator/(const vec3& v, float c) {
	assert(c != 0);
	vec3 v1 = vec3(v[0]/c, v[1]/c, v[2]/c);
	return v1;
}

//dot product (v1 * v2)
float operator*(const vec3& v1, const vec3& v2) {
	return (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]);
}

//cross product (v1 % v2)
vec3 operator%(const vec3& v1, const vec3& v2) {
	vec3 v = vec3();
	v[0] = (v1[1] * v2[2] - v1[2] * v2[1]);
	v[1] = (v1[2] * v2[0] - v1[0] * v2[2]);
	v[2] = (v1[0] * v2[1] - v1[1] * v2[0]);
	return v;
}
