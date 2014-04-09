#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <math.h>
using namespace std;

class Color;
class Point;
class Vector;
class Ray;
class Matrix;
class PointMatrix;


class Color{
	public:
	float r, g, b;
	Color();
	Color(float, float, float);
};


class Point{
	public:
	float x,y,z;
	Point();
	Point(Vector);
	Point(float, float, float);
	Vector operator-(Point&);
	Point operator+(Point&);
	Point operator-(Vector&);
	Point operator+(Vector&);
	Point operator*(float);
	Point operator*(Point);
	Point cross(Point);
	Point normalize();
	void print();
	float distance(Point);
};


class Vector {
    public:
	float x, y, z;
	Vector();
	Vector(Point, Point);
	Vector(float, float, float);
	float dot(Vector);
	Vector operator*(float);
	Vector operator+(Vector&);
	Vector operator-(Vector&);
	Point operator+(Point&);
	Point operator-(Point&);
	Vector normalize();
	Vector cross(Vector);
	void print();
};


class Ray{
	public:
	Point point;
	Vector vector;
	Ray();
	Ray(Point, Vector);
};


class Matrix {
	public:
	Matrix();
    Matrix(float[][4]);
        
	float matrix[4][4];

	Matrix mult(Matrix&);
	PointMatrix mult(PointMatrix);
	float determinant();
	Matrix transpose();
	void print();
        
	static Matrix create_identity_matrix() {
		float ident[4][4] = {{1, 0, 0, 0},
                             {0, 1, 0, 0},
                             {0, 0, 1, 0},
                             {0, 0, 0, 1}};
	return Matrix(ident);
	}
};


class PointMatrix {
	public:
	PointMatrix();
    PointMatrix(Point[][4]);
        
	Point matrix[4][4];

	PointMatrix mult(Matrix);
	PointMatrix mult(PointMatrix);
	Matrix transpose();
};


#endif
