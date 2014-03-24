// CS184 Simple OpenGL Example
#include <cstdlib>  //for rand


//#include "CImg.h"  //to get rgba data
//using namespace cimg_library;

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <sstream>

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

string input_file_name;
float sub_div_parameter;
// switch from uniform to adaptive mode 
bool adaptive = false;


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

void parseCommandlineArguments(int argc, char *argv[]) {
  if(argc < 3) {
    printf("\nWrong number of command-line arguments. Arguments should be in the format:\n");
    printf("main [inputfile.bez] [float subdivision parameter] optional[-a]\n\n");
    exit(0);
  }
  input_file_name = argv[1];
  sub_div_parameter = atof(argv[2]);
  
  if(argc == 4 && strcmp(argv[3],"-a")) {
    adaptive = true;
  }
}


vector<string> splitAtWhiteSpace(string const &input) { 
    istringstream buffer(input);
    vector<string> ret((istream_iterator<string>(buffer)), istream_iterator<string>());
    return ret;
}

void parseInputFile() {
  ifstream input_file("models/" + input_file_name);
  string line;
  if(input_file.is_open()) {
    int numPatches = 0;
    getline(input_file, line);
    numPatches = atoi(line.c_str());
    
    for (int i = 0; i < numPatches; i++){
        float point_array[4][4];
        for(int c = 0; c < 4; c++) {
            getline(input_file, line);
            vector<string> coor_list;
            coor_list = splitAtWhiteSpace(line);
            for(int p = 0; p < 4; p++) {
                float x = atof(coor_list[p + 3*c].c_str());
                float y = atof(coor_list[p + 3*c].c_str());
                float z = atof(coor_list[p + 3*c].c_str());
                Point point(x, y, z);
                cout << x << " " << y << " " << z << "\n";
            }
            //float curve[4] = {atof(coor_list[0].c_str()), atof(coor_list[1]), atof(coor_list[2]), atof(coor_list[3])};
            //point_array[c] = curve; 
        }
        //blank line
        getline(input_file, line);
    }
    input_file.close();
  }
  
}


//****************************************************
// keyboard functions
//****************************************************


void special_keyboard(int key, int x, int y){
  
  switch(key){
  case GLUT_KEY_RIGHT:
    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
      printf("object will be translated right\n");
      break;
    }
    printf("object will be rotated right\n");
    break;
  case GLUT_KEY_LEFT:
    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
      printf("object will be translated left\n");
      break;
    }
    printf("object will be rotated left\n");
    break;
  case GLUT_KEY_UP:
    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
      printf("object will be translated up\n");
      break;
    }
    printf("object will be rotated up\n");
    break;
  case GLUT_KEY_DOWN:
    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
      printf("object will be translated down\n");
      break;
    }
    printf("object will be rotated down\n");
    break;
  }

}

void keyboard(unsigned char key, int x, int y){
  switch(key){
  case 's':
    printf("toggle between flat and smooth shading\n");
    break;
  case 'w':
    printf("toggle between filled and wireframe mode.\n");
    break;
  case 'c':
    printf("do vertex color shading based on the Gaussian Curvature of the surface.\n");
    break;
  case 43:
    // PLUS sign +
    printf("zoom in\n");
    break;
  case 45:
    // MINUS sign -
    printf("zoom out\n");
    break;
  }
}

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
    
  parseCommandlineArguments(argc, argv);
  
  parseInputFile();
  
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
  glutCreateWindow("CS184! by Sebastian and Risa");

  initScene();                                 // quick function to set up scene
  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
  
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








