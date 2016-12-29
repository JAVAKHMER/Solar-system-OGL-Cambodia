
#ifndef SOLAR_SYSTEM_GLOBALS_H
// starField
#define NUM_STARS 1000
#define FAR_PLANE -20.0f
#define NEAR_PLANE 5.0f
#define GAP 0.0f
#define FIELD_WIDTH ( 30.0f )
#define FIELD_HEIGHT ( 25.0f )
#define DEFAULT_SPEED .01f;
float * g_xyz[NUM_STARS];
// the alpha and red components of polygon to blend
float G_SPEED = DEFAULT_SPEED;
float G_SPEED_INC = 0.0f;
/// this is a pointer to the mp3 stream we will be reading from the disk.
FSOUND_STREAM* g_mp3_stream = NULL;
GLuint _textureIdSun;
GLuint _textureIdMercury;
GLuint _textureIdVenus;
GLuint _textureIdEarth;
GLuint _textureIdMars;
GLuint _textureIdJupiter;
GLuint _textureIdSaturn;
GLuint _textureIdUranus;
GLuint _textureIdNeptun;
GLuint _textureIdSaturnRing;
GLUquadric *quad;
GLfloat ALPHA_PLENET =0;

GLboolean _light1=true;
GLboolean _light2=true;
GLboolean _light3=true;
GLboolean _light4=true;
int mode=true;
int stat; int but;
int Index=0;
GLfloat ALPHA_CIRCLE_SUN = 0;
GLfloat ALPHA_CIRCLE_MERCURY =0;
GLfloat ALPHA_CIRCLE_VENUS =0;
GLfloat ALPHA_CIRCLE_EARTH =0;
GLfloat ALPHA_CIRCLE_MARS =0;
GLfloat ALPHA_CIRCLE_JUPITER =0;
GLfloat ALPHA_CIRCLE_SATURN =0;
GLfloat ALPHA_CIRCLE_URANUS =0;
GLfloat ALPHA_CIRCLE_NEPTUN =0;

const GLfloat RADIUS_SUN = 0.7;
const GLfloat RADIUS_MERCURY = 0.14;
const GLfloat RADIUS_VENUS = 0.245;
const GLfloat RADIUS_EARTH = 0.175;
const GLfloat RADIUS_MARS = 0.1225;
const GLfloat RADIUS_JUPITER = 0.28;
const GLfloat RADIUS_SATURN = 0.245;
const GLfloat RADIUS_URANUS = 0.21;
#define SOLAR_SYSTEM_H
#endif
