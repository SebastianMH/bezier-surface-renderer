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

Point::Point(Vector v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

Vector Point::operator-(Point& p)
{return Vector(x-p.x, y-p.y, z-p.z);}

Point Point::operator+(Point& p)
{return Point(x+p.x, y+p.y, z+p.z);}

Point Point::operator+(Vector& v)
{return Point(x+v.x, y+v.y, z+v.z);}

Point Point::operator-(Vector& v)
{return Point(x-v.x, y-v.y, z-v.z);}

Point Point::operator*(float scale)
{return Point(x*scale,y*scale,z*scale);}

Point Point::cross(Point p) {
    return Point(((y*p.z)-(z*p.y)), ((z*p.x)-(x*p.z)), ((x*p.y)-(y*p.x)));
}

Point Point::normalize(){
    float length = sqrt((x*x) + (y*y) + (z*z));
    return Point(x/length, y/length, z/length);
}

void Point::print() {
    cout << "(" << x << ", " << y << ", " << z << ")";
}



















Vector::Vector(){}
Vector::Vector(float xx, float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
}

Vector::Vector(Point p1, Point p2) {
    x = p1.x-p2.x;
    y = p1.y-p2.y;
    z = p1.z-p2.z;
}

Vector Vector::operator*(float scale) {return Vector(x*scale, y*scale, z*scale);}

float Vector::dot(Vector v){return (x * v.x) + (y * v.y) + (z * v.z);}

Vector Vector::operator+(Vector& v){return Vector(x+v.x, y+v.y, z+v.z);}

Vector Vector::operator-(Vector& v){return Vector(x-v.x, y-v.y, z-v.z);}

Point Vector::operator+(Point& p){return Point(x+p.x, y+p.y, z+p.z);}

Point Vector::operator-(Point& p){return Point(x-p.x, y-p.y, z-p.z);}

Vector Vector::cross(Vector v) {return Vector(((y*v.z)-(z*v.y)), ((z*v.x)-(x*v.z)), ((x*v.y)-(y*v.x)));}

Vector Vector::normalize(){
    float length = sqrt((x*x) + (y*y) + (z*z));
    return Vector(x/length, y/length, z/length);
}

void Vector::print(){
    cout << "[" << x << ", " << y << ", " << z << "]";
}











Ray::Ray(){}
Ray::Ray(Point p, Vector v){
	point = p;
	vector = v;
}
