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
	Point cross(Point);
	Point normalize();
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


class Matrix {

    public:
        Matrix(float[][4]);
        Matrix(float[][3]);
        Matrix(float[][2]);
        Matrix();
        
        float matrix[4][4];
        int size;
        
        Matrix multiply(Matrix);
        float determinant();
        Matrix inverse_of_3Dtransformation();
        Matrix transpose();
        void print();
        
        
        static Matrix create_translation_matrix(float tx, float ty, float tz) {
            float matrix[4][4] = {{1, 0, 0, tx},
                                  {0, 1, 0, ty},
                                  {0, 0, 1, tz},
                                  {0, 0, 0, 1}};
            return Matrix(matrix);
        }

        static Matrix create_scale_matrix(float sx, float sy, float sz) {
            float matrix[4][4] = {{sx, 0, 0, 0},
                                  {0, sy, 0, 0},
                                  {0, 0, sz, 0},
                                  {0, 0, 0, 1}};
            return Matrix(matrix);
        }

        // angles in degrees
        static Matrix create_rotation_matrix(float angle_x, float angle_y, float angle_z) {
            angle_x = (PI/180)*angle_x;
            angle_y = (PI/180)*angle_y;
            angle_z = (PI/180)*angle_z;
            
            float Rx[4][4] = {{1, 0, 0, 0},
                              {0, cos(-angle_x), -sin(-angle_x), 0},
                              {0, sin(-angle_x), cos(-angle_x), 0},
                              {0, 0, 0, 1}};
            
            float Ry[4][4] = {{cos(-angle_y), 0, sin(-angle_y), 0},
                              {0, 1, 0, 0},
                              {-sin(-angle_y), 0, cos(-angle_y), 0},
                              {0, 0, 0, 1}};

            float Rz[4][4] = {{cos(-angle_z), -sin(-angle_z), 0, 0},
                              {sin(-angle_z), cos(-angle_z), 0, 0},
                              {0, 0, 1, 0},
                              {0, 0, 0, 1}};
           return (Matrix(Rx).multiply(Matrix(Ry))).multiply(Matrix(Rz));
            
        }
        
        static Matrix create_identity_matrix() {
            float ident[4][4] = {{1, 0, 0, 0},
                                 {0, 1, 0, 0},
                                 {0, 0, 1, 0},
                                 {0, 0, 0, 1}};
            return Matrix(ident);
        }
};
#endif
