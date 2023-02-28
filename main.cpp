#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

int winWidth = 900; // the window width
int winHeight = 600; // the window height
int time = 50; // Declare refresh interval in ms
float viewX = 100;
float viewY = 400;
float viewZ = 1200;
double pi = 3.1415926;

double cuboid[8][3]; // Set a matrix to record cuboid's vertexes

static GLuint boardwalk; // Initialize the boardwalk texture object
static GLuint brick; // Initialize the brick texture object
static GLuint snowfield; // Initialize the snowfield texture object
static GLuint door; // Initialize the door texture object
static GLint imageWidth;
static GLint imageHeight;
static GLint pixelLength;
static GLubyte* pixelData;

GLfloat rotateSpeed = 0.0; // Set the speed of rotate
GLfloat rotateAngle = 0.0; // Set the angle of rotate
GLfloat moonAngle = 0.0; // Set the parameter of sun's moving
GLfloat angleX = 0.0;
GLfloat angleY = 0.0;
GLfloat oldX;
GLfloat oldY;

// setvertexs() and constructCuboid() aim to draw a cuboid when giving start point and lenth
// Bind a cuboid's eight vertexes
void setvertexs(double x1, double y1, double z1, double x2, double y2, double z2) {
	cuboid[0][0] = x1;  // first vertex
	cuboid[0][1] = y1;
	cuboid[0][2] = z1;
	cuboid[1][0] = x1; //second vertex
	cuboid[1][1] = y1;
	cuboid[1][2] = z1 + z2;
	cuboid[2][0] = x1 + x2; // third vertex
	cuboid[2][1] = y1;
	cuboid[2][2] = z1 + z2;
	cuboid[3][0] = x1 + x2; //fourth vertex
	cuboid[3][1] = y1;
	cuboid[3][2] = z1;
	// bind the other four vertexes
	for (int m = 0; m < 4; m++) {
		for (int n = 0; n < 3; n++) {
			if (n == 1) {
				cuboid[4 + m][n] = cuboid[m][n] + y2;
			}
			else {
				cuboid[4 + m][n] = cuboid[m][n];
			}
		}
	}
}

// Build cuboid
void constructCuboid() {
	// Draw a cuboid's bottom
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(cuboid[0][0], cuboid[0][1], cuboid[0][2]);
	glVertex3f(cuboid[1][0], cuboid[1][1], cuboid[1][2]);
	glVertex3f(cuboid[2][0], cuboid[2][1], cuboid[2][2]);
	glVertex3f(cuboid[3][0], cuboid[3][1], cuboid[3][2]);
	glEnd();

	// Draw a cuboid's top
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(cuboid[4][0], cuboid[4][1], cuboid[4][2]);
	glVertex3f(cuboid[5][0], cuboid[5][1], cuboid[5][2]);
	glVertex3f(cuboid[6][0], cuboid[6][1], cuboid[6][2]);
	glVertex3f(cuboid[7][0], cuboid[7][1], cuboid[7][2]);
	glEnd();

	// Draw a cuboid' left side
	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(cuboid[0][0], cuboid[0][1], cuboid[0][2]);
	glVertex3f(cuboid[1][0], cuboid[1][1], cuboid[1][2]);
	glVertex3f(cuboid[5][0], cuboid[5][1], cuboid[5][2]);
	glVertex3f(cuboid[4][0], cuboid[4][1], cuboid[4][2]);
	glEnd();

	// Draw a cuboid's right side
	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(cuboid[2][0], cuboid[2][1], cuboid[2][2]);
	glVertex3f(cuboid[3][0], cuboid[3][1], cuboid[3][2]);
	glVertex3f(cuboid[7][0], cuboid[7][1], cuboid[7][2]);
	glVertex3f(cuboid[6][0], cuboid[6][1], cuboid[6][2]);
	glEnd();

	// Draw a cublid's front side
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(cuboid[5][0], cuboid[5][1], cuboid[5][2]);
	glVertex3f(cuboid[6][0], cuboid[6][1], cuboid[6][2]);
	glVertex3f(cuboid[2][0], cuboid[2][1], cuboid[2][2]);
	glVertex3f(cuboid[1][0], cuboid[1][1], cuboid[1][2]);
	glEnd();

	// Draw a cublid's back side
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(cuboid[0][0], cuboid[0][1], cuboid[0][2]);
	glVertex3f(cuboid[3][0], cuboid[3][1], cuboid[3][2]);
	glVertex3f(cuboid[7][0], cuboid[7][1], cuboid[7][2]);
	glVertex3f(cuboid[4][0], cuboid[4][1], cuboid[4][2]);
	glEnd();
}

