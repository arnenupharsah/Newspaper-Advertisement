#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#define   PI            3.14159 
#define WIN_WIDTH      600 
#define WIN_HEIGHT      600    
#define CYCLE_LENGTH   3.3f 
#define ROD_RADIUS      0.05f 
#define NUM_SPOKES      20 
#define SPOKE_ANGLE      18 
#define RADIUS_WHEEL   1.0f 
#define TUBE_WIDTH      0.08f 
#define RIGHT_ROD      1.6f 
#define RIGHT_ANGLE      48.0f 
#define MIDDLE_ROD      1.7f 
#define MIDDLE_ANGLE   106.0f 
#define BACK_CONNECTOR   0.5f 
#define LEFT_ANGLE      50.0f 
#define WHEEL_OFFSET   0.11f 
#define WHEEL_LEN      1.1f 
#define TOP_LEN         1.5f 
#define CRANK_ROD      0.7f 
#define CRANK_RODS      1.12f 
#define CRANK_ANGLE      8.0f 
#define HANDLE_ROD      1.2f 
#define FRONT_INCLINE   70.0f 
#define HANDLE_LIMIT   70.0f 

#define INC_STEERING   2.0f 
#define INC_SPEED      0.05f 

/*****************************************
*    All the Global Variables are Here
****************************************/
/*****************************************
*   Cycle - related variables
******************************************/
GLfloat pedalAngle, speed, steering;

/*******************************
*   User view realted variables
********************************/
GLfloat camx, camy, camz;
GLfloat anglex, angley, anglez;

/****************************
*   Mouse related variables
****************************/
int prevx, prevy;
GLenum Mouse;

/**************************
*   Cycle position related
*   variables
***************************/
GLfloat xpos, zpos, direction;

void ZCylinder(GLfloat radius, GLfloat length);
void XCylinder(GLfloat radius, GLfloat length);

void drawFrame(void);
void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth);
void drawChain(void);
void drawPedals(void);
void drawTyre(void);
void drawSeat(void);
void help(void);
void init(void);
void reset(void);
void display(void);
void idle(void);
void updateScene(void);
void landmarks(void);
void special(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void reshape(int w, int h);
void glSetupFuncs(void);

GLfloat Abs(GLfloat);
GLfloat degrees(GLfloat);
GLfloat radians(GLfloat);
GLfloat angleSum(GLfloat, GLfloat);

void ZCylinder(GLfloat radius, GLfloat length)
{
	GLUquadricObj *cylinder;
	cylinder = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	gluCylinder(cylinder, radius, radius, length, 15, 5);
	glPopMatrix();
}

void XCylinder(GLfloat radius, GLfloat length)
{
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	ZCylinder(radius, length);
	glPopMatrix();
}



// angleSum(a,b) = (a+b) MOD 2*PI 
// a and b are two angles (radians) 
//  both between 0 and 2*PI 
GLfloat angleSum(GLfloat a, GLfloat b)
{
	a += b;
	if (a < 0) return a + 2 * PI;
	else if (a > 2 * PI) return a - 2 * PI;
	else return a;
}

/************************************************
*   Draw the metal frame of the cycle and also
*   draw the seat and the back wheel with
*   this.
*   All these parts are always together in the
*   same plane.They never move out ot the
*   PLANE!   ;)
************************************************/
void drawFrame()
{
	glColor3f(1.0f, 0.0f, 0.0f);

	/********************************
	*   First draw the all the items
	*   at the center of the frame.
	*   Draw the bigger gear,the small
	*   cylinder acting as the socket
	*   for the pedals.Also DON'T
	*   forget to draw the two
	*   connecting cemtral rods
	*********************************/
	glPushMatrix();
	/******************************
	*   Allow me to draw the BIGGER
	*   gear and the socket cylinder
	*******************************/
	glPushMatrix();
	/***************************
	*   Let the gear have the
	*   green color
	***************************/
	glColor3f(0.0f, 1.0f, 0.0f);

	/**************************
	*   The gear should be
	*   outside the frame !!!
	*   This is the bigger
	*   GEAR
	***************************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.06f);
	glRotatef(-2 * pedalAngle, 0.0f, 0.0f, 1.0f);
	gear(0.08f, 0.3f, 0.03f, 30, 0.03f);
	glPopMatrix();
	/***************************
	*   Restore the color of the
	*   frame
	****************************/
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.2f);
	ZCylinder(0.08f, 0.32f);
	glPopMatrix();
	/*****************************
	*   Lets first draw the
	*   rightmost rod of the frame
	*******************************/
	glRotatef(RIGHT_ANGLE, 0.0f, 0.0f, 1.0f);
	XCylinder(ROD_RADIUS, RIGHT_ROD);

	/*******************************
	*   Now draw the centre rod of
	*   the frame which also supports
	*   the seat
	*********************************/
	glRotatef(MIDDLE_ANGLE - RIGHT_ANGLE, 0.0f, 0.0f, 1.0f);
	XCylinder(ROD_RADIUS, MIDDLE_ROD);
	/********************************
	*   We have drawn the support.So
	*   let's draw the seat with a
	*   new color
	*********************************/
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(MIDDLE_ROD, 0.0f, 0.0f);
	glRotatef(-MIDDLE_ANGLE, 0.0f, 0.0f, 1.0f);
	glScalef(0.3f, ROD_RADIUS, 0.25f);
	drawSeat();
	/**********************
	*   Restore the color !
	************************/
	glColor3f(1.0f, 0.0f, 0.0f);
	glPopMatrix();
	/*********************************
	*   Draw the horizontal part of
	*   the frame.
	*********************************/

	/*********************************
	*   Draw the main single rod
	*   connecting the center of the
	*   frame to the back wheel of the
	*   cycle
	**********************************/
	glPushMatrix();
	glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
	XCylinder(ROD_RADIUS, BACK_CONNECTOR);

	/***********************************
	*   Draw the two rods on the either
	*   side of the wheel
	*   These rods are part of the
	*   horizontal part of the cycle
	************************************/
	glPushMatrix();
	glTranslatef(0.5f, 0.0f, WHEEL_OFFSET);
	XCylinder(ROD_RADIUS, RADIUS_WHEEL + TUBE_WIDTH);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.5f, 0.0f, -WHEEL_OFFSET);
	XCylinder(ROD_RADIUS, RADIUS_WHEEL + TUBE_WIDTH);
	glPopMatrix();
	glPopMatrix();

	/************************************
	*   Draw the leftmost rods of the
	*   frame of the cycle
	*************************************/
	glPushMatrix();
	glTranslatef(-(BACK_CONNECTOR + RADIUS_WHEEL + TUBE_WIDTH), 0.0f, 0.0f);
	/********************************
 *   Transalted to the back wheel
 *   position.Why not draw the back
 *   wheel and also the gear ? :))
 **********************************/
	glPushMatrix();
	glRotatef(-2 * pedalAngle, 0.0f, 0.0f, 1.0f);
	drawTyre();
	glColor3f(0.0f, 1.0f, 0.0f);
	gear(0.03f, 0.15f, 0.03f, 20, 0.03f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPopMatrix();
	glRotatef(LEFT_ANGLE, 0.0f, 0.0f, 1.0f);

	/************************************
	*   Draw the two rods on the either
	*   side of the wheel connecting the
	*   backwheel and topmost horizontal
	*   part of the wheel
	*************************************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -WHEEL_OFFSET);
	XCylinder(ROD_RADIUS, WHEEL_LEN);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, WHEEL_OFFSET);
	XCylinder(ROD_RADIUS, WHEEL_LEN);
	glPopMatrix();

	/*****************************
	*   Draw the single rod of the
	*   same setup
	******************************/
	glTranslatef(WHEEL_LEN, 0.0f, 0.0f);
	XCylinder(ROD_RADIUS, CRANK_ROD);

	/*****************************
	*   Now Draw The topmost
	*   Horizontal rod
	*****************************/
	glTranslatef(CRANK_ROD, 0.0f, 0.0f);
	glRotatef(-LEFT_ANGLE, 0.0f, 0.0f, 1.0f);
	XCylinder(ROD_RADIUS, TOP_LEN);

	/*******************************
	*   Now instead of again traversing
	*   all the way back and again
	*   forward.WHY NOT DRAW THE
	*   HANDLE FROM HERE ITSELF?
	********************************/
	/*****************************
	*   Now draw the handle and
	*   small support rod which
	*   is incorporated in the
	*   frame itself.
	*   Set y-axis at te required
	*   incline.
	******************************/
	glTranslatef(TOP_LEN, 0.0f, 0.0f);
	glRotatef(-FRONT_INCLINE, 0.0f, 0.0f, 1.0f);

	/******************************
	*   Draw the small rod that acts
	*   as the socket joining the
	*   frame and the handle
	******************************/
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	XCylinder(ROD_RADIUS, 0.45f);
	glPopMatrix();

	/******************************
	*   I Hope the handle can rotate
	*   about its mean position
	*******************************/
	glPushMatrix();
	glRotatef(-steering, 1.0f, 0.0f, 0.0f);
	/******************************
	*   Roll back to the height of
	*   the handle to draw it
	*******************************/
	glTranslatef(-0.3f, 0.0f, 0.0f);

	/********************************
	*   We cannot use the incline
	*   the incline to draw the
	*   horizontal part of the rod
	********************************/
	glPushMatrix();
	glRotatef(FRONT_INCLINE, 0.0f, 0.0f, 1.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -HANDLE_ROD / 2);
	ZCylinder(ROD_RADIUS, HANDLE_ROD);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -HANDLE_ROD / 2);
	ZCylinder(0.07f, HANDLE_ROD / 4);
	glTranslatef(0.0f, 0.0f, HANDLE_ROD * 3 / 4);
	ZCylinder(0.07f, HANDLE_ROD / 4);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPopMatrix();
	glPopMatrix();

	/*********************************
	*   Using this incline now draw
	*   the handle.Maybe use this again
	*   to draw the wheel. ;)
	**********************************/
	glPushMatrix();
	/****************************
	*   Draw the main big rod
	****************************/
	XCylinder(ROD_RADIUS, CRANK_ROD);

	/******************************
	*   Why not draw the two rods and
	*   the WHEEL?   :)
	*   Yes!So,first go to the
	*   end of the main rod.
	*******************************/
	glTranslatef(CRANK_ROD, 0.0f, 0.0f);
	glRotatef(CRANK_ANGLE, 0.0f, 0.0f, 1.0f);

	/*******************************
	*   Draw the two rods connecting
	*   the handle and the front
	*   wheel.
	*   The two rods are at a incline
	*   to the connector.
	********************************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, WHEEL_OFFSET);
	XCylinder(ROD_RADIUS, CRANK_RODS);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -WHEEL_OFFSET);
	XCylinder(ROD_RADIUS, CRANK_RODS);
	glPopMatrix();
	/********************************
 *   Why not draw the wheel.
 *   The FRONT wheel to be precise
 *********************************/
	glTranslatef(CRANK_RODS, 0.0f, 0.0f);
	glRotatef(-2 * pedalAngle, 0.0f, 0.0f, 1.0f);
	drawTyre();
	glPopMatrix();
	glPopMatrix();   /*   End of the rotation of the handle effect   */
	glPopMatrix();
}


