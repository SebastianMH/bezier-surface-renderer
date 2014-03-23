#ifndef BEZIER_H
#define BEZIER_H


class Point{
	public:
	float x,y,z;
	Point();
	Point(float, float, float);
};


class Curve{
	public:
	Point a,b,c,d;
	Curve();
	Curve(Point, Point, Point, Point);
	void interpolate(float);
};


class Patch{
	public:
	Curve a,b,c,d;
	Patch(Curve, Curve, Curve, Curve);
	void interpolate(float, float);
	void subDivide(float);
};


#endif
