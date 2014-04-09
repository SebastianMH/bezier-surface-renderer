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

Point Point::operator*(Point p)
{return Point(x*p.x,y*p.y,z*p.z);}

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

float Point::distance(Point p){
	float dx = x-p.x;
	float dy = y-p.y;
	float dz = z-p.z;
	return sqrt( dx*dx + dy*dy + dz*dz );
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

Matrix::Matrix(float imatrix[][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrix[i][j] = imatrix[i][j];
}

Matrix::Matrix() {}

Matrix Matrix::mult(Matrix& matrix2) {
    float result[4][4];
    for(int i = 0; i != 4; ++i) {
		for(int j = 0; j != 4; ++j) {
			float sum = 0;
			for (int k = 0; k != 4; ++k) {
				sum += matrix[i][k] * matrix2.matrix[k][j];
			}
			result[i][j] = sum;
		}
	}
    return Matrix(result);
}

void Matrix::print() {
    for (int i = 0; i < 4; i++) {
        cout << "[ ";
        for (int j = 0; j < 4; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "]\n";
    }
}

float Matrix::determinant() {
    for(int x = 0; x < 4; x++){
        for(int j = 0; j < 4; j++){
            if(j > x){
                float divider = matrix[j][x]/matrix[x][x];
                for(int k = 0; k < 4; k++){
                    matrix[j][k] = matrix[j][k] - (divider * matrix[x][k]);
                }
            }
        }
    }
    
    float det = 1;
    for(int i = 0; i < 4; i++)
        det = det * matrix[i][i];
    return det;
}


Matrix Matrix::transpose(){
    float matrixT[4][4];
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            matrixT[j][i] = matrix[i][j];
        }
    }
    Matrix mT(matrixT);
    return mT;
}

PointMatrix Matrix::mult(PointMatrix matrix2) {
    Point result[4][4];
    for(int i = 0; i != 4; ++i) {
		for(int j = 0; j != 4; ++j) {
			Point sum(0.0,0.0,0.0);
			for (int k = 0; k != 4; ++k) {
				sum = matrix2.matrix[k][j] * matrix[i][k] + sum;
			}
			result[i][j] = sum;
		}
	}
    return PointMatrix(result);
}




PointMatrix::PointMatrix(Point points[][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrix[i][j] = points[i][j];
}

PointMatrix::PointMatrix() {}

PointMatrix PointMatrix::mult(PointMatrix matrix2) {
    Point result[4][4];
    for(int i = 0; i != 4; ++i) {
		for(int j = 0; j != 4; ++j) {
			Point sum(0.0,0.0,0.0);
			for (int k = 0; k != 4; ++k) {
				sum = matrix2.matrix[k][j] * matrix[i][k] + sum;
			}
			result[i][j] = sum;
		}
	}
    return PointMatrix(result);
}

PointMatrix PointMatrix::mult(Matrix matrix2) {
    Point result[4][4];
    for(int i = 0; i != 4; ++i) {
		for(int j = 0; j != 4; ++j) {
			Point sum(0.0,0.0,0.0);
			for (int k = 0; k != 4; ++k) {
				sum = matrix[i][k] * matrix2.matrix[k][j] + sum;
			}
			result[i][j] = sum;
		}
	}
    return PointMatrix(result);
}