void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
	GLint teeth, GLfloat tooth_depth)
{
	GLint i;
	GLfloat r0, r1, r2;
	GLfloat angle, da;
	GLfloat u, v, len;
	const double pi = 3.14159264;

	r0 = inner_radius;
	r1 = outer_radius - tooth_depth / 2.0;
	r2 = outer_radius + tooth_depth / 2.0;

	da = 2.0*pi / teeth / 4.0;

	glShadeModel(GL_FLAT);

	glNormal3f(0.0, 0.0, 1.0);

	/* draw front face */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0*pi / teeth;
		glVertex3f(r0*cos(angle), r0*sin(angle), width*0.5);
		glVertex3f(r1*cos(angle), r1*sin(angle), width*0.5);
		glVertex3f(r0*cos(angle), r0*sin(angle), width*0.5);
		glVertex3f(r1*cos(angle + 3 * da), r1*sin(angle + 3 * da), width*0.5);
	}
	glEnd();

	/* draw front sides of teeth */
	glBegin(GL_QUADS);
	da = 2.0*pi / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0*pi / teeth;

		glVertex3f(r1*cos(angle), r1*sin(angle), width*0.5);
		glVertex3f(r2*cos(angle + da), r2*sin(angle + da), width*0.5);
		glVertex3f(r2*cos(angle + 2 * da), r2*sin(angle + 2 * da), width*0.5);
		glVertex3f(r1*cos(angle + 3 * da), r1*sin(angle + 3 * da), width*0.5);
	}
	glEnd();


	glNormal3f(0.0, 0.0, -1.0);

	/* draw back face */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0*pi / teeth;
		glVertex3f(r1*cos(angle), r1*sin(angle), -width * 0.5);
		glVertex3f(r0*cos(angle), r0*sin(angle), -width * 0.5);
		glVertex3f(r1*cos(angle + 3 * da), r1*sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r0*cos(angle), r0*sin(angle), -width * 0.5);
	}
	glEnd();

	/* draw back sides of teeth */
	glBegin(GL_QUADS);
	da = 2.0*pi / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0*pi / teeth;

		glVertex3f(r1*cos(angle + 3 * da), r1*sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r2*cos(angle + 2 * da), r2*sin(angle + 2 * da), -width * 0.5);
		glVertex3f(r2*cos(angle + da), r2*sin(angle + da), -width * 0.5);
		glVertex3f(r1*cos(angle), r1*sin(angle), -width * 0.5);
	}
	glEnd();


	/* draw outward faces of teeth */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0*pi / teeth;

		glVertex3f(r1*cos(angle), r1*sin(angle), width*0.5);
		glVertex3f(r1*cos(angle), r1*sin(angle), -width * 0.5);
		u = r2 * cos(angle + da) - r1 * cos(angle);
		v = r2 * sin(angle + da) - r1 * sin(angle);
		len = sqrt(u*u + v * v);
		u /= len;
		v /= len;
		glNormal3f(v, -u, 0.0);
		glVertex3f(r2*cos(angle + da), r2*sin(angle + da), width*0.5);
		glVertex3f(r2*cos(angle + da), r2*sin(angle + da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
		glVertex3f(r2*cos(angle + 2 * da), r2*sin(angle + 2 * da), width*0.5);
		glVertex3f(r2*cos(angle + 2 * da), r2*sin(angle + 2 * da), -width * 0.5);
		u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
		v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
		glNormal3f(v, -u, 0.0);
		glVertex3f(r1*cos(angle + 3 * da), r1*sin(angle + 3 * da), width*0.5);
		glVertex3f(r1*cos(angle + 3 * da), r1*sin(angle + 3 * da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
	}

	glVertex3f(r1*cos(0.0), r1*sin(0.0), width*0.5);
	glVertex3f(r1*cos(0.0), r1*sin(0.0), -width * 0.5);

	glEnd();


	glShadeModel(GL_SMOOTH);

	/* draw inside radius cylinder */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0*pi / teeth;
		glNormal3f(-cos(angle), -sin(angle), 0.0);
		glVertex3f(r0*cos(angle), r0*sin(angle), -width * 0.5);
		glVertex3f(r0*cos(angle), r0*sin(angle), width*0.5);
	}
	glEnd();

}

/******************************************
*   Could not model the exact chain
*   Think it eats up a lot of power if
*   approximated by a lot of spheres
*   So approximated with the stippled
*   lines instead
******************************************/
void drawChain()
{
	GLfloat depth;
	static int mode = 0;

	glColor3f(0.0f, 1.0f, 0.0f);
	glEnable(GL_LINE_STIPPLE);
	mode = (mode + 1) % 2;

	if (mode == 0 && speed > 0)
		glLineStipple(1, 0x1c47);
	else if (mode == 1 && speed > 0)
		glLineStipple(1, 0x00FF);

	glBegin(GL_LINES);
	for (depth = 0.06f; depth <= 0.12f; depth += 0.01f)
	{
		glVertex3f(-1.6f, 0.15f, ROD_RADIUS);
		glVertex3f(0.0f, 0.3f, depth);

		glVertex3f(-1.6f, -0.15f, ROD_RADIUS);
		glVertex3f(0.0f, -0.3f, depth);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

void drawSeat()
{
	/*********************************
	*   Draw the top of the seat
	**********************************/
	glBegin(GL_POLYGON);
	glVertex3f(-0.1f, 1.0f, -0.5f);
	glVertex3f(1.0f, 1.0f, -0.3f);
	glVertex3f(1.0f, 1.0f, 0.3f);
	glVertex3f(-0.1f, 1.0f, 0.5f);
	glVertex3f(-0.5f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-0.5f, 1.0f, -1.0f);
	glEnd();

	/**********************************
	*   Draw the bottom base part of the
	*   seat
	************************************/
	glBegin(GL_POLYGON);
	glVertex3f(-0.1f, -1.0f, -0.5f);
	glVertex3f(1.0f, -1.0f, -0.3f);
	glVertex3f(1.0f, -1.0f, 0.3f);
	glVertex3f(-0.1f, -1.0f, 0.5f);
	glVertex3f(-0.5f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-0.5f, -1.0f, -1.0f);
	glEnd();

	/**********************
	*   Draw the sides!
	***********************/
	glBegin(GL_QUADS);
	glVertex3f(1.0f, 1.0f, -0.3f);
	glVertex3f(1.0f, 1.0f, 0.3f);
	glVertex3f(1.0f, -1.0f, 0.3f);
	glVertex3f(1.0f, -1.0f, -0.3f);

	glVertex3f(1.0f, 1.0f, 0.3f);
	glVertex3f(-0.1f, 1.0f, 0.5f);
	glVertex3f(-0.1f, -1.0f, 0.5f);
	glVertex3f(1.0f, -1.0f, 0.3f);

	glVertex3f(1.0f, 1.0f, -0.3f);
	glVertex3f(-0.1f, 1.0f, -0.5f);
	glVertex3f(-0.1f, -1.0f, -0.5f);
	glVertex3f(1.0f, -1.0f, -0.3f);

	glVertex3f(-0.1f, 1.0f, 0.5f);
	glVertex3f(-0.5f, 1.0f, 1.0f);
	glVertex3f(-0.5f, -1.0f, 1.0f);
	glVertex3f(-0.1f, -1.0f, 0.5f);

	glVertex3f(-0.1f, 1.0f, -0.5f);
	glVertex3f(-0.5f, 1.0f, -1.0f);
	glVertex3f(-0.5f, -1.0f, -1.0f);
	glVertex3f(-0.1f, -1.0f, -0.5f);

	glVertex3f(-0.5f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-0.5f, -1.0f, 1.0f);

	glVertex3f(-0.5f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-0.5f, -1.0f, -1.0f);

	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();


}

void drawPedals()
{
	glColor3f(0.0f, 0.0f, 1.0f);
	/***************************
	*   Lets draw the two pedals
	*   offset from the center
	*   of the frame.
	****************************/

	/*****************************
	*   First draw the one visible
	*   to the viewer
	******************************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.105f);
	glRotatef(-pedalAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.25f, 0.0f, 0.0f);
	/*************************
	*   Draw the pedal rod
	*************************/
	glPushMatrix();
	glScalef(0.5f, 0.1f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	/************************
	*   Draw the resting pad
	************************/
	glPushMatrix();
	glTranslatef(0.25f, 0.0f, 0.15f);
	glRotatef(pedalAngle, 0.0f, 0.0f, 1.0f);
	glScalef(0.2f, 0.02f, 0.3f);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();

	/*******************************
	*   Draw the one on the other
	*   side  of the frame
	*******************************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.105f);
	glRotatef(180.0f - pedalAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.25f, 0.0f, 0.0f);

	/***************************
	*   Now again draw the pedal
	*   rod
	****************************/
	glPushMatrix();
	glScalef(0.5f, 0.1f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	/****************************
	*   Draw the resting pad of
	*   the pedal
	*****************************/
	glPushMatrix();
	glTranslatef(0.25f, 0.0f, -0.15f);
	glRotatef(pedalAngle - 180.0f, 0.0f, 0.0f, 1.0f);
	glScalef(0.2f, 0.02f, 0.3f);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();

	glColor3f(1.0f, 0.0f, 0.0f);
}

void drawTyre(void)
{
	int i;
	//   Draw The Rim 
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidTorus(0.06f, 0.92f, 4, 30);
	//   Draw The Central Cylinder 
	//   Length of cylinder  0.12f 
	glColor3f(1.0f, 1.0f, 0.5f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.06f);
	ZCylinder(0.02f, 0.12f);
	glPopMatrix();
	glutSolidTorus(0.02f, 0.02f, 3, 20);

	//   Draw The Spokes 
	glColor3f(1.0f, 1.0f, 1.0f);
	for (i = 0; i < NUM_SPOKES; ++i)
	{
		glPushMatrix();
		glRotatef(i * SPOKE_ANGLE, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.02f, 0.0f);
		glVertex3f(0.0f, 0.86f, 0.0f);
		glEnd();
		glPopMatrix();
	}

	//   Draw The Tyre 
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidTorus(TUBE_WIDTH, RADIUS_WHEEL, 10, 30);
	glColor3f(1.0f, 0.0f, 0.0f);
}

void cycle(void)
{
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);*/

	glPushMatrix();
	/*******************************
	 *    Prepare the rotations
	 *    and start doing the
	 *    remaining scene
	 *******************************/
	glRotatef(angley, 1.0f, 0.0f, 0.0f);
	glRotatef(anglex, 0.0f, 1.0f, 0.0f);
	glRotatef(anglez, 0.0f, 0.0f, 1.0f);

	/***********************
	 *    Start rendering
	 *    the scene;
	 *    the bicycle ;)
	 **********************/

	 //landmarks();

	 /****************************
	 *   Move the cycle.
	 ****************************/
	glPushMatrix();
	glTranslatef(xpos, 0.0f, zpos);
	glRotatef(direction, 0.0f, 1.0f, 0.0f);

	drawFrame();
	drawChain();
	drawPedals();
	glPopMatrix();

	glPopMatrix();

	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camx, camy, camz, camx, 0.0, 0.0, 0.0, 1.0, 0.0);

	glutSwapBuffers();*/
}


static GLdouble viewer[] = { 0.0,0.0,5.0 };



void wall(double thickness)
{
	glPushMatrix();
	glTranslated(0.5, 0.0*thickness, 0.5);
	glScaled(100.0, thickness, 80.0);
	glutSolidCube(1);
	glPopMatrix();
}

void drawSuface() {
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);

	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();
}

void drawText1(float x, float y, float z, float r, float g, float b, const char* string)
{
	int j = strlen(string);
	//	glLineWidth(20.0);
	glColor3f(r, g, b);//1.0, 0.24902, 0.0
	glRasterPos3f(x, y, z);
	for (int i = 0; i < j; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void drawPolygon(float x, float y, float z, float r, float g, float b, const char* str) {

	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad

	glColor3d(1, 1, 1);
	glVertex3f(-12, -8, -50);
	glVertex3f(12, -8, -50);
	glVertex3f(12, 8, -50);
	glVertex3f(-12, 8, -50);
	glEnd();

	drawText1(-8, 5, -4, 0, 0, 0, "Modi at mega rally in Varansi for Elections 2019");

	//Photo1
	glBegin(GL_QUADS);
	glColor3d(0.8, 0.6, 0.8);
	glVertex3f(-3, -2, -48);
	glVertex3f(3, -2, -48);
	glVertex3f(3, 4, -48);
	glVertex3f(-3, 4, -48);
	glEnd();

	//Mike
	glBegin(GL_QUADS);
	glColor3f(0.4, 0.4, 0.6);
	glVertex3f(-2.0, 5.8, 4);
	glVertex3f(-1.9, 5.8, 4);
	glVertex3f(-1.9, -2.0, 4);
	glVertex3f(-2.0, -2.0, -5.6);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4, 0.5, 0.7);
	glVertex3f(-2.0, 1.8, -5.6);
	glVertex3f(-1.9, 1.8, -5.6);
	glVertex3f(-1.4, 2.25, -5.6);
	glVertex3f(-1.5, 2.25, -5.6);

	glEnd();
	//Shirt

	glBegin(GL_QUADS);
	glColor3d(1.0, 0.5, 0.0);
	glVertex3f(-0.3, 0.5, -6);
	glVertex3f(0.3, 0.5, -6);
	glVertex3f(1.5, -0.2, -6);
	glVertex3f(-1.5, -0.2, -6);
	glEnd();



	glBegin(GL_QUADS);
	glColor3d(1.0, 0.5, 00);
	glVertex3f(-1.5, -0.2, -6);
	glVertex3f(1.5, -0.2, -6);
	glVertex3f(1.5, -2, -6);
	glVertex3f(-1.5, -2, -6);
	glEnd();



	glBegin(GL_QUADS);

	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(-1.0, -0.8, -5.8);
	glVertex3f(-0.9, -0.8, -5.8);
	glVertex3f(-0.9, -2, -5.8);
	glVertex3f(-1.0, -2, -5.8);
	glEnd();



	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(1.0, -0.8, -5.8);
	glVertex3f(0.9, -0.8, -5.8);
	glVertex3f(0.9, -2, -5.8);
	glVertex3f(1.0, -2, -5.8);
	glEnd();


	//Left lines

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 4, -10);
	glVertex3f(-3.8, 4, -10);
	glVertex3f(-3.8, 4.5, -10);
	glVertex3f(-11, 4.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 3, -10);
	glVertex3f(-3.8, 3, -10);
	glVertex3f(-3.8, 3.5, -10);
	glVertex3f(-11, 3.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 2, -10);
	glVertex3f(-3.8, 2, -10);
	glVertex3f(-3.8, 2.5, -10);
	glVertex3f(-11, 2.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 1, -10);
	glVertex3f(-3.8, 1, -10);
	glVertex3f(-3.8, 1.5, -10);
	glVertex3f(-11, 1.5, -10);
	glEnd();


	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 0, -10);
	glVertex3f(-3.8, 0, -10);
	glVertex3f(-3.8, 0.5, -10);
	glVertex3f(-11, 0.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -1, -10);
	glVertex3f(-3.8, -1, -10);
	glVertex3f(-3.8, -0.5, -10);
	glVertex3f(-11, -0.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -2, -10);
	glVertex3f(-3.8, -2, -10);
	glVertex3f(-3.8, -1.5, -10);
	glVertex3f(-11, -1.5, -10);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -3, -10);
	glVertex3f(-3.8, -3, -10);
	glVertex3f(-3.8, -2.5, -10);
	glVertex3f(-11, -2.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -4, -10);
	glVertex3f(-3.8, -4, -10);
	glVertex3f(-3.8, -3.5, -10);
	glVertex3f(-11, -3.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -5, -10);
	glVertex3f(-3.8, -5, -10);
	glVertex3f(-3.8, -4.5, -10);
	glVertex3f(-11, -4.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -6, -10);
	glVertex3f(-3.8, -6, -10);
	glVertex3f(-3.8, -5.5, -10);
	glVertex3f(-11, -5.5, -10);
	glEnd();
	//Right Lines
	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(4, -3, -10);
	glVertex3f(-3.5, -3, -10);
	glVertex3f(-3.5, -2.5, -10);
	glVertex3f(4, -2.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(4, -4, -10);
	glVertex3f(-3.5, -4, -10);
	glVertex3f(-3.5, -3.5, -10);
	glVertex3f(4, -3.5, -10);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(4, -5, -10);
	glVertex3f(-3.5, -5, -10);
	glVertex3f(-3.5, -4.5, -10);
	glVertex3f(4, -4.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(4, -6, -10);
	glVertex3f(-3.5, -6, -10);
	glVertex3f(-3.5, -5.5, -10);
	glVertex3f(4, -5.5, -10);
	glEnd();

	drawText1(5.6, 5, -4, 0, 0, 0, "New Homes Sales Rise 4.5%");
	//Photo2
	glBegin(GL_QUADS);
	glColor3d(0.8, 0.8, 1);
	glVertex3f(11, -2, -48);
	glVertex3f(6, -2, -48);
	glVertex3f(6, 4, -48);
	glVertex3f(11, 4, -48);
	glEnd();

	//House
	glBegin(GL_TRIANGLES);
	glColor3d(0.7, 0.4, 0);
	glVertex3f(10, 1.5, -10);
	glVertex3f(7, 1.5, -10);
	glVertex3f(8.5, 3.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(1, 0.9, 0.4);
	glVertex3f(10, -2, -10);
	glVertex3f(7, -2, -10);
	glVertex3f(7, 1.5, -10);
	glVertex3f(10, 1.5, -10);
	glEnd();


	//Down lines

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(11.5, -3, -10);
	glVertex3f(5.5, -3, -10);
	glVertex3f(5.5, -2.5, -10);
	glVertex3f(11.5, -2.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(11.5, -4, -10);
	glVertex3f(5.5, -4, -10);
	glVertex3f(5.5, -3.5, -10);
	glVertex3f(11.5, -3.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(11.5, -5, -10);
	glVertex3f(5.5, -5, -10);
	glVertex3f(5.5, -4.5, -10);
	glVertex3f(11.5, -4.5, -10);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(11.5, -6, -10);
	glVertex3f(5.5, -6, -10);
	glVertex3f(5.5, -5.5, -10);
	glVertex3f(11.5, -5.5, -10);
	glEnd();

	drawText1(x, y, z, r, g, b,
		str);

	glLoadIdentity();

	gluLookAt(0.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//hair

	glPushMatrix();
	glColor3d(0.9, 0.9, 0.9);
	glTranslatef(0.0, 0.995, 4);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();



	//face
	glPushMatrix();
	glColor3f(0.9, 0.7, 0.8);
	glTranslatef(0.0, 0.96, 4);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();

	//left ear

	glPushMatrix();
	glColor3f(0.9, 0.7, 0.8);
	glTranslatef(-0.8, 2.5, 5);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();

	//right ear

	glPushMatrix();

	glColor3f(0.9, 0.7, 0.8);

	glTranslatef(0.8, 3.0, -6);

	glutSolidSphere(0.1, 100, 100);

	glPopMatrix();



	//left eye

	glPushMatrix();

	glColor3f(0, 0, 0);
	glTranslatef(-0.05, 1.0, 4.2);
	glutSolidSphere(0.04, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.05, 1.045, 4.4);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.05, 1.09, 4.6);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();

	//right eye

	glPushMatrix();

	glColor3f(0, 0, 0);
	glTranslatef(0.05, 1.0, 4.2);
	glutSolidSphere(0.04, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.05, 1.045, 4.4);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.05, 1.09, 4.6);
	glutSolidSphere(0.015, 100, 100);
	glPopMatrix();

	//Moustache

	glPushMatrix();
	glColor3f(0.9, 0.9, 1);
	glTranslatef(0.0, 0.96, 4.4);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0.9, 0.9, 1);
	glTranslatef(-0.04, 0.96, 4.4);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0.9, 0.9, 1);
	glTranslatef(0.04, 0.96, 4.4);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();

	//lips

	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslatef(0.0, 1.045, 4.8);
	glutSolidSphere(0.015, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslatef(-0.01, 1.045, 4.8);
	glutSolidSphere(0.01, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslatef(0.01, 1.045, 4.8);
	glutSolidSphere(0.01, 100, 100);
	glPopMatrix();





	//Mike

	glPushMatrix();
	glColor3f(0.4, 0.4, 0.6);
	glTranslatef(-0.14, 1.05, 4);
	glutSolidSphere(0.036, 100, 100);
	glPopMatrix();

}


void drawScene() {

	glBegin(GL_QUADS);

	//Front
	glNormal3f(0.0f, 0.0f, 1.0f);
	//glNormal3f(-1.0f, 0.0f, 1.0f);

	glVertex3f(-0.5f, -1.0f, 0.5f);
	//glNormal3f(1.0f, 0.0f, 1.0f);

	glVertex3f(0.5f, -1.0f, 0.5f);
	//glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 1.0f, 0.5f);
	//glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 1.0f, 0.5f);

	//Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, -1.0f, -0.5f);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, 1.0f, -0.5f);
	//glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 1.0f, 0.5f);
	//glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, -1.0f, 0.5f);

	//Back
	glNormal3f(0.0f, 0.0f, -1.0f);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-0.5f, -1.0f, -0.5f);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-0.5f, 1.0f, -0.5f);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, 1.0f, -0.5f);
	//glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, -1.0f, -0.5f);

	//Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-0.5f, -1.0f, -0.5f);
	//glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -1.0f, 0.5f);
	//glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 1.0f, 0.5f);
	//glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-0.5f, 1.0f, -0.5f);

	glEnd();


}

void newspaper() {
	{



		glBegin(GL_QUADS);

		glColor3d(1, 1, 1);
		glVertex3f(-4, -3, -10);
		glVertex3f(4, -3, -10);
		glVertex3f(4, 3, -10);
		glVertex3f(-4, 3, -10);
		glEnd();


		//Main - Heading
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, 2.1, -10);
		glVertex3f(-3.8, 2.1, -10);
		glVertex3f(-3.8, 2.8, -10);
		glVertex3f(3.8, 2.8, -10);
		glEnd();

		//Sub-Heading1
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0, 1.65, -10);
		glVertex3f(-2.8, 1.65, -10);
		glVertex3f(-2.8, 2, -10);
		glVertex3f(0, 2, -10);
		glEnd();

		//Photo1
		glBegin(GL_QUADS);
		glColor3d(1, 1, 1);
		glVertex3f(-1, 0, -10);
		glVertex3f(-2.5, 0, -10);
		glVertex3f(-2.5, 1.5, -10);
		glVertex3f(-1, 1.5, -10);
		glEnd();

		//Human
		glBegin(GL_TRIANGLES);
		glColor3d(0, 0, 1);
		glVertex3f(-1, 0, -10);
		glVertex3f(-2.5, 0, -10);
		glVertex3f(-1.75, 1.0, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0.8, 0.6, 0.5);
		glVertex3f(-1.5, 0.8, -10);
		glVertex3f(-2, 0.8, -10);
		glVertex3f(-2, 1.2, -10);
		glVertex3f(-1.5, 1.2, -10);
		glEnd();

		//Left lines
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(-2.6, 0, -10);
		glVertex3f(-3.8, 0, -10);
		glVertex3f(-3.8, 0.25, -10);
		glVertex3f(-2.6, 0.25, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(-2.6, 0.35, -10);
		glVertex3f(-3.8, 0.35, -10);
		glVertex3f(-3.8, 0.6, -10);
		glVertex3f(-2.6, 0.6, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(-2.6, 0.7, -10);
		glVertex3f(-3.8, 0.7, -10);
		glVertex3f(-3.8, 1, -10);
		glVertex3f(-2.6, 1, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(-2.6, 1.1, -10);
		glVertex3f(-3.8, 1.1, -10);
		glVertex3f(-3.8, 1.35, -10);
		glVertex3f(-2.6, 1.35, -10);
		glEnd();
		//Right lines
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, 0, -10);
		glVertex3f(-0.9, 0, -10);
		glVertex3f(-0.9, 0.25, -10);
		glVertex3f(0.9, 0.25, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, 0.35, -10);
		glVertex3f(-0.9, 0.35, -10);
		glVertex3f(-0.9, 0.6, -10);
		glVertex3f(0.9, 0.6, -10);
		glEnd();


		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, 0.7, -10);
		glVertex3f(-0.9, 0.7, -10);
		glVertex3f(-0.9, 1, -10);
		glVertex3f(0.9, 1, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, 1.1, -10);
		glVertex3f(-0.9, 1.1, -10);
		glVertex3f(-0.9, 1.35, -10);
		glVertex3f(0.9, 1.35, -10);
		glEnd();

		//Down lines
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, -0.1, -10);
		glVertex3f(-3.8, -0.1, -10);
		glVertex3f(-3.8, -0.35, -10);
		glVertex3f(0.9, -0.35, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, -0.45, -10);
		glVertex3f(-3.8, -0.45, -10);
		glVertex3f(-3.8, -0.70, -10);
		glVertex3f(0.9, -0.70, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, -0.8, -10);
		glVertex3f(-3.8, -0.8, -10);
		glVertex3f(-3.8, -1.05, -10);
		glVertex3f(0.9, -1.05, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, -1.15, -10);
		glVertex3f(-3.8, -1.15, -10);
		glVertex3f(-3.8, -1.4, -10);
		glVertex3f(0.9, -1.4, -10);
		glEnd();
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(0.9, -1.5, -10);
		glVertex3f(-3.8, -1.5, -10);
		glVertex3f(-3.8, -1.75, -10);
		glVertex3f(0.9, -1.75, -10);
		glEnd();

		//Sub-Heading2
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, 1.65, -10);
		glVertex3f(1.4, 1.65, -10);
		glVertex3f(1.4, 2, -10);
		glVertex3f(3.8, 2, -10);
		glEnd();

		//Photo2
		glBegin(GL_QUADS);
		glColor3d(1, 0, 1);
		glVertex3f(2.5, 0, -10);
		glVertex3f(1.4, 0, -10);
		glVertex3f(1.4, 1.5, -10);
		glVertex3f(2.5, 1.5, -10);
		glEnd();

		//House
		glBegin(GL_TRIANGLES);
		glColor3d(0.7, 0.4, 0);
		glVertex3f(1.6, 1, -10);
		glVertex3f(2.3, 1, -10);
		glVertex3f(2, 1.5, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(1, 0.9, 0.4);
		glVertex3f(1.6, 0, -10);
		glVertex3f(2.3, 0, -10);
		glVertex3f(2.3, 1, -10);
		glVertex3f(1.6, 1, -10);
		glEnd();


		//Right lines
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, 0, -10);
		glVertex3f(2.7, 0, -10);
		glVertex3f(2.7, 0.25, -10);
		glVertex3f(3.8, 0.25, -10);
		glEnd();
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, 0.35, -10);
		glVertex3f(2.7, 0.35, -10);
		glVertex3f(2.7, 0.6, -10);
		glVertex3f(3.8, 0.6, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, 0.7, -10);
		glVertex3f(2.7, 0.7, -10);
		glVertex3f(2.7, 0.95, -10);
		glVertex3f(3.8, 0.95, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, 1.05, -10);
		glVertex3f(2.7, 1.05, -10);
		glVertex3f(2.7, 1.3, -10);
		glVertex3f(3.8, 1.3, -10);
		glEnd();

		//Down lines
		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, -0.1, -10);
		glVertex3f(1.4, -0.1, -10);
		glVertex3f(1.4, -0.35, -10);
		glVertex3f(3.8, -0.35, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, -0.45, -10);
		glVertex3f(1.4, -0.45, -10);
		glVertex3f(1.4, -0.7, -10);
		glVertex3f(3.8, -0.7, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, -0.8, -10);
		glVertex3f(1.4, -0.8, -10);
		glVertex3f(1.4, -1.05, -10);
		glVertex3f(3.8, -1.05, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, -1.15, -10);
		glVertex3f(1.4, -1.15, -10);
		glVertex3f(1.4, -1.4, -10);
		glVertex3f(3.8, -1.4, -10);
		glEnd();

		glBegin(GL_QUADS);
		glColor3d(0, 0, 0);
		glVertex3f(3.8, -1.5, -10);
		glVertex3f(1.4, -1.5, -10);
		glVertex3f(1.4, -1.75, -10);
		glVertex3f(3.8, -1.75, -10);
		glEnd();
		glPopMatrix();

		glutSwapBuffers();
	}
}

