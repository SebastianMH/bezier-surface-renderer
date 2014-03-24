#include "utility.h"


Color::Color(){}
Color::Color(float rr, float gg, float bb){
	r = rr;
	g = gg;
	b = bb;
}


Point::Point(){}
Point::Point(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}


Vector::Vector(){}
Vector::Vector(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}


Ray::Ray(){}
Ray::Ray(Point p, Vector v){
	point = p;
	vector = v;
}
