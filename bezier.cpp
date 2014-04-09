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
*/
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
vector<Patch> Patch::subDivide(){
	float hz1_array[4][4] = {{  1.0,  0.0,  0.0,  0.0},
				     	    {  0.5,  0.5,  0.0,  0.0},
					        { 0.25,  0.5, 0.25,  0.0},
					        {0.125,0.375,0.375,0.125}};
				
	float hz2_array[4][4] = {{0.125,0.375,0.375,0.125},
					  		{  0.0, 0.25,  0.5, 0.25},
			   		  		{  0.0,  0.0,  0.5,  0.5},
					  		{  0.0,  0.0,  0.0,  1.0}};
	Matrix Hz1(hz1_array);
	Matrix Hz2(hz2_array);
	PointMatrix P(points);

	PointMatrix P11 = Hz1.mult(P).mult(Hz1.transpose());
	PointMatrix P12 = Hz1.mult(P).mult(Hz2.transpose());
	PointMatrix P21 = Hz2.mult(P).mult(Hz1.transpose());
	PointMatrix P22 = Hz2.mult(P).mult(Hz2.transpose());
	
	Patch patch11(P11);
	Patch patch12(P12);
	Patch patch21(P21);
	Patch patch22(P22);
	
	vector<Patch> newPatches;
	newPatches.push_back(patch11);
	newPatches.push_back(patch12);
	newPatches.push_back(patch21);
	newPatches.push_back(patch22);
	
	return newPatches;
}


Model::Model(){}
Model::Model(vector<Patch> p, Color c){
	patches = p;
	color = c;
}


void Model::draw(){
// Rectangle Code

  //glColor3f(1.0f,0.0f,0.0f); 
  Point A,B,C,D;

  for (int i = 0; i< patches.size(); i++){
      for (int u = 0; u < 3; u++){
          for (int v =0; v < 3; v++){
              A = patches[i].points[0 +u][0 +v];
      		  B = patches[i].points[0 +u][1 +v];
      		  C = patches[i].points[1 +u][1 +v];
              D = patches[i].points[1 +u][0 +v];
              glBegin(GL_QUADS);                       // draw rectangle 
              Point normal = ((A-B).cross(A-C)).normalize();
              GLfloat gl_normal[] = {normal.x, normal.y, normal.z};
              glNormal3fv(gl_normal);
              glVertex3f(A.x, A.y, A.z);               // bottom left corner of rectangle
              glVertex3f(B.x, B.y, B.z);               // top left corner of rectangle
              glVertex3f(C.x, C.y, C.z);               // top right corner of rectangle
              glVertex3f(D.x, D.y, D.z);               // bottom right corner of rectangle
              glEnd();
          }
      }
  }
  

}

void Model::uSubDivide(){
	int numPatches = patches.size();
	vector<Patch> temp;
	for (int i = 0; i < numPatches; i++){
		temp = patches[i].subDivide();
		patches[i] = temp[0];
		patches.push_back(temp[1]);
		patches.push_back(temp[2]);
		patches.push_back(temp[3]);
		
	}
}

/*
// given a control patch and (u,v) values, find
// the surface point and normal
Ray bezpatchinterp(Point patch, u, v) 
	# build control points for a Bezier curve in v
	vcurve[0] = bezcurveinterp(patch[0][0:3], u).point;
	vcurve[1] = bezcurveinterp(patch[1][0:3], u).point;
	vcurve[2] = bezcurveinterp(patch[2][0:3], u).point;
	vcurve[3] = bezcurveinterp(patch[3][0:3], u).point;
	# build control points for a Bezier curve in u
	ucurve[0] = bezcurveinterp(patch[0:3][0], v).point;
	ucurve[1] = bezcurveinterp(patch[0:3][1], v).point;
	ucurve[2] = bezcurveinterp(patch[0:3][2], v).point;
	ucurve[3] = bezcurveinterp(patch[0:3][3], v).point;

	# evaluate surface and derivative for u and v
	p, dPdv = bezcurveinterp(vcurve, v)
	p, dPdu = bezcurveinterp(ucurve, u)
	# take cross product of partials to find normal
	n = cross(dPdu, dPdv)
	n = n / length(n)
	return p, n
*/




























