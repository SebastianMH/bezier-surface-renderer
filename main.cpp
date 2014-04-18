// CS184 Simple OpenGL Example
#include <cstdlib>  //for rand


//#include "CImg.h"  //to get rgba data
//using namespace cimg_library;

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <iterator>

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

#include "bezier.h"
#include "utility.h"

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif
using namespace std;
#define PI 3.14159265
//load structure of models->patchs->curves->vertex

//****************************************************
// Global Variables
//****************************************************

//Change in location when keys are pressed
float degree_step = 1;
float translation_step = 0.1;

string input_file_name;
float sub_div_parameter;
// switch from uniform to adaptive mode 
bool adaptive = false;
bool flat_shading = false;
bool wireframe = false;
bool hiddenLineMode = false;
Model model;





float zoom = 15.0f;
float rotx = 0;
float roty = 0.001f;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};






//****************************************************
// Some Classes
//****************************************************

class Viewport {
  public:
    int w, h; // width and height
};
Viewport viewport;


//****************************************************
// Some Functions
//****************************************************
/******* lighting **********/
void light (void) {
    
    GLfloat white[] = {1.0, 1.0, 1.0}; 
    GLfloat low_red[] = {0.3, 0.0, 0.0};
    GLfloat red[] = {1.0, 0.0, 0.0};
    GLfloat black[] = {0.0, 0.0, 0.0};
    GLfloat light_position[] = {-7.0, 7.0, 7.0, 0.0};
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
/******* lighting **********/

/*
Extracts commandline arguments, or returns an error if arguments are not valid.
Commandline arguments should be formatted like this:
main [inputfile.bez] [float subdivision parameter] optional[-a]

example:
./main models/teapot.bez 0.5
*/
void parseCommandlineArguments(int argc, char *argv[]) {
  if(argc < 3) {
    printf("\nWrong number of command-line arguments. Arguments should be in the format:\n");
    printf("main [inputfile.bez] [float subdivision parameter] optional[-a]\n\n");
    exit(0);
  }
  input_file_name = argv[1];
  sub_div_parameter = atof(argv[2]);
  
  if(argc == 4 && strcmp(argv[3],"-a") ==0) {
    adaptive = true;
  }
}


/*
Splits a string at white space and returns extracted words as strings in a vector 
*/
vector<string> splitAtWhiteSpace(string const &input) { 
    istringstream buffer(input);
    vector<string> ret((istream_iterator<string>(buffer)), istream_iterator<string>());
    return ret;
}


/*
Parses input file and returns a model
*/
Model parseBezFile() {
  ifstream input_file(input_file_name.c_str());
  string line;
  if(input_file.is_open()) {
    int numPatches = 0;
    getline(input_file, line);
    numPatches = atoi(line.c_str());
    vector<Patch> patches;
    for (int i = 0; i < numPatches; i++){
        Point point_array[4][4];
        for(int c = 0; c < 4; c++) {
            getline(input_file, line);
            vector<string> coor_list;
            coor_list = splitAtWhiteSpace(line);
            
            for(int p = 0; p < 4; p++) {
                float x = atof(coor_list[3*p].c_str());
                float y = atof(coor_list[3*p+1].c_str());
                float z = atof(coor_list[3*p+2].c_str());
                Point point(x, y, z);
                point_array[c][p] = point;
            }
        }
        Patch patch(point_array);
        patches.push_back(patch);
        
        getline(input_file, line); //blank line between patches
    }
    input_file.close();
    Model model(patches, Color(0, 1, 0));
    return model;
  }
  printf("input file was not found\n");
  exit(1);
}


void parseObjFile() {
  ifstream input_file(input_file_name.c_str());
  string line;
  if(input_file.is_open()) {
      vector<Point> points;
      while (getline(input_file, line)) {
          vector<string> llist;
          llist = splitAtWhiteSpace(line);
          if (llist[0].compare("v") == 0) {
              Point p(atof(llist[1].c_str()), atof(llist[2].c_str()), atof(llist[3].c_str()));
              points.push_back(p);
          }
          getline(input_file, line); //blank line between patches
      }
  } else {
      printf("input file was not found\n");
      exit(1);
  }
}

//****************************************************
// Keyboard functions
//****************************************************


void special_keyboard(int key, int x, int y){
  glMatrixMode(GL_MODELVIEW);
  
  switch(key){
      
      case GLUT_KEY_RIGHT:
        if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
          // object will be translated right
          tx += translation_step;
          break;
        }
        // object will be rotated right
        roty += degree_step;
        break;

      case GLUT_KEY_LEFT:
        if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
          // object will be translated left
          tx -= translation_step;
          break;
        }
        // object will be rotated left
        roty -= degree_step;
        break;

      case GLUT_KEY_UP:
        if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
          // object will be translated up
          ty += translation_step;
          break;
        }
        // object will be rotated up
        rotx -= degree_step;
        break;

      case GLUT_KEY_DOWN:
        if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
          // object will be translated down
          ty -= translation_step;
          break;
        }
        // object will be rotated down
        rotx += degree_step;
        break;

  }

}

