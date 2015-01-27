Bézier Surface Renderer
=======================
A [bézier surface](http://en.wikipedia.org/wiki/B%C3%A9zier_surface) is a mathmatical representation of a surface. 
This program [tessellates](http://en.wikipedia.org/wiki/Tessellation_(computer_graphics)) polygons to approximate bézier surfaces and renders the result.
Both uniform  and adaptive tessellation are implemented.
This code was written by Sebastian Miller-Hack and Risa Pesavento for a class project at UC Berkeley.
The project specification can be found on the class website [here] (http://www-inst.eecs.berkeley.edu/~cs184/sp14/assignments/ass3/assignment-03.pdf). To see some examples of this program running, you can watch this [demonstration video] (https://www.youtube.com/watch?v=xeRLmaDDV7w).

Features
========
* Reads in a list of patch data from a file.  
* Tessellates/triangulates the patch using either a uniform or adaptive technique.  
* Opens a window and uses OpenGL to Render the object.  


Controls 
========  
* When "s" is pressed the program will toggle between flat and smooth shading.  
* When "w" is pressed the program will toggle between filled and wireframe mode.  
* When the arrow keys are pressed the object will be rotated.  
* When the shift+arrow keys are pressed the object will be translated.  
* When launched the initial zoom will show the entire object. Pressing the +/- keys will zoom in/out.  
* (EXTRA CREDIT) When "h" is pressed the program will toggle between filled and hidden-line mode.  


Compiling  
=========  
```
$ make
```

Command Line Options  
====================  
Required: 
* input file name (bez or obj)  
* the subdivision parameter  

Optional:  
* "-a": Sets subdivision to adaptive (default is uniform)   
* (EXTRA CREDIT) "-o output.obj": The program will create an OBJ file of the subdivided bezier curves instead of displaying them.  

Examples:  
```
$ ./main models/teapot.bez 0.1  
$ ./main models/teapot.bez 0.01 -a  
$ ./main models/teddy.obj 0.01  
$ ./main models/teapot.bez 0.01 -a -o teapot.obj  
```

Additional Work (see website)
=============================
* Created a custom input file (my_art.bez)  
* (EXTRA CREDIT) Created a video demonstrating features  


Resources
=========
Used to debug camera code:  
http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html#4  
  
Used to generate teddy bear:  
http://groups.csail.mit.edu/graphics/classes/6.837/F03/models/teddy.obj  


