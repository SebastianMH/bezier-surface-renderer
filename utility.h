#ifndef UTILITY_H
#define UTILITY_H


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
	Point(float, float, float);
};


class Vector{
	public:
	float x,y,z;
	Vector();
	Vector(float, float, float);
};


class Ray{
	public:
	Point point;
	Vector vector;
	Ray();
	Ray(Point, Vector);
};


#endif