void keyboard(unsigned char key, int x, int y){
  glMatrixMode(GL_MODELVIEW);
  switch(key){
      case 's':
        // toggle between flat and smooth shading
        flat_shading = !flat_shading;
        if (flat_shading){
        	glShadeModel(GL_FLAT);
        }else{
        	glShadeModel(GL_SMOOTH);
        }
        break;
    
      case 'w':
        // toggle between filled and wireframe mode
        wireframe = !wireframe;
        break;
        
	case 'h':
        // toggle between filled and wireframe mode
        hiddenLineMode = !hiddenLineMode;
        break;
    
      case 'c':
        // do vertex color shading based on the Gaussian Curvature of the surface.
        break;
    
      case 61: // = sign (PLUS)
        // zoom in
        zoom -= translation_step;
        break;
    
      case 45: // MINUS sign -
        // zoom out
        zoom += translation_step;
        break;
  }
}


//****************************************************
// Reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;
  
  glMatrixMode(GL_PROJECTION);  
  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
  glLoadIdentity();                // loading the identity matrix for the screen

  
	gluPerspective(45,(float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
// prevent divide by 0 error when minimised
if(viewport.w==0) 
	viewport.h = 1;

glViewport(0,0,viewport.w,viewport.h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45,(float)viewport.w/viewport.h,0.1,100);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
  
  /******* lighting **********/
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  /******* lighting **********/
  
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);


  ///-----------------------------------------------------------------------
  light();
  //GLfloat purple[] = {1.0, 0, 1.0}; 
  //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, purple);
  
  if(wireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  	  glDisable(GL_LIGHTING);
      model.draw();
  }
  else if(hiddenLineMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        model.draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.5, 0.5);
        glColor3f(0.0,0.0,0.0);
        model.draw();
        glDisable(GL_POLYGON_OFFSET_FILL);
        glColor3f(1.0,1.0,1.0);
    } else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    	glEnable(GL_LIGHTING);
        model.draw();
    }  

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

void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[0] && Buttons[1] )
	{
		zoom -= (float) 0.05f * diffx;
	}
	else
		if( Buttons[0] )
		{
			rotx += (float) 0.5f * diffy;
			roty += (float) 0.5f * diffx;		
		}
		else
			if( Buttons[1] )
			{
				tx += (float) 0.05f * diffx;
				ty -= (float) 0.05f * diffy;
			}
			glutPostRedisplay();
}

//-------------------------------------------------------------------------------
//
void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	glutPostRedisplay();
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
      
  parseCommandlineArguments(argc, argv);
  
  string extension = input_file_name.substr(input_file_name.size() - 3);
  if(extension.compare("bez") == 0) {
      model = parseBezFile();
  } else {
      parseObjFile();
      exit(0);
  }
  if (adaptive){
  	model.aSubDivide(sub_div_parameter);
  }else{
  	model.uSubDivide(sub_div_parameter);
  }
  
  
  
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB|GLUT_DEPTH);

  // Initalize theviewport size
  viewport.w = 1000;//1280
  viewport.h = 1000;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS184! by Sebastian and Risa");

  initScene();                                 // quick function to set up scene
  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  
  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);


  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








