#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include "utility.h"

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

using namespace std;


class Triangle{
    public:
    Point a,b,c;
    Point auv,buv,cuv;
    Triangle();
    Triangle(Point, Point, Point);
    void draw();
    Point midpoint();
    void print();
};


class Patch{
	public:
	Point points[4][4];
	Patch(Point[4][4]);
	vector<Triangle> triangles;
	Patch(PointMatrix);
	Ray interpolate(float, float);
	void uSubDivide(float);
	void aSubDivide(float);
	void draw();
	Point midpoint();
};


class Model{
	public:
	vector<Patch> patches;
    
	Color color;
	Model();
	Model(vector<Patch>, Color);
	void draw();
	void uSubDivide(float);
	void aSubDivide(float);
};


#endif
