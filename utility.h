#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <math.h>
using namespace std;

class Color;
class Point;
class Vector;
class Ray;


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
	void print();
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


#endif
