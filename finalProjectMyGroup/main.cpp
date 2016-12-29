#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>
#include <GL/glut.h>
#include<time.h>
#include <fmod.h>
#include <fmod_errors.h>
#include<fstream>
#include <wincompat.h>
#include "SolarSystemGlobals.h"
#include "imageloader.h"
#include "material.h"
using namespace std;
mattype matlist[14];
void drawSun();
void drawVenus();
void drawEarth();
void drawJupiter();
void drawMercury();
void drawNeptun();
void drawUranus();
void drawSaturn();
void drawMars();
void drawSaturnRing();
void init(void);
GLuint loadTexture(Image* image);
void initRendering();
void render();
void handleResize(int w, int h);
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);
int mp3();
void matProperty(mattype mat);
void setLight();
void myMouse(int button, int state, int x, int y);
void myMovedMouse(int x, int y);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1350, 730);
	glutCreateWindow("Project");
    initRendering();
    glutTimerFunc(25,update,0);
	glutDisplayFunc(display);
	glutFullScreen();
	 glViewport( 0,0,1350, 730);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMovedMouse);
	glutReshapeFunc(handleResize);
    init();
	mp3();
    glutMainLoop();
	return 0;
}
// initialization
void init(void)
{

   glShadeModel (GL_SMOOTH);
   glClearColor( 0, 0, 0, 0 );
    // set the shading model to 'smooth'
    glShadeModel( GL_SMOOTH );
    // set GL to render front of polygons
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    glLightModeli( GL_FRONT, GL_TRUE );

    glClear( GL_COLOR_BUFFER_BIT );

    // randomly generate
    srand( time( NULL ) );
    for( int i = 0; i < NUM_STARS; i++ )
    {
        g_xyz[i] = new float[3];
        g_xyz[i][0] = ( (float) rand() / RAND_MAX - .5f ) * FIELD_WIDTH;
        g_xyz[i][1] = ( (float) rand() / RAND_MAX - .5f ) * FIELD_HEIGHT;
        g_xyz[i][2] = ( (float) rand() / RAND_MAX ) *
                      ( NEAR_PLANE - FAR_PLANE + GAP ) + FAR_PLANE;
    }

}
//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	quad = gluNewQuadric();

    Image* imageSun = loadBMP("MyImage/Sun.bmp");
	_textureIdSun = loadTexture(imageSun);
	delete imageSun;

    Image* imageMars = loadBMP("MyImage/Mars.bmp");
	_textureIdMars = loadTexture(imageMars);
	delete imageMars;

    Image* imageMercury = loadBMP("MyImage/Mercury.bmp");
	_textureIdMercury = loadTexture(imageMercury);
	delete imageMercury;

    Image* imageVenus = loadBMP("MyImage/Venus.bmp");
	_textureIdVenus = loadTexture(imageVenus);
	delete imageVenus;


	Image* imageEarth = loadBMP("MyImage/Earth.bmp");
	_textureIdEarth = loadTexture(imageEarth);
	delete imageEarth;

	Image* imageJupiter = loadBMP("MyImage/Jupiter.bmp");
	_textureIdJupiter = loadTexture(imageJupiter);
	delete imageJupiter;

	Image* imageSaturn = loadBMP("MyImage/Saturn.bmp");
	_textureIdSaturn = loadTexture(imageSaturn);
	delete imageSaturn;
	Image* imageSaturnRing = loadBMP("MyImage/SaturnRing.bmp");
	_textureIdSaturnRing = loadTexture(imageSaturnRing);
	delete imageSaturnRing;

	Image* imageUranus = loadBMP("MyImage/Uranus.bmp");
	_textureIdUranus = loadTexture(imageUranus);
	delete imageUranus;

	Image* imageNeptun = loadBMP("MyImage/Neptun.bmp");
	_textureIdNeptun = loadTexture(imageNeptun);
	delete imageNeptun;
}
void render()
{
    static float inc = 0.0f;
    inc += 0.001f;
    glRotatef( 50 * cos( inc ), 0.0f, 0.0f, 1.0f );

    G_SPEED += G_SPEED_INC;
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdNeptun);
    for( int i = 0; i < NUM_STARS; i++ )
    {
        glPushMatrix();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTranslatef( g_xyz[i][0], g_xyz[i][1], g_xyz[i][2] );
            glutSolidSphere( .02f, 5, 5 );
        glPopMatrix();
        // increment z
        g_xyz[i][2] += G_SPEED;
        // check to see if passed view
        if( g_xyz[i][2] > NEAR_PLANE + GAP )
        {
            g_xyz[i][0] = ( (float) rand() / RAND_MAX - .5f ) * FIELD_WIDTH;
            g_xyz[i][1] = ( (float) rand() / RAND_MAX - .5f ) * FIELD_HEIGHT;
            g_xyz[i][2] = FAR_PLANE;
        }
        else if( g_xyz[i][2] < FAR_PLANE )
        {
            g_xyz[i][0] = ( (float) rand() / RAND_MAX - .5f ) * FIELD_WIDTH;
            g_xyz[i][1] = ( (float) rand() / RAND_MAX - .5f ) * FIELD_HEIGHT;
            g_xyz[i][2] = NEAR_PLANE;
        }
    }
}
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(45.0, (float)w/h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1,0.0);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

