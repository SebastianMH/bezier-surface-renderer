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





Matrix::Matrix(float imatrix[][4]) {
    size = 4;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = imatrix[i][j];
}

Matrix::Matrix(float imatrix[][3]) {
    size = 3;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = imatrix[i][j];
}

Matrix::Matrix(float imatrix[][2]) {
    size = 2;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = imatrix[i][j];
}

Matrix::Matrix() {
    size = 0;
    float matrix[0][0];
}

// multiply by another sizeXsize matrix 
Matrix Matrix::multiply(Matrix matrix2) {
    float result[MAX][MAX];
    for(int i = 0; i != size; ++i) {
		for(int j = 0; j != size; ++j) {
			float sum = 0;
			for (int k = 0; k != size; ++k) {
				sum += matrix[i][k] * matrix2.matrix[k][j];
			}
			result[i][j] = sum;
		}
	}
    return Matrix(result);
}

void Matrix::print() {
    for (int i = 0; i < size; i++) {
        cout << "[ ";
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "]\n";
    }
}

float Matrix::determinant() {
    for(int x = 0; x < size; x++){
        for(int j = 0; j < size; j++){
            if(j > x){
                float divider = matrix[j][x]/matrix[x][x];
                for(int k = 0; k < size; k++){
                    matrix[j][k] = matrix[j][k] - (divider * matrix[x][k]);
                }
            }
        }
    }
    
    float det = 1;
    for(int i = 0; i < size; i++)
        det = det * matrix[i][i];
    return det;
}

Matrix Matrix::inverse_of_3Dtransformation(){
    float m[3][3] = {{matrix[0][0], matrix[0][1], matrix[0][2]},
                     {matrix[1][0], matrix[1][1], matrix[1][2]},
                     {matrix[2][0], matrix[2][1], matrix[2][2]}};
    Matrix mat3(m);
    
    float det = mat3.determinant();
    
    float a = m[1][1]*m[2][2] - m[1][2]*m[2][1];
    float b = m[0][2]*m[2][1] - m[0][1]*m[2][2];
    float c = m[0][1]*m[1][2] - m[0][2]*m[1][1];
    
    float d = m[1][2]*m[2][0] - m[1][0]*m[2][2];
    float e = m[0][0]*m[2][2] - m[0][2]*m[2][0];
    float f = m[0][2]*m[1][0] - m[0][0]*m[1][2];
    
    float g = m[1][0]*m[2][1] - m[1][1]*m[2][0];
    float h = m[0][1]*m[2][0] - m[0][0]*m[2][1];
    float i = m[0][0]*m[1][1] - m[0][1]*m[1][0];
    
    float r[4][4] = {{a/det, b/det, c/det, 0},
                     {d/det, e/det, f/det, 0},
                     {g/det, h/det, i/det, 0},
                     {0,     0,     0,     1}};
    Matrix result(r);
    return result;
}


Matrix Matrix::transpose(){
    float matrixT[MAX][MAX];
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            matrixT[j][i] = matrix[i][j];
        }
    }
    Matrix mT(matrixT);
    return mT;
}














