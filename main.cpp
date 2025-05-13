
#include "shaderutils.h"
#include "Mesh.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 


Mesh mesh("pov/cat.pov");           // global mesh variable
float angle = 0;                    // angle of rotation updated on idle

int currFunc = 0;                   // global function choice : function_() 
float f = 15;                       // global value, set by user to be incorporated into shader functions
int k = 3;                          // same as f
float t = 0.1;                      // same as f

int frame = 1;                      // frame variable that constantly increases, if flow enabled
bool flow = false;                  // flag that enables or disables effect flow
bool forwardFlag = true;            // flag to reverse direction of effect flow

bool rFlag = true;                  //flag to toggle Y-Axis rotation

GLint program;                      // global program variable


// load the shader program and load the shape
void init(void)
{
  glClearColor( 0, 1, 0, 0 );
  glEnable( GL_DEPTH_TEST );              

  // load the shaders and combine into shader program; enable use of program
  program = loadProgram( "vertexShader.glsl", "fragmentShader.glsl" );
  glUseProgram( program );

  //setup data and data layout buffers for the mesh
  mesh.setupBuffers();
}


void display(void)
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // get current active program -- created in init() -- needed to access shader variables
  glGetIntegerv( GL_CURRENT_PROGRAM, &program );

  // get the name of the uniform/shared variable in (one of the) shaders
  GLuint angleVar = glGetUniformLocation(program, "angle");
  glUniform1f(angleVar, angle);

  //register currFunc as uniform variable for fragmentShader to use
  GLuint choice = glGetUniformLocation(program, "currFunc");
  glUniform1i(choice, currFunc);

  //register f as uniform variable for fragmentShader to use
  GLuint userF = glGetUniformLocation(program, "f");
  glUniform1f(userF, f);

  //register k as uniform variable for fragment shader
  GLuint userK = glGetUniformLocation(program, "k");
  glUniform1i(userK, k);

  //register t as uniform variable for fragment shader
  GLuint userT = glGetUniformLocation(program, "t");
  glUniform1f(userT, t);

  //register frame and flow variables as uniforms
  GLuint frameVal = glGetUniformLocation(program, "frame");
  glUniform1i(frameVal, frame);
  
  GLuint flowFlag = glGetUniformLocation(program, "flow");
  glUniform1i(flowFlag, flow);

  mesh.draw();

  glutSwapBuffers();
}


void idle()
{
    if(rFlag) angle += 0.0001;

    if (flow)
    {
        if (forwardFlag) frame += 2;            //which direction to flow
        else frame -= 2;
    }

    glutPostRedisplay();
}


void keyboard( unsigned char key, int x, int y )
{
    string filename;                        //filename for when user changes mesh

    switch (key)
    {
        case 27:
            exit(0);
            break;

        case '0':                           //choose function to use
        case '1':
        case '2':
            currFunc = key - '0';
            break;
        case '3':
            f = 90;
            currFunc = key - '0';
            break;
        case '4':
            f = 15;
            currFunc = key - '0';
            break;
        case '5':
            f = 15;
            currFunc = key - '0';
            break;
        case '6':
            f = 10;
            currFunc = key - '0';
            break;
        case '7':
            f = 7;
            k = 3; 
            currFunc = key - '0';
            break;
        case '8':
            f = 8;
            k = 5;
            t = 0.2;
            currFunc = key - '0';           
            break;

        case 'f':                           //adjust f value
            f++;
            break;
        case 'F':
            f--;
            break;

        case 'k':                           //adjust k value
            k++;
            break;
        case 'K':
            k--;
            break;

        case 't':                           //adjust t value
            t += 0.05;
            break;
        case 'T':
            t -= 0.05;
            break;

        case 'n':                           //toggle flow (stops in place)
            if (flow) flow = false;
            else flow = true;
        case 'm':                           //reverse flow direction
            if (forwardFlag) forwardFlag = false;
            else forwardFlag = true;
            break;
        case 'b':                           //reset flow back to start
            frame = 1;
            break;

        case 'r':                           //toggle rotation
            if (rFlag) rFlag = false;
            else rFlag = true;
            break;


        case '?':
            cout << "Enter a file path to a mesh:" << endl;
            cin >> filename;

            mesh = Mesh(filename);
            mesh.setupBuffers();             //must setup buffers again after change mesh (only done once in init())

            break;

        default:
            break;
    }

    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);

  glutInitContextVersion( 3, 0 );
  glutInitContextProfile( GLUT_CORE_PROFILE );
  glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutCreateWindow( "Special Effects - OpenGLSL" );

  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutIdleFunc( idle );
  
  glewExperimental = GL_TRUE;
  glewInit();

  init();

  glutMainLoop();
}