setLight();

 new_material(Turquoise);
 matlist[0]=material;

 new_material(Turquoise);
 matlist[1]=material;

 new_material(Turquoise);
 matlist[2]=material;

 new_material(Turquoise);
 matlist[3]=material;

 new_material(Turquoise);
 matlist[4]=material;

    glPushMatrix( );
        render( );
    glPopMatrix( );

	glPushMatrix();
        drawSun();
        drawNeptun();
        drawUranus();
        drawSaturn();
        drawSaturnRing();
        drawJupiter();
        drawMars();
        drawEarth();
        drawVenus();
        drawMercury();
glPopMatrix();
 glutSwapBuffers();
 glFlush();
}
void update(int value)
{
/*
    ALPHA_PLENET += 36;

    ALPHA_CIRCLE_MERCURY += 36/(float)88;
    ALPHA_CIRCLE_VENUS += 36/(float)225;
    ALPHA_CIRCLE_EARTH += 36/(float)365;
    ALPHA_CIRCLE_MARS += 36/(float)686;
    ALPHA_CIRCLE_JUPITER += 36/(float)4333;
    ALPHA_CIRCLE_SATURN += 36/(float)10759;
    ALPHA_CIRCLE_URANUS += 36/(float)30685;
    ALPHA_CIRCLE_NEPTUN += 36/(float)60190;

    if(ALPHA_CIRCLE_MERCURY >= 360) ALPHA_CIRCLE_MERCURY =0;
    if(ALPHA_CIRCLE_VENUS >= 360) ALPHA_CIRCLE_VENUS =0;
    if(ALPHA_CIRCLE_EARTH >= 360) ALPHA_CIRCLE_EARTH =0;
    if(ALPHA_CIRCLE_MARS >= 360) ALPHA_CIRCLE_MARS =0;
    if(ALPHA_CIRCLE_JUPITER >= 360) ALPHA_CIRCLE_JUPITER =0;
    if(ALPHA_CIRCLE_SATURN >= 360) ALPHA_CIRCLE_SATURN =0;
    if(ALPHA_CIRCLE_URANUS >= 360) ALPHA_CIRCLE_URANUS =0;
    if(ALPHA_CIRCLE_NEPTUN >= 360) ALPHA_CIRCLE_NEPTUN =0;

    if(ALPHA_PLENET >=360) ALPHA_PLENET=0;
*/
       ALPHA_PLENET += 36/5;
    ALPHA_CIRCLE_SUN += 0.1;
    ALPHA_CIRCLE_MERCURY += 36/(float)88 + 0.4;
    ALPHA_CIRCLE_VENUS += 36/(float)225 + 0.4;
    ALPHA_CIRCLE_EARTH += 36/(float)365 + 0.4;
    ALPHA_CIRCLE_MARS += 36/(float)686 + 0.4;
    ALPHA_CIRCLE_JUPITER += 36/(float)4333 + 0.4;
    ALPHA_CIRCLE_SATURN += 36/(float)10759 + 0.3;
    ALPHA_CIRCLE_URANUS += 36/(float)30685 + 0.2;
    ALPHA_CIRCLE_NEPTUN += 36/(float)60190 + 0.1;

    if(ALPHA_CIRCLE_MERCURY >= 360) ALPHA_CIRCLE_MERCURY =0;
    if(ALPHA_CIRCLE_VENUS >= 360) ALPHA_CIRCLE_VENUS =0;
    if(ALPHA_CIRCLE_EARTH >= 360) ALPHA_CIRCLE_EARTH =0;
    if(ALPHA_CIRCLE_MARS >= 360) ALPHA_CIRCLE_MARS =0;
    if(ALPHA_CIRCLE_JUPITER >= 360) ALPHA_CIRCLE_JUPITER =0;
    if(ALPHA_CIRCLE_SATURN >= 360) ALPHA_CIRCLE_SATURN =0;
    if(ALPHA_CIRCLE_URANUS >= 360) ALPHA_CIRCLE_URANUS =0;
    if(ALPHA_CIRCLE_NEPTUN >= 360) ALPHA_CIRCLE_NEPTUN =0;
    if(ALPHA_CIRCLE_SUN >= 360 ) ALPHA_CIRCLE_SUN = 0;

    if(ALPHA_PLENET >=360) ALPHA_PLENET=0;

    glutPostRedisplay();
    glutTimerFunc(25,update,0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
    case 'y' : glRotatef(1,1.0,0.0,0);break;
    case 'x' : glRotatef(1,0.0,0.1,0);break;
    case 'z' : glRotatef(1,0.0,0.0,1);break;
    case 27:
			exit(0);
	}
	 glutPostRedisplay();
}
int mp3()
{
    if( FSOUND_Init(4400,64,0) == FALSE )
	{
		cout << "[ERROR] Could not initialise fmod\n";
		return 0;
	}
	// attempt to open the mp3 file as a stream
	g_mp3_stream = FSOUND_Stream_Open( "audio solar system by bong rith kon khmer.mp3" , FSOUND_2D , 0 , 0 );
	//g_mp3_stream = FSOUND_Stream_Open( "OpenGL Solar System.mp3" , FSOUND_2D , 0 , 0 );
    g_mp3_stream = FSOUND_Stream_Open( "Solar System Sound.mp3" , FSOUND_2D , 0 , 0 );
	// make sure mp3 opened OK
	if(!g_mp3_stream) {
		cout << "[ERROR] could not open file\n";
		return 0;
	}
	FSOUND_Stream_Play(0,g_mp3_stream);
	return 0;
}

