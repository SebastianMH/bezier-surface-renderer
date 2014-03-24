#ifndef BEZIER_H
#define BEZIER_H


class Color{
	public:
	float r, g, b;
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
	Vector(float, float, float);
};


class Ray{
	public:
	Point point;
	Vector vector;
	Ray(Point, Vector);
};

/*
class Curve{
	public:
	Point a,b,c,d;
	Curve();
	Curve(Point, Point, Point, Point);
	Ray interpolate(float);
};


class Patch{
	public:
	Curve a,b,c,d;
	Patch(Curve, Curve, Curve, Curve);
	void interpolate(float, float);
	void subDivide(float);
	void draw();
};
*/

class Model{
	public:
	vector<Point[][]> patches;
	Color color;
	Model(vector<[][]>, Color);
	void draw();
};


#endif
