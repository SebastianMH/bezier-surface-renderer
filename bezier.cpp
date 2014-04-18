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
    normal  = ((a-b).cross(a-c)).normalize();
}


Triangle::Triangle(Ray aa, Ray bb, Ray cc, Point aauv, Point bbuv, Point ccuv){
    a = aa.point;
    b = bb.point;
    c = cc.point;
    auv = aauv;
    buv = bbuv;
    cuv = ccuv;
    na = aa.vector.normalize();
    nb = bb.vector.normalize();
    nc = cc.vector.normalize();
    normal  = (na + nb + nc).normalize();
}

Triangle::Triangle(Ray aa, Ray bb, Ray cc){
    a = aa.point;
    b = bb.point;
    c = cc.point;
    na = aa.vector.normalize();
    nb = bb.vector.normalize();
    nc = cc.vector.normalize();
    normal  = (na + nb + nc).normalize();
}


void Triangle::draw(){

    	glBegin(GL_TRIANGLES);
    	
		GLfloat gl_normala[] = {na.x, na.y, na.z};
		glNormal3fv(gl_normala);
		glVertex3f(a.x, a.y, a.z);

		GLfloat gl_normalb[] = {nb.x, nb.y, nb.z};
		glNormal3fv(gl_normalb);
		glVertex3f(b.x, b.y, b.z);

		GLfloat gl_normalc[] = {nc.x, nc.y, nc.z};
		glNormal3fv(gl_normalc);
		glVertex3f(c.x, c.y, c.z);

		glEnd();
}


void Triangle::drawFlat(){

		glBegin(GL_TRIANGLES);
		GLfloat gl_normal[] = {normal.x, normal.y, normal.z};
		glNormal3fv(gl_normal);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
		glEnd();
}


void Triangle::print(){
    printf("Triangle: {");
    a.print();
    printf(",");
    b.print();
    printf(",");
    c.print();
    printf("} and uv map <");
    auv.print();
    printf(",");
    buv.print();
    printf(",");
    cuv.print();
    printf(">\n");
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
    // the uth column of points in the patch
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

	// check and correct for errors in which control points overlap
    if(uRay.vector.x == 0.0 && uRay.vector.y == 0.0 && uRay.vector.z == 0.0){
    	if( u == 1.0){ u = 0.00000009;}
    	if( v == 1.0){ v = 0.00000009;}
    	if( u == 0.0){ u = 0.00000001;}
    	if( v == 0.0){ v = 0.00000001;}
    	return interpolate(u,v);
    }
    
    uRay.vector = uRay.vector.normalize();
    return uRay;
}