void myMouse(int button, int state, int x, int y){
      but=button;
      stat=state;
}

void myMovedMouse(int x, int y){

       if (but==GLUT_LEFT_BUTTON)
             glRotatef(-1,0,0,1);

 glutPostRedisplay();

}
void setLight(){


   GLfloat diffuse0[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat diffuse1[] = {1.0, 0.0, 0.0, 1.0};
   GLfloat diffuse2[] = {0.0, 1.0, 0.0, 1.0};
   GLfloat diffuse3[] = {0.0, 0.0, 1.0, 1.0};

   GLfloat light_position0[] = { 0.0, 0.0, 0.1, 1.0 };
   GLfloat light_position1[] = { -3.0, 3.0, 0.0, 1.0 };
   GLfloat light_position2[] = { 0.0,3.0, 3.0, 1.0 };
   GLfloat light_position3[] = { 2.0, 3.0, 0.0, 1.0 };
   GLfloat light_position4[] = {0.0,0.0,1.0,1.0};

   GLfloat direction1[]={1.0,-1.0,0.0};
   GLfloat direction2[]={0.0,1.0,2.0};
   GLfloat direction3[]={0.5,1.0,2.0};
   GLfloat direction4[]={0.0,0.0,5.0};

   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
   glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3);

   //Position the lights
   glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
   glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
   glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
   glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
   glLightfv(GL_LIGHT3, GL_POSITION, light_position4);

   //spot direction
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
   glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction2);
   glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction3);
   glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction4);


   //exponent propertie defines the concentration of the light
   glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0f);
   glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15.0f);
   glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 15.0f);

   //angle of the cone light emitted by the spot : value between 0 to 180
   glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
   glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90);
   glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90);

   GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);


   glEnable(GL_LIGHTING);


   glEnable(GL_LIGHT0);

   if(_light1==true)
     glEnable(GL_LIGHT1);
   else
     glDisable(GL_LIGHT1);

   if(_light2==true)
     glEnable(GL_LIGHT2);
   else
     glDisable(GL_LIGHT2);

   if(_light3==true)
     glEnable(GL_LIGHT3);
   else
     glDisable(GL_LIGHT3);

   if(_light4==true)
     glEnable(GL_LIGHT4);
   else
     glDisable(GL_LIGHT4);


   glShadeModel (GL_SMOOTH);
 }

