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
    uRay.vector.normalize();
    return uRay;

/*	Point ucurve[4];
	Point vcurve[4];
	Point temp[4];
	Point p;
	Ray uRay, vRay;
	// build control points for a Bezier curve in v
	vcurve[0] = bezcurveinterp(points[0], u).point;
	vcurve[1] = bezcurveinterp(points[1], u).point;
	vcurve[2] = bezcurveinterp(points[2], u).point;
	vcurve[3] = bezcurveinterp(points[3], u).point;
	
    printf("\n\n <<<<< \n");
    vcurve[0].print();
    vcurve[1].print();
    vcurve[2].print();
    vcurve[3].print();
    printf("\n\n >>>>> \n");
	
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
	return uRay;*/
}


void Patch::uSubDivide(float step){
    
	int iu, iv;
    float u,v;
    Point a,b,c;
	int numDiv = (int)floor(1.0/step); //note: check what prof wants regarding subdivision into two parts
    step = 1.0/numDiv;
    
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


void Patch::aSubDivide(float step){
    
    /////////////////// TEST CASES ////////////////////////
    /*
    Point curve[] = {Point(0,0,0), Point(1,-1,0), Point(3,1,0), Point(4,0,0)};
    Point patch[4][4];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            Point i_point(i,i,i);
            patch[i][j] = curve[j]+i_point;
        }
    }
    
    bezcurveinterp(curve, 0).point.print();
    Patch p(patch);
    p.interpolate(0, (0.666666666666+0.3333333333333)/2).point.print();
    
    exit(0);
    */
    
    int error_code;
    float error1,error2,error3;
    Point surface1,surface2,surface3,uv1,uv2,uv3,edge1,edge2,edge3;
    Point a,b,c,auv,buv,cuv;
    
    Triangle triangle (interpolate(0, 0).point,interpolate(0, 1).point,interpolate(1, 0).point); // old points[0][0],points[0][3],points[3][0]
    triangle.auv = Point(0,0,0);
    triangle.buv = Point(0,1,0);
    triangle.cuv = Point(1,0,0);
    triangles.push_back(triangle);

    
    triangle = Triangle(interpolate(1, 0).point,interpolate(0, 1).point,interpolate(1, 1).point); // old: points[3][0],points[0][3],points[3][3]
    triangle.auv = Point(1,0,0);
    triangle.buv = Point(0,1,0);
    triangle.cuv = Point(1,1,0);
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
		surface1 = interpolate(uv1.x, uv1.y).point;
		error1 = surface1.distance(edge1);

		//e2
        edge2 = (b + c)*0.5;
        uv2 = (buv + cuv)*0.5;
		surface2 = interpolate(uv2.x, uv2.y).point;
		error2 = surface2.distance(edge2);
		
		//e3
        edge3 = (c + a)*0.5;
        uv3 = (cuv + auv)*0.5;
		surface3 = interpolate(uv3.x, uv3.y).point;
		error3 = surface3.distance(edge3);
		
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
                triangles[i].print();
                triangle = triangles[i];
                triangle.b = surface1;
                triangle.buv = uv1;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1;
                triangle.auv = uv1;
                triangles[i] = triangle;
                printf("%d\n", i);
                triangles[i].print();
                printf("\n\n");
                i--;
                break;
            case 10:
                triangle = triangles[i];
                triangle.c = surface2;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.b = surface2;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            case 100:
                triangle = triangles[i];
                triangle.c = surface3;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3;
                triangle.auv = uv3;
                triangles[i] = triangle;
                i--;
                break;
            case 11:
                triangle = triangles[i];
                triangle.b = surface1;
                triangle.c = surface2;
                triangle.buv = uv1;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1;
                triangle.c = surface2;
                triangle.auv = uv1;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.b = surface2;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            case 110:
                triangle = triangles[i];
                triangle.c = surface3;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3;
                triangle.c = surface2;
                triangle.auv = uv3;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3;
                triangle.b = surface2;
                triangle.auv = uv3;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            case 101:
                triangle = triangles[i];
                triangle.b = surface1;
                triangle.c = surface3;
                triangle.buv = uv1;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3;
                triangle.b = surface1;
                triangle.auv = uv3;
                triangle.buv = uv1;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1;
                triangle.auv = uv1;
                triangles[i] = triangle;
                i--;
                break;
            case 111:
                triangle = triangles[i];
                triangle.b = surface1;
                triangle.c = surface3;
                triangle.buv = uv1;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1;
                triangle.b = surface2;
                triangle.c = surface3;
                triangle.auv = uv1;
                triangle.buv = uv2;
                triangle.cuv = uv3;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface1;
                triangle.c = surface2;
                triangle.auv = uv1;
                triangle.cuv = uv2;
                triangles.push_back(triangle);
                triangle = triangles[i];
                triangle.a = surface3;
                triangle.b = surface2;
                triangle.auv = uv3;
                triangle.buv = uv2;
                triangles[i] = triangle;
                i--;
                break;
            default:
                exit(-1);
		}
		
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


void Model::aSubDivide(float step){
    for (int i = 0; i < patches.size(); i++){
        patches[i].aSubDivide(step);
    }
}