void raised() {

	glBegin(GL_POLYGON);
	glVertex3f(2., 1., 2.5);
	glVertex3f(0.8, 1., .5);
	glVertex3f(.2, 0.8, -.5);
	glVertex3f(-.4, 1, -1.2);
	glVertex3f(-1.4, 1, -1.6);

	glVertex3f(-0.9, 1.2, 2.5);
	glVertex3f(-1.6, 1.2, -1.6);
	glVertex3f(-0.6, 1.2, .5);
	glVertex3f(-.57, .9, .5);
	glVertex3f(-0.72, 1., .5);
	glVertex3f(-1.2, 1.2, 2.4);
	glEnd();

}

GLdouble y = 1.3;
GLdouble x = 0.8;
GLdouble z = 4.0;
GLdouble k = 1.0;
GLdouble e = 0.25;

GLdouble c_y = 1.5;
GLdouble c_x = 0.5;
GLdouble c_z = 4.;

GLdouble n_y = 1.5;
GLdouble n_x = 0.5;
GLdouble n_z = 4.;
GLint r_z = 145;
bool cvis = false;

void delay() {
	for (GLdouble i = 0; i < 5000000; i++);
}
void idle() {
	delay();
	if (k > 0.0)
		k -= 0.01;
	if (k <= 0.05 && c_y > -1.6 && n_x > -1.) {
		c_y -= 0.01;
		n_y -= 0.01;
		if (c_y < -.9) {
			c_z -= 0.001;
			n_y += 0.03;
			n_x -= 0.005;
			r_z = (r_z + 30) % 360;
			printf("%f %f %f \n", n_x, n_y, n_z);
		}
		else {
			c_z -= 0.01;
			n_z -= 0.01;
		}
	}
	if (n_x <= 0.146 && n_y >= 0.52 && n_z >= 1.61 && y >= 0.5) {
		y -= 0.01;
		//x -= 0.1;
		//z += 0.1;

	}

	glutPostRedisplay();
}