void matProperty(mattype mat){
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.a );
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.d);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.s);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat.e);
    glMaterialf(GL_FRONT, GL_SHININESS, mat.sh);
}

void drawSun()
{
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureIdSun);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(quad,1);
        glRotated(ALPHA_CIRCLE_SUN,0,1,0);
        gluSphere(quad,RADIUS_SUN,60,60);
}
void drawMercury()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
             glRotated(ALPHA_CIRCLE_MERCURY,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdMercury);
            glTranslated(1,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad,RADIUS_MERCURY,50,50);
            glPopMatrix();
}
void drawVenus()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glRotated(ALPHA_CIRCLE_VENUS,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdVenus);
            glTranslated(1.4,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad,RADIUS_VENUS,50,50);
            glPopMatrix();
}
void drawEarth()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glRotated(ALPHA_CIRCLE_EARTH,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdEarth);
            glTranslated(2,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad , RADIUS_EARTH ,50,50);
            glPopMatrix();
}
void drawMars()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glRotated(ALPHA_CIRCLE_MARS,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdMars);
            glTranslated(2.5,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad,RADIUS_MARS,50,50);
            glPopMatrix();
}
void drawJupiter()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glRotated(ALPHA_CIRCLE_JUPITER,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdJupiter);
            glTranslated(3,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad,RADIUS_JUPITER,50,50);
            glPopMatrix();
}
void drawSaturn()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glRotated(ALPHA_CIRCLE_SATURN,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdSaturn);
            glTranslated(3.5,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad,RADIUS_SATURN,50,50);
            glPopMatrix();
}
void drawSaturnRing()
{
    glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glRotated(ALPHA_CIRCLE_SATURN,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdSaturnRing);
            glTranslated(3.5,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            glScalef(1,1,.02);
            gluSphere(quad,RADIUS_SATURN*2,50,50);
            glPopMatrix();
}
void drawUranus()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
             glRotated(ALPHA_CIRCLE_URANUS,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdUranus);
            glTranslated(4,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad,RADIUS_URANUS,50,50);
            glPopMatrix();
}
void drawNeptun()
{
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glRotated(ALPHA_CIRCLE_NEPTUN,0,1,0);
            glBindTexture(GL_TEXTURE_2D, _textureIdNeptun);
            glTranslated(4.5,0,0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glRotatef(90,1.0f,0.0f,0.0f);
            glRotatef(ALPHA_PLENET,0.0f,0.0f,1.0f);
            gluQuadricTexture(quad,1);
            gluSphere(quad,0.21,50,50);
            glPopMatrix();
}
