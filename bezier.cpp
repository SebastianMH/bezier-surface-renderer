#include "bezier.h"

Point::Point(){ }

Color::Color(float rr, float gg, float bb){
	r = rr;
	g = gg;
	b = bb;
}


Point::Point(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}

Curve::Curve() { }

Vector::Vector(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}


Ray::Ray(Point p, Vector v){
	point = p;
	vector = v;
}


/*
Curve::Curve(Point aa, Point bb, Point cc, Point dd){
	a = aa;
	b = bb;
	c = cc;
	d = dd;
}


Ray Curve::interpolate(float u){
	// first, split each of the three segments
	// to form two new ones AB and BC
	A = a * (1.0-u) + b * u;
	B = b * (1.0-u) + c * u;
	C = c * (1.0-u) + d * u;
	// now, split AB and BC to form a new segment DE
	D = A * (1.0-u) + B * u;
	E = B * (1.0-u) + C * u;
	// finally, pick the right point on DE,
	// this is the point on the curve
	p = D * (1.0-u) + E * u;
	// compute derivative also
	dPdu = 3 * (E - D);
	
	return Ray(p, dPdu);
}


Patch::Patch(Curve aa, Curve bb, Curve cc, Curve dd){
	a = aa;
	b = bb;
	c = cc;
	d = dd;
}
void interpolate(float u, float v){
	// build control points for a Bezier curve in v
	vcurve.a = a.bezcurveinterp(patch[0][0:3], u).point;
	vcurve.b = b.bezcurveinterp(patch[1][0:3], u).point;
	vcurve.c = c.bezcurveinterp(patch[2][0:3], u).point;
	vcurve.d = d.bezcurveinterp(patch[3][0:3], u).point;
	// build control points for a Bezier curve in u
	ucurve.a = bezcurveinterp(patch[0:3][0], v).point;
	ucurve.b = bezcurveinterp(patch[0:3][1], v).point;
	ucurve.c = bezcurveinterp(patch[0:3][2], v).point;
	ucurve.d = bezcurveinterp(patch[0:3][3], v).point;

	// evaluate surface and derivative for u and v
	p, dPdv = vcurve.bezcurveinterp(v)
	p, dPdu = ucurve.bezcurveinterp(u)
	// take cross product of partials to find normal
	n = cross(dPdu, dPdv)
	n = n / length(n)
return Ray(p, n);





}
void Patch::subDivide(float step){}
*/


Model::Model(vector<Point[][]> p, Color c){
	patches = p;
	color = c;
};


void Model::draw(){}





