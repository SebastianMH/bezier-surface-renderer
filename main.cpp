// CS184 Simple OpenGL Example
#include <cstdlib>  //for rand

#include <iostream>  //to load images
using namespace std;


//#include "CImg.h"  //to get rgba data
//using namespace cimg_library;


#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>







#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265
#define Nsquares 512
using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
  public:
    int w, h; // width and height
};



float rand_float(float a, float b)
{
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}

/*
CImg<unsigned char> src("image.jpg");
int src_w = src.width();
int src_h = src.height();

float getR(float x, float y){
	return (float)((int)src(src_w + (int)(x*src_w/2),src_w + (int)(y*src_h/2),0,0)/255);
}
float getG(float x, float y){
	return (float)((int)src(src_w + (int)(x*src_w/2),src_w + (int)(y*src_h/2),0,1)/255);
}
float getB(float x, float y){
	return (float)((int)src(src_w + (int)(x*src_w/2),src_w + (int)(y*src_h/2),0,2)/255);
}

*/









class SquareLens {
  public:
    float s, temp_theta, temp_mag;
    float x, y, vx, vy;
    float r, g, b;
    
    SquareLens();
    void draw();
    void move(SquareLens squarelens[]);
};

SquareLens::SquareLens(){
    s = rand_float(0.01f,0.04f); //side length/2
    temp_theta = rand_float(0,(float)2*PI);
	temp_mag = rand_float(0,(float)2*PI);
	x = temp_mag*cos(temp_theta);
    y = temp_mag*sin(temp_theta);
    vx = rand_float(-0.004f,0.004f);
    vy = rand_float(-0.004f,0.004f);
	r = 0.5f;
	g = 0.5f;
	b = 0.5f;
}

void SquareLens::draw () {
    glColor3f(r,g,b);                   // setting the color to pure red 90% for the rect
    glBegin(GL_POLYGON);                         // draw rectangle 
    //glVertex3f(x val, y val, z val (won't change the point because of the projection type));
    glVertex3f( x - s , y - s , 0.0f);               // bottom left corner of rectangle
    glVertex3f( x - s , y + s , 0.0f);               // top left corner of rectangle
    glVertex3f( x + s , y + s , 0.0f);               // top right corner of rectangle
    glVertex3f( x + s , y - s , 0.0f);               // bottom right corner of rectangle
    glEnd();
}

void SquareLens::move (SquareLens squarelens[]) {
    float distance;
	for (int i = 0; i <= Nsquares; i++){
		distance = sqrt(pow(squarelens[i].x - x,2) + pow(squarelens[i].y - y,2));
		if (distance != 0.0f) {
			vx = vx + 0.0001f * pow(squarelens[i].x - x,3);
			vy = vy + 0.0001f * pow(squarelens[i].y - y,3);
		}
	}


	vx = min(vx, 0.05f);
	vy = min(vy, 0.05f);
	vx = max(vx, -0.05f);
	vy = max(vy, -0.05f);


	x = x + vx;
    y = y + vy;
    if (x < -1.0f + s || x > 1.0f - s ) {vx = -vx;}
    if (y < -1.0f + s || y > 1.0f - s ) {vy = -vy;}
	r = min(max( r +rand_float(-0.04f,0.04f),0.0f),1.0f);
	g = min(max( g +rand_float(-0.04f,0.04f),0.0f),1.0f);
	b = min(max( b +rand_float(-0.04f,0.04f),0.0f),1.0f);

	
}



//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;
SquareLens squarelens[Nsquares];