GLdouble s2 = 0.0;
GLdouble zx = 0.1;
GLdouble zy = 0.1;
GLdouble zz = 0.1;
void idle2() {
	delay();
	printf("called 2 %f %f\n", y, s2);
	s2 += 0.01;
	if (s2 > 0.5) {
		zx += 0.01;
		zy += 0.01;
		zz += 0.01;
	}
	glutPostRedisplay();
}

GLdouble s3 = 0.0;
void idle3() {
	delay();
	delay();
	printf("called 3 %f %f %f\n", y, s2, s3);
	s3 += 0.01;
	glutPostRedisplay();
}

//void draw2D(const char* str) {
//	glDisable(GL_DEPTH_TEST);
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	gluOrtho2D(0.0, 1.0, 1.0, 0.0);
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	glScalef(20, 20, 20);
//	drawPolygon(str);
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//
//	glEnable(GL_DEPTH_TEST);
//
//	glutSwapBuffers();
//}

void frame1() {
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);


	glPushMatrix();
	glTranslatef(-10.0, 1.4, .10);
	glRotatef(90, 0.0f, .0f, 1.0f);
	glColor3f(0.9, 0.9, 0.9);
	wall(0.0001);
	glPopMatrix();


	//glPushMatrix();

	//glRotatef(90, 0.0f, .0f, 1.0f);
	//glColor3f(0.99, 0.99, 0.99);
	//raised();
	//glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 1.4, .10);
	glRotatef(90, 0.0f, .0f, 1.0f);
	glRotatef(120, 0.0f, 1.0f, .0f);
	glColor3f(0.91, 0.85, 0.7);
	drawScene();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 1.0, -0.5);
	glRotatef(90, 0.0f, .0f, 1.0f);
	glRotatef(120, 0.0f, 1.0f, .0f);
	glScaled(1, 1, 1);
	glColor3f(0.65, 0.6, 0.65);
	drawScene();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.2, 0.8, -1.15);
	glRotatef(90, 0.0f, .0f, 1.0f);
	glRotatef(125, 0.0f, 1.0f, .0f);
	glScalef(3.0, 0.7, 1);
	glColor3f(0.91, 0.85, 0.7);
	drawScene();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.2, -0.5, -2.9);
	glRotatef(90, 0.0f, .0f, 1.0f);
	glRotatef(125, 0.0f, 1.0f, .0f);
	glScalef(1.0, 0.7, 1);
	glColor3f(0.28, 0.7, 0.8);
	drawScene();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.2, -1.6, -4.5);
	glRotatef(90, 0.0f, .0f, 1.0f);
	glRotatef(145, 0.0f, 1.0f, .0f);
	glScalef(1.0, 0.7, 1);
	glColor3f(0.91, 0.85, 0.7);
	drawScene();
	glPopMatrix();

	if (k < 0) {
		glPushMatrix();
		glTranslatef(0.5, c_y, c_z);
		glRotatef(90, 0.0f, .0f, 1.0f);
		glRotatef(-230, 0.0f, 1.0f, .0f);
		glRotatef(-180, 1.0f, 0.0f, .0f);
		glScalef(0.2, 0.2, 0.2);
		glColor3f(0.91, 0.85, 0.7);
		cycle();
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(n_x, n_y, n_z);
	glRotatef(90, 0.0f, .0f, 1.0f);
	glRotatef(180, 0.0f, 1.0f, 0.0f);
	glRotatef(r_z, 0.0f, .0f, 1.0f);
	glScalef(0.02, 0.014, 0.02);

	newspaper();
	glPopMatrix();
}