// Draw and decorrate the foundation
void ground() {
	// Draw a ground
	glPushMatrix();
	glScalef(600.0, 8.0, 900.0);
	glColor3f(1.0,1.0,1.0);
	glutSolidCube(1);
	glPopMatrix();

	// Draw a road
	glPushMatrix();
	glTranslatef(0, 0, 525);
	glScalef(600.0, 8.0, 150.0);
	glColor3f(0.5,0.5,0.5);
	glutSolidCube(1);
	glPopMatrix();
}

// Draw a house and its decorations
void house() {
	// Draw house's walls
	// Draw the left wall
	glPushMatrix();
	glColor3ub(178,34,34);
	setvertexs(-100, 0, -150, 5, 150, 300);
	constructCuboid();
	// Draw the back wall
	setvertexs(100, 0, -150, -200, 150, 5);
	constructCuboid();
	// Draw side which have window ;
	setvertexs(100, 0, -150, -5, 150, 100);
	constructCuboid();
	setvertexs(100, 0, 50, -5, 150, 100);
	constructCuboid();
	setvertexs(100, 0, -50, -5, 35, 100);
	constructCuboid();
	setvertexs(100, 115, -50, -5, 35, 100);
	constructCuboid();
	// Draw side which have door
	setvertexs(-100, 0, 150, 75, 150, -5);
	constructCuboid();
	setvertexs(25, 0, 150, 75, 150, -5);
	constructCuboid();
	setvertexs(-25, 100, 150, 50, 50, -5);
	constructCuboid();
	glPopMatrix();

	// Draw house's roof
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
		glTranslatef(0, 150 + 7 * i, 5);
		glScalef(225.0 - 25 * i, 5.0, 350.0 - 25 * i);
		glColor3ub(248, 248, 255);
		glutSolidCube(1);
		glPopMatrix();
	}

	// Draw a door
	glPushMatrix();
	glColor3f(0.5,0.5,0.5);
	setvertexs(-25, 0, 145, 50, 80, 5);
	constructCuboid();
	glPopMatrix();

	// Draw a eaves
	glPushMatrix();
	glColor3ub(238, 197, 145);
	setvertexs(-25, 80, 145, 50, 20, 20);
	constructCuboid();
	glPopMatrix();

	// Draw some windows
	glPushMatrix();
	glColor4f(0.0,0.0,1.0, 0.6);
	setvertexs(100, 35, -50, 5, 80, 100);
	constructCuboid();
	glPopMatrix();

	// Draw house fence
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			glPushMatrix();
			glTranslatef(200 - 400*i, 40 + 40*j, -200);
			glColor3ub(205, 133, 63);
			glutSolidCylinder(5, 400, 20, 20);
			glPopMatrix();
		}
	}
	for (int i = 0; i < 2; i++) {
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(200, 40+40*i, -200);
		glutSolidCylinder(5, 400, 20, 20);
		glPopMatrix();
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
		    glTranslatef(-200 + 400 * i, -160 + 90 * j, 0);
			glutSolidCylinder(5, 90, 20, 20);
			glPopMatrix();
		}
	}
	for (int i = 1; i < 5; i++) {
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glTranslatef(-200 + 80 * i, 200, 0);
		glutSolidCylinder(5, 90, 20, 20);
		glPopMatrix();
	}
}

// Draw some trees
void trees() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			glPushMatrix();
			glColor3ub(200,100,30);
			glTranslatef(-250+i*500, 6, 200 - j * 120);
			setvertexs(0, 0, 0, 20, 100, 20);
			constructCuboid();
			glColor3f(0.0,0.2,0.2);
			glTranslatef(0, 90, 10);
			glutSolidSphere(30, 20, 20);
			glTranslatef(20, 0, 0);
			glutSolidSphere(30, 20, 20);
			glTranslatef(-10, 0, -10);
			glutSolidSphere(30, 20, 20);
			glTranslatef(0, 0, 20);
			glutSolidSphere(30, 20, 20);
			glTranslatef(0, 10, -10);
			glutSolidSphere(30, 20, 20);
			glPopMatrix();
		}
	}
}

// Draw a sun which moves as time goes by
void moon() {
	glPushMatrix();
	glColor3ub(176,196,122);
	glTranslatef(-150, 0, 0);
	glTranslatef(-500 * cos(pi * moonAngle / 180), 400 * sin(pi * moonAngle / 180), 0);
	glutSolidSphere(40, 20, 20);
	glPopMatrix();
}