//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                // loading the identity matrix for the screen

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  // glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

  glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch

  //------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  myReshape(viewport.w,viewport.h);
  
  
  
  
  
  
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {
    
    for (int i = 0; i <= Nsquares; i++){
        squarelens[i].move(squarelens);
    }
    
    
    
    


  //----------------------- ----------------------- -----------------------
  // This is a quick hack to add a little bit of animation.
  static float tip = 0.5f;
  const  float stp = 0.01f;
  const  float beg = 0.1f;
  const  float end = 0.9f;

  tip += stp;
  if (tip>end) tip = beg;
  //----------------------- ----------------------- -----------------------


  glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)

  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"
	/*
  //----------------------- code to draw objects --------------------------
  // Rectangle Code
  //glColor3f(red component, green component, blue component);
  glColor3f(1.0f,0.0f,0.0f);                   // setting the color to pure red 90% for the rect

  glBegin(GL_POLYGON);                         // draw rectangle 
  //glVertex3f(x val, y val, z val (won't change the point because of the projection type));
  glVertex3f(-0.8f, 0.0f, 0.0f);               // bottom left corner of rectangle
  glVertex3f(-0.8f, 0.5f, 0.0f);               // top left corner of rectangle
  glVertex3f( 0.0f, 0.5f, 0.0f);               // top right corner of rectangle
  glVertex3f( 0.0f, 0.0f, 0.0f);               // bottom right corner of rectangle
  glEnd();
  // Triangle Code
  glColor3f(1.0f,0.5f,0.0f);                   // setting the color to orange for the triangle

  float basey = -sqrt(0.48f);                  // height of triangle = sqrt(.8^2-.4^2)
  glBegin(GL_POLYGON);
  glVertex3f(tip,  0.0f, 0.0f);                // top tip of triangle
  glVertex3f(0.1f, basey, 0.0f);               // lower left corner of triangle
  glVertex3f(0.9f, basey, 0.0f);               // lower right corner of triangle
  glEnd();
  
  */
  
  for (int i = 0; i <= Nsquares; i++){
      squarelens[i].draw();
  }
  
  
  
  
  
  //-----------------------------------------------------------------------

  glFlush();
  glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
}


//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    
    
    cout << argv[1];
    cout << argv[2];
    /*
        int i = 1;
    float r, g, b;
    float x, y, z;
    while(i < argc) {
        if(strcmp(argv[i],"-ka") == 0) {
            r = atof(argv[i+1]);
            g = atof(argv[i+2]);
            b = atof(argv[i+3]);
            ambient = RGB(r, g, b);
            ambient_valid = true;
            i += 4;
        } else if (strcmp(argv[i],"-kd") == 0) {
            r = atof(argv[i+1]);
            g = atof(argv[i+2]);
            b = atof(argv[i+3]);
            diffuse = RGB(r, g, b);
            diffuse_valid = true;
            i += 4;
        } else if (strcmp(argv[i],"-ks") == 0) {
            r = atof(argv[i+1]);
            g = atof(argv[i+2]);
            b = atof(argv[i+3]);
            specular = RGB(r, g, b);
            specular_valid = true;
            i += 4;
        } else if (strcmp(argv[i],"-sp") == 0) {
            power_coeff = atof(argv[i+1]);
            i += 2;
        } else if (strcmp(argv[i],"-pl") == 0) {
            x = atof(argv[i+1]);
            y = atof(argv[i+2]);
            z = atof(argv[i+3]);
            r = atof(argv[i+4]);
            g = atof(argv[i+5]);
            b = atof(argv[i+6]);
            point_lights[num_point_lights] = Light(x, y, z, r, g, b);
            num_point_lights++;
            i += 7;
        } else if (strcmp(argv[i],"-dl") == 0) {
            x = atof(argv[i+1]);
            y = atof(argv[i+2]);
            z = atof(argv[i+3]);
            r = atof(argv[i+4]);
            g = atof(argv[i+5]);
            b = atof(argv[i+6]);
            direct_lights[num_direct_lights] = Light(x, y, z, r, g, b);
            num_direct_lights++;
            i += 7;
        } else if (strcmp(argv[i],"-toon") == 0) {
                toon = true;
                i += 1;
        } else {
            printf("Invalid command line flags");
            exit(0);
        }
    }
    */
    
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 1000;//1280
  viewport.h = 1000;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS184! Sebastian Miller-Hack");

  initScene();                                 // quick function to set up scene
  SquareLens squarelens;
  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








