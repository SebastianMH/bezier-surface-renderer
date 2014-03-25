#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include "utility.h"

using namespace std;

/*
class Curve{
	public:
	Point a,b,c,d;
	Curve();
	Curve(Point, Point, Point, Point);
	Ray interpolate(float);
};

*/
class Patch{
	public:
	Point points[4][4];
	Patch(Point[4][4]);
	Ray interpolate(float, float);
	//void subDivide(float);
	void draw();
};



class Model{
	public:
	vector<Patch> patches;
	Color color;
	Model(vector<Patch>, Color);
	void draw();
};


#endif