void snowman() {
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glTranslatef(-150, 35, 200);
	glutSolidSphere(30, 15, 15);
	glTranslatef(0, 40, 0);
	glutSolidSphere(20, 10, 10);
	glColor3ub(0,0,0);
	glTranslatef(-5, 10, 20);
	glutSolidSphere(3, 10, 10);
	glTranslatef(15, 0, 0);
	glutSolidSphere(3, 10, 10);
	glColor3ub(210, 105, 30);
	glTranslatef(-10, -10, 0);
	glutSolidCone(3,20,10,10);
	glPopMatrix();
}

// Read image and bind texture
GLuint setTexture(const char* fileName) {
	GLuint lastTextureID = 0, textureID = 0;
	GLubyte* pixeldata = 0;
	// Read in and open an image file
	FILE* file = NULL; // The image file should be placed in the same folder with the source code.
	fopen_s(&file, fileName, "rb"); // read in binary mode
	if (file == 0) {
		return 0;
	}
	fseek(file, 0x0012, SEEK_SET);
	fread(&imageWidth, sizeof(imageWidth), 1, file);
	fread(&imageHeight, sizeof(imageHeight), 1, file);
	fseek(file, 54, SEEK_SET);
	pixelLength = imageWidth * 3;
	while (pixelLength % 4 != 0) {
		pixelLength++;
	}
	// Pixel data length = width * height
	pixelLength *= imageHeight;
	// Malloc for the image by pixel
	pixeldata = (GLubyte*)malloc(pixelLength); // memory allocation
	// Close file
	if (pixeldata == 0) {
		fclose(file);
		return 0;
	}
	if (fread(pixeldata, pixelLength, 1, file) <= 0) {
		free(pixeldata);
		fclose(file);
		return 0;
	}
	glGenTextures(1, &textureID);
	if (textureID == 0) {
		free(pixeldata);
		fclose(file);
		return 0;
	}
	// Set texture parameters
	GLint last_texture_ID = lastTextureID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixeldata);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);
	free(pixeldata);

	return textureID;
}

// Put brick texture to the wall
void setbrick() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brick);
	//left wall
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-101, 0, -151);
	glTexCoord2f(0, 1); glVertex3f(-101, 80, -151);
	glTexCoord2f(1, 1); glVertex3f(-101, 80, 151);
	glTexCoord2f(1, 0); glVertex3f(-101, 0, 151);
	glEnd();
	//right wall
	for (int i = 0; i < 2; i++) {
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(101, 0, 151-200*i);
		glTexCoord2f(0, 1); glVertex3f(101, 80, 151 - 200 * i);
		glTexCoord2f(1, 1); glVertex3f(101, 80, 49 - 200 * i);
		glTexCoord2f(1, 0); glVertex3f(101, 0, 49 - 200 * i);
		glEnd();
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(101, 0, 51);
	glTexCoord2f(0, 1); glVertex3f(101, 35, 51);
	glTexCoord2f(1, 1); glVertex3f(101, 25, -151);
	glTexCoord2f(1, 0); glVertex3f(101, 0, -151);
	glEnd();
	//back wall
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(101, 0, -151);
	glTexCoord2f(0, 1); glVertex3f(101, 80, -151);
	glTexCoord2f(1, 1); glVertex3f(-101, 80, -151);
	glTexCoord2f(1, 0); glVertex3f(-101, 0, -151);
	glEnd();
	//front wall
	for (int i = 0; i < 2; i++) {
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-101+125*i, 0, 151);
		glTexCoord2f(0, 1); glVertex3f(-101+125*i, 80, 151);
		glTexCoord2f(1, 1); glVertex3f(-24+125*i, 80, 151);
		glTexCoord2f(1, 0); glVertex3f(-24+125*i, 0, 151);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Put snowfield texture to the ground
void setsnowfield() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, snowfield);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(300, 5, -450);
	glTexCoord2f(0, 5); glVertex3f(300, 5, 450);
	glTexCoord2f(5, 5); glVertex3f(-300, 5, 450);
	glTexCoord2f(5, 0); glVertex3f(-300, 5, -450);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Put door texture
void setdoor() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, door);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-27, 0, 151);
	glTexCoord2f(0, 1); glVertex3f(-27, 80, 151);
	glTexCoord2f(1, 1); glVertex3f(25, 80, 151);
	glTexCoord2f(1, 0); glVertex3f(25, 0, 151);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Put wood texture to the road