void Patch::uSubDivide(float step){
    
	int iu, iv;
    float u,v;
    Ray a,b,c;
	int numDiv = (int)floor(1.0/step);
    step = 1.0/numDiv;
    
	for (iu = 0; iu < numDiv; iu++){
		for (iv = 0; iv < numDiv; iv++){
            u = iu*step;
            v = iv*step;

            a = interpolate(u,v);
            b = interpolate(u,v+step);
            c = interpolate(u+step,v);
			triangles.push_back(Triangle(a,b,c,Point(u,v,0),Point(u,v+step,0),Point(u+step,v,0)));

			a = interpolate(u+step,v+step);
			triangles.push_back(Triangle(c,b,a,Point(u+step,v,0),Point(u,v+step,0),Point(u+step,v+step,0)));

		}
	}

	for (int i = 0; i < triangles.size(); i=i+1){
		u = (triangles[i].auv.x + triangles[i].buv.x + triangles[i].cuv.x)/3.0;
		v = (triangles[i].auv.y + triangles[i].buv.y + triangles[i].cuv.y)/3.0;
		triangles[i].normal  = interpolate(u,v).vector.normalize();
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


void Patch::drawFlat(){
    for (int i = 0; i < triangles.size(); i++){
        triangles[i].drawFlat();
    }
}


void Patch::aSubDivide(float step){

    int error_code;
    float error1,error2,error3;
    Point uv1,uv2,uv3,edge1,edge2,edge3;
    Ray surface1,surface2,surface3;
    Point a,b,c,auv,buv,cuv;
    
    Triangle triangle (interpolate(0, 0),interpolate(0, 1),interpolate(1, 0),Point(0,0,0),Point(0,1,0),Point(1,0,0));
    triangles.push_back(triangle);

    triangle = Triangle(interpolate(1, 0),interpolate(0, 1),interpolate(1, 1),Point(1,0,0),Point(0,1,0),Point(1,1,0));
    triangles.push_back(triangle);
    
    for (int i = 0; i < triangles.size(); i++){
	    error_code = 0;

        a = triangles[i].a;
        b = triangles[i].b;
	    c = triangles[i].c;
	    auv = triangles[i].auv;
	    buv = triangles[i].buv;
	    cuv = triangles[i].cuv;

 	    //e1
        edge1 = (a + b)*0.5;
        uv1 = (auv + buv)*0.5;
		surface1 = interpolate(uv1.x, uv1.y);
		error1 = surface1.point.distance(edge1);

		//e2
        edge2 = (b + c)*0.5;
        uv2 = (buv + cuv)*0.5;
		surface2 = interpolate(uv2.x, uv2.y);
		error2 = surface2.point.distance(edge2);

		//e3
        edge3 = (c + a)*0.5;
        uv3 = (cuv + auv)*0.5;
		surface3 = interpolate(uv3.x, uv3.y);
		error3 = surface3.point.distance(edge3);

		if (error1 > step){
            error_code += 1;
        }
        if (error2 > step){
            error_code += 10;
        }
        if (error3 > step){
            error_code += 100;
        }
        
        switch (error_code) {
            case 0:
                break;
            case 1:
                triangle = triangles[i];
                triangle.b = surface1.point;
                triangle.nb = surface1.vector;
                triangle.buv = uv1;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1.point;
                triangle.na = surface1.vector;
                triangle.auv = uv1;
                triangles[i] = triangle;
                i--;
                break;
            case 10:
                triangle = triangles[i];
                triangle.c = surface2.point;
                triangle.nc = surface2.vector;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.b = surface2.point;
                triangle.nb = surface2.vector;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            case 100:
                triangle = triangles[i];
                triangle.c = surface3.point;
                triangle.nc = surface3.vector;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3.point;
                triangle.na = surface3.vector;
                triangle.auv = uv3;
                triangles[i] = triangle;
                i--;
                break;
            case 11:
                triangle = triangles[i];
                triangle.b = surface1.point;
                triangle.nb = surface1.vector;
                triangle.buv = uv1;
                triangle.c = surface2.point;
                triangle.nc = surface2.vector;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1.point;
                triangle.na = surface1.vector;
                triangle.auv = uv1;
                triangle.c = surface2.point;
                triangle.nc = surface2.vector;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.b = surface2.point;
                triangle.nb = surface2.vector;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            case 110:
                triangle = triangles[i];
                triangle.c = surface3.point;
                triangle.nc = surface3.vector;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3.point;
                triangle.na = surface3.vector;
                triangle.auv = uv3;
                triangle.c = surface2.point;
                triangle.nc = surface2.vector;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3.point;
                triangle.na = surface3.vector;
                triangle.auv = uv3;
                triangle.b = surface2.point;
                triangle.nb = surface2.vector;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            case 101:
                triangle = triangles[i];
                triangle.b = surface1.point;
                triangle.nb = surface1.vector;
                triangle.buv = uv1;
                triangle.c = surface3.point;
                triangle.nc = surface3.vector;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3.point;
                triangle.na = surface3.vector;
                triangle.auv = uv3;
                triangle.b = surface1.point;
                triangle.nb = surface1.vector;
                triangle.buv = uv1;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1.point;
                triangle.na = surface1.vector;
                triangle.auv = uv1;
                triangles[i] = triangle;
                i--;
                break;
            case 111:
                triangle = triangles[i];
                triangle.b = surface1.point;
                triangle.nb = surface1.vector;
                triangle.buv = uv1;
                triangle.c = surface3.point;
                triangle.nc = surface3.vector;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1.point;
                triangle.na = surface1.vector;
                triangle.auv = uv1;
                triangle.b = surface2.point;
                triangle.nb = surface2.vector;
                triangle.buv = uv2;
                triangle.c = surface3.point;
                triangle.nc = surface3.vector;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1.point;
                triangle.na = surface1.vector;
                triangle.auv = uv1;
                triangle.c = surface2.point;
                triangle.nc = surface2.vector;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3.point;
                triangle.na = surface3.vector;
                triangle.auv = uv3;
                triangle.b = surface2.point;
                triangle.nb = surface2.vector;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            default:
                exit(-1);
		}

	}

	for (int i = 0; i < triangles.size(); i++){
		triangles[i].normal  = (triangles[i].na + triangles[i].nb + triangles[i].nc).normalize();
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


void Model::drawFlat(){
	for (int i = 0; i < patches.size(); i++){
        patches[i].drawFlat();
    }
}


void Model::uSubDivide(float step){
    for (int i = 0; i < patches.size(); i++){
        patches[i].uSubDivide(step);
    }
}


void Model::aSubDivide(float step){
    for (int i = 0; i < patches.size(); i++){
        patches[i].aSubDivide(step);
    }
}
