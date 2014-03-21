#include "bezier.h"


Point::Point(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}


Curve::Curve(Point aa, Point bb, Point cc, Point dd){
	a = aa;
	b = bb;
	c = cc;
	d = dd;
}
void Curve::interpolate(float u){}


Patch::Patch(Curve aa, Curve bb, Curve cc, Curve dd){
	a = aa;
	b = bb;
	c = cc;
	d = dd;
}
void interpolate(float u, float v){}
void Patch::subDivide(float step){}