void setboardwalk() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, boardwalk);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(300, 5, 450);
	glTexCoord2f(0, 5); glVertex3f(300, 5, 600);
	glTexCoord2f(5, 5); glVertex3f(-300, 5, 600);
	glTexCoord2f(5, 0); glVertex3f(-300, 5, 450);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Init light
void init(void) {
	GLfloat moonPosition[] = {-500 * cos(pi * moonAngle / 180) - 150, 400 * sin(pi * moonAngle / 180), 0.0, 0.0 };
	GLfloat moonIntensity[] = { 1.0, 0.8, 0.0, 0.4 };
	GLfloat ambientIntensity[] = { 0.5, 0.5, 0.5, 1.0 };
	// Set up ambient light
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientIntensity);
	// Set up moonlight
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, moonPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, moonIntensity);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLineWidth(5);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-winWidth, winWidth, -winHeight, winHeight, -winWidth, winWidth);
}

// Set a timer to control moon's moving
void onTimer(int Value) {
	moonAngle += 3;
	if (moonAngle >= 180) {
		moonAngle = 0;
	}
	glutTimerFunc(time, onTimer, 1);
}

// Idle callback function
void whenInMainloop() {
	// Force OpenGL to redraw the current window
	glutPostRedisplay();
}

// This function mixes the objects together and implements them
void display() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1, 5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(viewX, viewY, viewZ, -150, 0, 0, 0, 1, 0);
	glRotatef(rotateAngle, 0.0, 1.0, 0.0);
	glRotatef(angleX, 0.0, 1.0, 0.0);
	glRotatef(angleY, 1.0, 0.0, 0.0);
	ground();
	house();
	trees();
	snowman();
	moon();
	setbrick();
	setsnowfield();
	setboardwalk();
	setdoor();
	init();
	glFlush();
	glutSwapBuffers();
}

// Set a keyboard to control the view field size and the speed of time
void keyboardOperation(unsigned char key, int x, int y) {
	if (key == 'Q' || key == 'q') {
		exit(0);
	}
	// Enlarge
	if (key == 'L' || key == 'l') {
		viewX -= 0.1 * viewX;
		viewY -= 0.1 * viewY;
		viewZ -= 0.1 * viewZ;
	}
	// Shrink
	if (key == 'S' || key == 's') {
		viewX += 0.1 * viewX;
		viewY += 0.1 * viewY;
		viewZ += 0.1 * viewZ;
	}
	// Set the boundary of narrowing the perspective
	if (viewY >= 500) {
		viewX = -187.5;
		viewY = 500;
		viewZ = -1250;
	}
	// Set the boundary of enlarging the perspective
	if (viewY <= 200) {
		viewX = -75;
		viewY = 200;
		viewZ = -500;
	}
	// Decrease the refresh interval
	if (key == 'D' || key == 'd') {
		time -= 10;
	}
	// Increase the refresh interval
	if (key == 'F' || key == 'f') {
		time += 10;
	}
	// Set the boundary of refresh interval
	if (time >= 100) {
		time = 100;
	}
	// Set the boundary of refresh interval
	if (time <= 10) {
		time = 10;
	}
	glutPostRedisplay();
}

// The mouse() and motion() aims to use mouse to rotate the angle of view
void mouse(int button, int state, int x, int y) {
	if (button = GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			rotateSpeed = 0.0;
			rotateAngle = 0.0;
			oldX = x + winWidth / 2.0;
			oldY = y + winHeight / 2.0;
		}
	}
}

void animation(int x, int y) {
	GLint deltaX = oldX - x + winWidth / 2.0;
	GLint deltaY = oldY - y + winHeight / 2.0;
	angleX += 360 * (GLfloat)deltaX / (GLfloat)winWidth;
	angleY += 360 * (GLfloat)deltaY / (GLfloat)winHeight;
	oldX = x + winWidth / 2.0;
	oldY = y + winHeight / 2.0;
	glutPostRedisplay();
}

// The main function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("snowhouse");
	// Add texture information to the object
	brick = setTexture("brick.bmp");
	snowfield = setTexture("snowfield.bmp");
	boardwalk = setTexture("road.bmp");
	door = setTexture("door.bmp");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Recall the function
	glutDisplayFunc(display);
	glutIdleFunc(whenInMainloop);
	glutKeyboardFunc(keyboardOperation);
	glutMouseFunc(mouse);
	glutMotionFunc(animation);
	glutTimerFunc(time, onTimer, 1);
	glutMainLoop();
}