void frame3() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wall

	glBegin(GL_POLYGON);
	glColor3d(0.9, 0.8, 0.6);
	glVertex3f(40, -6, -10);
	glVertex3f(40, 300, -10);
	glVertex3f(-40, -6, -10);
	glVertex3f(-40, 300, -10);

	glEnd();



	glBegin(GL_POLYGON);

	glColor3d(0.9, 1, 1);

	glVertex3f(11, 8, -9.5);

	glVertex3f(11, -3, -9.5);

	glVertex3f(-11, -3, -9.5);

	glVertex3f(-11, 8, -9.5);

	glEnd();

	//floor

	glBegin(GL_POLYGON);

	glColor3d(0.8, 0.6, 0.1);

	glVertex3f(40, -6, -10);

	glVertex3f(-40, -20, -10);

	glVertex3f(-40, -6, -10);

	glVertex3f(40, -20, -10);

	glEnd();
	drawText1(-8, -8, -3.5, 1, 1, 1, "GET THE LATEST NEW ABOUT NATIONAL AND INTERNATIONAL EVENTS");



	//Table

	glBegin(GL_QUADS);

	glColor3d(0.7, 0.5, 0.1);

	glVertex3f(-2.5, 1.3, -9);

	glVertex3f(2.5, 1.3, -9);

	glVertex3f(2.5, 0.8, -9);

	glVertex3f(-2.5, 0.8, -9);

	glEnd();



	glBegin(GL_QUADS);

	glColor3d(0.7, 0.5, 0.2);

	glVertex3f(-2.2, 1.0, -9);

	glVertex3f(2.2, 1.0, -9);

	glVertex3f(2.2, -6.5, -9);

	glVertex3f(-2.2, -6.5, -9);

	glEnd();

	//drawer1

	glBegin(GL_QUADS);

	glColor3d(0.4, 0.3, 0.2);

	glVertex3f(-2.0, 0.7, -7.5);

	glVertex3f(2.0, 0.7, -7.5);

	glVertex3f(2.0, -5.5, -7.5);

	glVertex3f(-2.0, -5.5, -7.5);

	glEnd();



	//Shirt

	glBegin(GL_QUADS);

	glColor3d(1.0, 0.5, 0.0);

	glVertex3f(-0.3, 2.5, -6);

	glVertex3f(0.3, 2.5, -6);

	glVertex3f(1.5, 1.8, -6);

	glVertex3f(-1.5, 1.8, -6);

	glEnd();



	glBegin(GL_QUADS);

	glColor3d(1.0, 0.5, 0.0);

	glVertex3f(-1.5, 1.8, -6);

	glVertex3f(1.5, 1.8, -6);

	glVertex3f(1.5, 1.25, -6);

	glVertex3f(-1.5, 1.25, -6);

	glEnd();



	glBegin(GL_QUADS);

	glColor3d(0.0, 0.0, 0.0);

	glVertex3f(-1.0, 1.8, -5.8);

	glVertex3f(-0.9, 1.8, -5.8);

	glVertex3f(-0.9, 1.25, -5.8);

	glVertex3f(-1.0, 1.25, -5.8);

	glEnd();



	glBegin(GL_QUADS);

	glColor3d(0.0, 0.0, 0.0);

	glVertex3f(1.0, 1.8, -5.8);

	glVertex3f(0.9, 1.8, -5.8);

	glVertex3f(0.9, 1.25, -5.8);

	glVertex3f(1.0, 1.25, -5.8);

	glEnd();



	//specs line

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(0.2, 3.4, -3.5);
	glVertex3f(-0.1, 3.4, -3.5);
	glVertex3f(-0.1, 3.5, -3.5);
	glVertex3f(0.2, 3.5, -3.5);
	glEnd();



	//Mike

	glBegin(GL_QUADS);

	glColor3f(0.4, 0.4, 0.6);

	glVertex3f(-2.0, 1.8, -5.6);

	glVertex3f(-1.9, 1.8, -5.6);

	glVertex3f(-1.9, 1.25, -5.6);

	glVertex3f(-2.0, 1.25, -5.6);

	glEnd();



	glBegin(GL_QUADS);

	glColor3f(0.4, 0.5, 0.7);

	glVertex3f(-2.0, 1.8, -5.6);

	glVertex3f(-1.9, 1.8, -5.6);

	glVertex3f(-1.4, 2.25, -5.6);

	glVertex3f(-1.5, 2.25, -5.6);

	glEnd();



	glLoadIdentity();

	gluLookAt(0.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// This has to be light not material





	//hair

	glPushMatrix();
	glColor3d(0.8, 0.8, 0.8);
	glTranslatef(0.0, 1.2, 4);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();



	//face
	glPushMatrix();
	glColor3f(0.9, 0.7, 0.8);
	glTranslatef(0.0, 1.15, 4);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();

	//left ear

	glPushMatrix();
	glColor3f(0.9, 0.7, 0.8);
	glTranslatef(-0.8, 3.0, -6);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();

	//right ear

	glPushMatrix();

	glColor3f(0.9, 0.7, 0.8);

	glTranslatef(0.8, 3.0, -6);

	glutSolidSphere(0.1, 100, 100);

	glPopMatrix();



	//left eye

	glPushMatrix();

	glColor3f(0, 0, 0);
	glTranslatef(-0.05, 1.2, 4.2);
	glutSolidSphere(0.04, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0.8, 0.8, .8);
	glTranslatef(-0.05, 1.24, 4.4);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.05, 1.28, 4.6);
	glutSolidSphere(0.015, 100, 100);
	glPopMatrix();

	//right eye

	glPushMatrix();

	glColor3f(0, 0, 0);
	glTranslatef(0.05, 1.2, 4.2);
	glutSolidSphere(0.04, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.05, 1.24, 4.4);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.05, 1.28, 4.6);
	glutSolidSphere(0.015, 100, 100);
	glPopMatrix();

	//Moustache

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0.0, 1.15, 4.4);
	glutSolidSphere(0.04, 100, 100);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(-0.04, 1.15, 4.4);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0.04, 1.15, 4.4);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();

	//lips

	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslatef(0.0, 1.225, 4.8);
	glutSolidSphere(0.01, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslatef(-0.01, 1.225, 4.8);
	glutSolidSphere(0.01, 100, 100);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslatef(0.01, 1.225, 4.8);
	glutSolidSphere(0.01, 100, 100);
	glPopMatrix();



	//Mike

	glPushMatrix();
	glColor3f(0.4, 0.4, 0.6);
	glTranslatef(-0.14, 1.05, 4);
	glutSolidSphere(0.036, 100, 100);
	glPopMatrix();

	glFlush();



	glutSwapBuffers();


}


