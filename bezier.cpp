#include "bezier.h"


// given the control points of a bezier curve
// and a parametric value, return the curve
// point and derivative
Ray bezcurveinterp(Point curve[], float u){
	Point A, B, C, D, E, p,temp;
	Vector dPdu;
	// first, split each of the three segments
	// to form two new ones AB and BC
	A = curve[0] * (1.0-u);
		A = curve[1] * u + A;
	B = curve[1] * (1.0-u);
		B = curve[2] * u + B;
	C = curve[2] * (1.0-u);
		C = curve[3] * u + C;
	// now, split AB and BC to form a new segment DE
	D = A * (1.0-u);
		D = B * u + D;
	E = B * (1.0-u);
		E = C * u + E;
	// finally, pick the right point on DE,
	// this is the point on the curve
	p = D * (1.0-u);
		p = E * u + p;
	// compute derivative also
	dPdu = (E - D) * 3.0f;
	return Ray(p, dPdu);
}


Triangle::Triangle(){}


Triangle::Triangle(Point aa, Point bb, Point cc){
    a = aa;
    b = bb;
    c = cc;
}


void Triangle::draw(){
    glBegin(GL_TRIANGLES);
	Point normal = ((a-b).cross(a-c)).normalize();
	GLfloat gl_normal[] = {normal.x, normal.y, normal.z};
	glNormal3fv(gl_normal);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glEnd();
}


Point Triangle::midpoint(){
    return (a + b + c)*(1.0/3.0);
}


Patch::Patch(Point p[4][4]){
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){
			points[i][j] = p[i][j];
		}
	}
}


Patch::Patch(PointMatrix p){
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){
			points[i][j] = p.matrix[i][j];
		}
	}
}


Ray Patch::interpolate(float u, float v){
	Point ucurve[4];
	Point vcurve[4];
	Point temp[4];
	Point p;
	Ray uRay, vRay;
	// build control points for a Bezier curve in v
	vcurve[0] = bezcurveinterp(points[0], u).point;
	vcurve[1] = bezcurveinterp(points[1], u).point;
	vcurve[2] = bezcurveinterp(points[2], u).point;
	vcurve[3] = bezcurveinterp(points[3], u).point;
	// build control points for a Bezier curve in u
	temp[0] = points[0][0];
	temp[1] = points[1][0];
	temp[2] = points[2][0];
	temp[3] = points[3][0];
	ucurve[0] = bezcurveinterp(temp, v).point;
	temp[0] = points[0][1];
	temp[1] = points[1][1];
	temp[2] = points[2][1];
	temp[3] = points[3][1];
	ucurve[1] = bezcurveinterp(temp, v).point;
	temp[0] = points[0][2];
	temp[1] = points[1][2];
	temp[2] = points[2][2];
	temp[3] = points[3][2];
	ucurve[2] = bezcurveinterp(temp, v).point;
	temp[0] = points[0][3];
	temp[1] = points[1][3];
	temp[2] = points[2][3];
	temp[3] = points[3][3];
	ucurve[3] = bezcurveinterp(temp, v).point;

	// evaluate surface and derivative for u and v
	uRay = bezcurveinterp(ucurve, u);
	vRay = bezcurveinterp(vcurve, v);
	
	// take cross product of partials to find normal
	uRay.vector = uRay.vector.cross(vRay.vector);
	uRay.vector.normalize();
	return uRay;
}


void Patch::uSubDivide(float step){
    
	int iu, iv;
    float u,v;
    Point a,b,c;
	int numDiv = (int)floor(1.0/step); //note: check what prof wants regarding subdivision into two parts
    step = 1.0/numDiv;
    printf("%f",step);
	for (iu = 0; iu < numDiv; iu++){
		for (iv = 0; iv < numDiv; iv++){
            u = iu*step;
            v = iv*step;

            a = interpolate(u,v).point;
            b = interpolate(u,v+step).point;
            c = interpolate(u+step,v).point;
			triangles.push_back(Triangle(a,b,c));
			
			a = interpolate(u+step,v+step).point;
			triangles.push_back(Triangle(c,b,a));
						
		}
	}
}


Point Patch::midpoint(){
	return (points[0][0]+points[0][3]+points[3][0]+points[3][3])*0.25;
}


void Patch::draw(){
    for (int i = 0; i < triangles.size(); i++){
        triangles[i].draw();
    }
}


Model::Model(){}


Model::Model(vector<Patch> p, Color c){
	patches = p;
	color = c;
}


void Model::draw(){
	for (int i = 0; i < patches.size(); i++){
        patches[i].draw();
    }
}


void Model::uSubDivide(float step){
    for (int i = 0; i < patches.size(); i++){
        patches[i].uSubDivide(step);
    }
}


void Model::aSubDivide(float tolerance){
    /*
	int i;
	float error;
	Point surface, polygon;
	vector<Patch> temp;
	
	for (i = 0; i < patches.size(); i++){
		surface = patches[i].interpolate(0.5, 0.5).point;
		polygon = patches[i].midpoint();
		error = surface.distance(polygon);
		if(error > tolerance){
			temp = patches[i].subDivide();
			patches[i] = temp[0];
			patches.push_back(temp[1]);
			patches.push_back(temp[2]);
			patches.push_back(temp[3]);
			i--;
		}
	}*/
}