void frame4() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//left Wall

	glBegin(GL_POLYGON);

	glColor3d(0, 1, 0.7);

	glVertex3f(-4, 0, -10);

	glVertex3f(-4, 300, -10);

	glVertex3f(-40, 0, -10);

	glVertex3f(-40, -70, -10);

	glEnd();



	//Right Wall

	glBegin(GL_POLYGON);

	glColor3d(0, 1, 1);

	glVertex3f(-4, 0, -10);

	glVertex3f(-4, 300, -10);

	glVertex3f(40, 30, -10);

	glVertex3f(40, 0, -10);

	glEnd();

	//Floor

	glBegin(GL_POLYGON);

	glColor3d(0, 0.6, 0.8);

	glVertex3f(-4, 0, -10);

	glVertex3f(40, 0, -10);

	glVertex3f(40, -30, -10);

	glVertex3f(-40, -70, -10);

	glEnd();

	drawText1(-8, -8, -3.5, 1, 1, 1, "WORLD NEWS IN THE COMFORT OF YOUR HOUSE!");

	//Table

	glBegin(GL_QUADS);

	glColor3d(0.4, 0.3, 0.2);

	glVertex3f(-3, -1, -9);

	glVertex3f(5, -1, -9);

	glVertex3f(5, -2, -9);

	glVertex3f(-3, -2, -9);

	glEnd();



	glBegin(GL_QUADS);

	glColor3d(0.6, 0.3, 0.0);



	glVertex3f(-2.8, -2, -9);

	glVertex3f(4.8, -2, -9);

	glVertex3f(4.8, -7, -9);

	glVertex3f(-2.8, -7, -9);

	glEnd();

	//drawer1

	glBegin(GL_QUADS);

	glColor3d(0.4, 0.3, 0.2);

	glVertex3f(-2.3, -2.5, -7.5);

	glVertex3f(4.3, -2.5, -7.5);

	glVertex3f(4.3, -4.2, -7.5);

	glVertex3f(-2.3, -4.2, -7.5);

	glEnd();

	//drawer2

	glBegin(GL_QUADS);

	glColor3d(0.4, 0.3, 0.2);

	glVertex3f(-2.3, -4.5, -7.5);

	glVertex3f(4.3, -4.5, -7.5);

	glVertex3f(4.3, -6.5, -7.5);

	glVertex3f(-2.3, -6.5, -7.5);

	glEnd();



	//newspaper

	glBegin(GL_QUADS);

	glColor3d(1, 1, 1);

	glVertex3f(0, -1, -4);

	glVertex3f(1, -1, -4);

	glVertex3f(1, 2.9, -4);

	glVertex3f(0, 2.3, -4);

	glEnd();




	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glVertex3f(1, -1, -4);
	glVertex3f(2, -1, -4);
	glVertex3f(2, 2.3, -4);
	glVertex3f(1, 2.3, -4);
	glEnd();


	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(0.2, 1.7, -3.5);
	glVertex3f(1.8, 1.7, -3.5);
	glVertex3f(1.8, 2.0, -3.5);
	glVertex3f(0.2, 2.0, -3.5);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(0.2, 1.1, -3.5);
	glVertex3f(1.8, 1.1, -3.5);
	glVertex3f(1.8, 1.4, -3.5);
	glVertex3f(0.2, 1.4, -3.5);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(0.2, 0.5, -3.5);
	glVertex3f(1.8, 0.5, -3.5);
	glVertex3f(1.8, 0.8, -3.5);
	glVertex3f(0.2, 0.8, -3.5);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(0.2, -0.1, -3.5);
	glVertex3f(1.8, -0.1, -3.5);
	glVertex3f(1.8, 0.2, -3.5);
	glVertex3f(0.2, 0.2, -3.5);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(0.2, -0.7, -3.5);
	glVertex3f(1.8, -0.7, -3.5);
	glVertex3f(1.8, -0.4, -3.5);
	glVertex3f(0.2, -0.4, -3.5);
	glEnd();


	glLoadIdentity();

	gluLookAt(0.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// This has to be light not material





	//hair

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.1, 1.28, 4.4);

	glutSolidSphere(0.06, 100, 100);

	glPopMatrix();



	glPushMatrix();

	glColor3f(0.0, 0.0, 0.0);

	glTranslatef(0.03, 1.28, 4.4);

	glutSolidSphere(0.06, 100, 100);

	glPopMatrix();



	glPushMatrix();

	glColor3f(0.0, 0.0, 0.0);

	glTranslatef(0.17, 1.28, 4.4);

	glutSolidSphere(0.06, 100, 100);

	glPopMatrix();



	glPushMatrix();

	glColor3f(0, 0, 0);

	glTranslatef(0.15, 1.33, 4.4);

	glutSolidSphere(0.04, 100, 100);

	glPopMatrix();





	glPushMatrix();

	glColor3f(0, 0, 0);

	glTranslatef(0.05, 1.33, 4.4);

	glutSolidSphere(0.04, 100, 100);

	glPopMatrix();

	//face

	glPushMatrix();

	glColor3f(0.8, 0.6, 0.5);

	glTranslatef(0.1, 1.1,
		4);

	glutSolidSphere(0.1, 100, 100);

	glPopMatrix();

	//left ear

	glPushMatrix();

	glColor3f(0.8, 0.6, 0.5);

	glTranslatef(-0.01, 1.1, 4);

	glutSolidSphere(0.01, 100, 100);

	glPopMatrix();

	//right ear

	glPushMatrix();

	glColor3f(0.8, 0.6, 0.5);


	glTranslatef(0.21, 1.1, 4);

	glutSolidSphere(0.01, 100, 100);

	glPopMatrix();



	//left eye

	glPushMatrix();

	glColor3f(0, 0, 0);

	glTranslatef(0.05, 1.2, 4.4);

	glutSolidSphere(0.02, 100, 100);

	glPopMatrix();



	//right eye

	glPushMatrix();

	glColor3f(0, 0, 0);

	glTranslatef(0.15, 1.2, 4.4);

	glutSolidSphere(0.02, 100, 100);

	glPopMatrix();



	//left hand

	glPushMatrix();

	glColor3f(0.8, 0.6, 0.5);

	glTranslatef(0.0, 1.05, 4.8);

	glutSolidSphere(0.02, 10, 10);

	glPopMatrix();



	glPushMatrix();

	glColor3f(0.8, 0.6, 0.5);

	glTranslatef(0.0, 1.03, 4.8);

	glutSolidSphere(0.02, 10, 10);

	glPopMatrix();

	//right hand

	glPushMatrix();

	glColor3f(0.8, 0.6, 0.5);

	glTranslatef(0.2, 1.05, 4.8);

	glutSolidSphere(0.02, 10, 10);

	glPopMatrix();


	glPushMatrix();

	glColor3f(0.8, 0.6, 0.5);

	glTranslatef(0.2, 1.03, 4.8);

	glutSolidSphere(0.02, 10, 10);

	glPopMatrix();


	glFlush();

	glutSwapBuffers();

}



void frame3Disp() {
	glPushMatrix();

	glScalef(0.1, 0.1, 0.1);
	frame3();
	glPopMatrix();
}

void frame4Disp() {
	glPushMatrix();
	glScalef(0.1, 0.1, 0.1);

	frame4();
	glPopMatrix();
}

void newsframe() {



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//Newspaper


	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glVertex3f(-12, -8, -50);
	glVertex3f(12, -8, -50);
	glVertex3f(12, 8, -50);
	glVertex3f(-12, 8, -50);
	glEnd();



	//Main - Heading

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 6, -4);
	glVertex3f(11, 6, -4);
	glVertex3f(11, 8, -4);
	glVertex3f(-11, 8, -4);
	glEnd();

	//Sub-heading 1

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-10, 4, -4);
	glVertex3f(1, 4, -4);
	glVertex3f(1, 5, -4);
	glVertex3f(-10, 5, -4);
	glEnd();

	//Photo1
	glBegin(GL_QUADS);
	glColor3d(0.8, 0.8, 0);
	glVertex3f(-8, 0.0, -4);
	glVertex3f(-2, 0.0, -4);
	glVertex3f(-2, 3, -4);
	glVertex3f(-8, 3, -4);
	glEnd();

	//Left Lines


	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 3, -4);
	glVertex3f(-8.2, 3, -4);
	glVertex3f(-8.2, 3.4, -4);
	glVertex3f(-11, 3.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 2, -4);
	glVertex3f(-8.2, 2, -4);
	glVertex3f(-8.2, 2.4, -4);
	glVertex3f(-11, 2.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 1, -4);
	glVertex3f(-8.2, 1, -4);
	glVertex3f(-8.2, 1.4, -4);
	glVertex3f(-11, 1.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, 0, -4);
	glVertex3f(-8.2, 0, -4);
	glVertex3f(-8.2, 0.4, -4);
	glVertex3f(-11, 0.4, -4);
	glEnd();

	//Right Lines


	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-1.8, 3, -4);
	glVertex3f(2, 3, -4);
	glVertex3f(2, 3.4, -4);
	glVertex3f(-1.8, 3.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-1.8, 2, -4);
	glVertex3f(2, 2, -4);
	glVertex3f(2, 2.4, -4);
	glVertex3f(-1.8, 2.4, -4);
	glEnd();
	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-1.8, 1, -4);
	glVertex3f(2, 1, -4);
	glVertex3f(2, 1.4, -4);
	glVertex3f(-1.8, 1.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-1.8, 0, -4);
	glVertex3f(2, 0, -4);
	glVertex3f(2, 0.4, -4);
	glVertex3f(-1.8, 0.4, -4);
	glEnd();

	//Article lines
	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -1, -4);
	glVertex3f(2, -1, -4);
	glVertex3f(2, -0.6, -4);
	glVertex3f(-11, -0.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -2, -4);
	glVertex3f(2, -2, -4);
	glVertex3f(2, -1.6, -4);
	glVertex3f(-11, -1.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -3, -4);
	glVertex3f(2, -3, -4);
	glVertex3f(2, -2.6, -4);
	glVertex3f(-11, -2.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -4, -4);
	glVertex3f(2, -4, -4);
	glVertex3f(2, -3.6, -4);
	glVertex3f(-11, -3.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -5, -4);
	glVertex3f(2, -5, -4);
	glVertex3f(2, -4.6, -4);
	glVertex3f(-11, -4.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -6, -4);
	glVertex3f(2, -6, -4);
	glVertex3f(2, -5.6, -4);
	glVertex3f(-11, -5.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(-11, -7, -4);
	glVertex3f(2, -7, -4);
	glVertex3f(2, -6.6, -4);
	glVertex3f(-11, -6.6, -4);
	glEnd();

	//Sub-heading 2

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(4, 4, -4);
	glVertex3f(10, 4, -4);
	glVertex3f(10, 5, -4);
	glVertex3f(4, 5, -4);
	glEnd();

	//Photo2
	glBegin(GL_QUADS);
	glColor3d(0.8, 0.8, 0);
	glVertex3f(3, 0.0, -4);
	glVertex3f(7, 0.0, -4);
	glVertex3f(7, 3, -4);
	glVertex3f(3, 3, -4);
	glEnd();


	//Right Lines


	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(7.2, 3, -4);
	glVertex3f(11, 3, -4);
	glVertex3f(11, 3.4, -4);
	glVertex3f(7.2, 3.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(7.2, 2, -4);
	glVertex3f(11, 2, -4);
	glVertex3f(11, 2.4, -4);
	glVertex3f(7.2, 2.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(7.2, 1, -4);
	glVertex3f(11, 1, -4);
	glVertex3f(11, 1.4, -4);
	glVertex3f(7.2, 1.4, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(7.2, 0, -4);
	glVertex3f(11, 0, -4);
	glVertex3f(11, 0.4, -4);
	glVertex3f(7.2, 0.4, -4);
	glEnd();

	//Article lines
	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(3, -1, -4);
	glVertex3f(11, -1, -4);
	glVertex3f(11, -0.6, -4);
	glVertex3f(3, -0.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(3, -2, -4);
	glVertex3f(11, -2, -4);
	glVertex3f(11, -1.6, -4);
	glVertex3f(3, -1.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(3, -3, -4);
	glVertex3f(11, -3, -4);
	glVertex3f(11, -2.6, -4);
	glVertex3f(3, -2.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(3, -4, -4);
	glVertex3f(11, -4, -4);
	glVertex3f(11, -3.6, -4);
	glVertex3f(3, -3.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(3, -5, -4);
	glVertex3f(11, -5, -4);
	glVertex3f(11, -4.6, -4);
	glVertex3f(3, -4.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(3, -6, -4);
	glVertex3f(11, -6, -4);
	glVertex3f(11, -5.6, -4);
	glVertex3f(3, -5.6, -4);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3f(3, -7, -4);
	glVertex3f(11, -7, -4);
	glVertex3f(11, -6.6, -4);
	glVertex3f(3, -6.6, -4);
	glEnd();




}



void displaysolid(void)
{
	GLfloat mat_ambient[] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat mat_diffuse[] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat mat_shininess[] = { 50.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	GLfloat lightint[] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat lightpos[] = { 2.0f,6.0f,18.0f,0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightint);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double winht = 1.0;
	glOrtho(-winht * 64 / 48.0, winht * 64 / 48.0, -winht, winht, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, y, z, k, e, 1.0, 1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (y > 0.5) {
		frame1();
		glDisable(GL_LIGHTING);
		drawText1(-1., .2, 0.1, 1, 1, 1, "NEWS AT YOUR DOORSTEP EARLY IN THE MORNING");
		glEnable(GL_LIGHTING);
		//drawPolygon("NewsPaper  ngfjhfkhljfdklksadtyukjfghdffgjkjkhjgfghgjhkjgfgfghjhkhg");
	}

	if (y < 0.5) {

		glutIdleFunc(idle2);
		glFlush();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_LIGHTING);
		glLoadIdentity();

		glPushMatrix();

		glScalef(zx, zy, 0.1);

		drawPolygon(-2, 6, -4, 0, 0, 0, "GLOBAL TIMES");


		drawPolygon(-4, 4.5, -4, 0, 0, 0, "Modi at mega rally in Varansi for Elections 2019");
		glPopMatrix();
		//
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//for (int j = 0; j < 10; j++)
		//	delay();
		//glLoadIdentity();
		//glPushMatrix();
		//frame3();
		//glPopMatrix();
	}

	if (s2 >= 1) {

		glutIdleFunc(idle3);
		y = 0.5;
		glFlush();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		frame3Disp();


	}
	if (s3 >= 1) {
		y = 0.5;
		s2 = 0.1;
		glutIdleFunc(NULL);
		glFlush();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		frame4Disp();
	}

	glFlush();
}

void main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Newspaper");
	glutDisplayFunc(displaysolid);
	glutIdleFunc(idle);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.8, 0.92, 0.91, 0.0);
	glViewport(0, 0, 640, 480);
	glutMainLoop();
}
