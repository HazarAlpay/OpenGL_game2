/*********
   CTIS164 - Template Source Program
----------
STUDENT : Dogukan Hazar Alpay
SECTION : 2
HOMEWORK: 2
----------
PROBLEMS: There is a little problem with the fire hitting the center of the target. The program compares the fire's location and the target's location and
then reappearing the target at a random location but sometimes the fire goes a little top of the target but the target still reappearing at somewhere else.
I think the problem happens because the fire location is not the origin point of the screen.
----------
ADDITIONAL FEATURES:
	-animated background
	-complex player (ufo)
	-clicking to start the game at the beggining of the game
	-information screen about the ending of the game in seconds
	-hard mode (after pressing <F3> amongus moving and the fire keep following it)
	-firing takes place from the tip of the weapon, not from the player's midpoint
	-in hard mode, if amogus disappears from the screen it will reappear at a random point
	-The program terminates itself after the game is finished.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800
#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer
#define TARGET_RADIUS 30
#define D2R 0.0174532
#define START 1
#define SHOW 0
#define EASY 1
#define DIFFICULT 2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int app_state = START;
float degree_of_tan;
int xAmogus = rand() % (1400) - 700;
int yAmogus = rand() % (800) - 400;
float xSpaceGun = -52;
float ySpaceGun = 80;
int xp, yp;
float shoterX = -52;
float shoterY = 80;
int r = rand() % 255, g = rand() % 255, b = rand() % 255;
float angle = 0;
int timer = 60 * 60;
int ending_timer = 5 * 60;
float fireX = 0;
float fireY = 0;
int mode = EASY;
bool targetLocked = false;
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//



void amogus()
{
	glColor3ub(r, g, b);
	circle(xAmogus, yAmogus, TARGET_RADIUS);
	glColor3ub(r, g, b);
	glRectf(xAmogus - 30, yAmogus, xAmogus + 30, yAmogus - 40);
	glColor3ub(r, g, b);
	glRectf(xAmogus - 40, yAmogus + 5, xAmogus, yAmogus - 40);
	glColor3ub(r, g, b);
	glRectf(xAmogus - 28, yAmogus - 55, xAmogus - 5, yAmogus - 30);
	glColor3ub(r, g, b);
	glRectf(xAmogus + 25, yAmogus - 55, xAmogus + 5, yAmogus - 30);
	glColor3ub(185, 185, 185);
	glRectf(xAmogus - 5, yAmogus + 5, xAmogus + 35, yAmogus - 15);
	glColor3ub(255, 255, 255);
	glRectf(xAmogus, yAmogus + 5, xAmogus + 35, yAmogus - 8);
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2f(xAmogus - TARGET_RADIUS + 1, yAmogus + 5);
	glVertex2f(xAmogus - TARGET_RADIUS + 1, yAmogus - 40);
	glEnd();
	glColor3ub(255, 0, 0);
	vprint(xAmogus - 12, yAmogus + 40, GLUT_BITMAP_8_BY_13, "SUS");
}



void spaceGun()
{
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3ub(122, 122, 122);
	glVertex2f(-52, -15);
	glVertex2f(-52, 80);
	glEnd();

	glColor3ub(122, 122, 122);
	circle(-52, 80, 5);

	glColor3ub(122, 122, 122);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(-52, 80);
	glVertex2f(-52 + 50 * cos(angle * D2R), 80 + 50 * sin(angle * D2R));
	glEnd();

	glColor3ub(255, 0, 0);
	circle(-52 + 50 * cos(angle * D2R), 80 + 50 * sin(angle * D2R), 5);
}

void background1()
{
	int x, y, status;
	srand(time(NULL));

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x >= 700) {
			x = x / 2 * (-1);
		}
		if (y >= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x <= 700) {
			x = x / 2 * (-1);
		}
		if (y <= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x <= 700) {
			x = x / 2 * (-1);
		}
		if (y >= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x >= 700) {
			x = x / 2 * (-1);
		}
		if (y <= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}
	vprint(-75, 300, GLUT_BITMAP_8_BY_13, "HUNTING AMOGUSES!!!");
	vprint(-650, -350, GLUT_BITMAP_8_BY_13, "Dogukan Hazar Alpay 22003159");
	vprint(-190, -390, GLUT_BITMAP_8_BY_13, "PRESS <F3> to switch the difficulty of the game");
	vprint(450, -350, GLUT_BITMAP_8_BY_13, "PRESS <ESC> to exit");
}

void createTargetAgain()
{
	xAmogus = rand() % (1400) - 700;
	yAmogus = rand() % (800) - 400;
	r = rand() % 255,
		g = rand() % 255,
		b = rand() % 255;
}

void alienInsideUfo()
{
	glColor3ub(0, 0, 0);
	circle(-52, 80, 2);

	glColor3ub(154, 255, 155);
	circle(0, 0, 50);

	glColor3ub(154, 255, 155);
	glRectf(-50, 10, 50, -25);

	glColor3ub(0, 166, 17);
	circle(0, 15, 25);

	glBegin(GL_TRIANGLES);
	glColor3ub(0, 166, 17);
	glVertex2f(-25, 10);
	glVertex2f(25, 10);
	glVertex2f(0, -40);
	glEnd();

	glColor3ub(0, 0, 0);
	circle(-12, 10, 8);
	glColor3ub(0, 0, 0);
	circle(12, 10, 8);

	glColor3f(1, 1, 1);
	circle(16, 14, 2);

	glColor3f(1, 1, 1);
	circle(-9, 14, 2);

	glColor3ub(0, 0, 0);
	circle(0, 3, 4);
	glColor3ub(0, 166, 17);
	circle(0, 5, 4);

	glColor3ub(122, 122, 122);
	glRectf(-55, -10, 55, -25);

	glBegin(GL_TRIANGLES);
	glColor3ub(122, 122, 122);
	glVertex2f(-55, -10);
	glVertex2f(-55, -25);
	glVertex2f(-70, -25);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3ub(122, 122, 122);
	glVertex2f(55, -10);
	glVertex2f(55, -25);
	glVertex2f(70, -25);
	glEnd();

	glColor3ub(122, 122, 122);
	glRectf(-70, -25, 70, -50);

	glBegin(GL_TRIANGLES);
	glColor3ub(122, 122, 122);
	glVertex2f(-70, -25);
	glVertex2f(-70, -50);
	glVertex2f(-120, -50);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3ub(122, 122, 122);
	glVertex2f(70, -25);
	glVertex2f(70, -50);
	glVertex2f(120, -50);
	glEnd();

	spaceGun();

	for (int i = 0; i < 90; i += 30) {
		glColor3ub(237, 255, 67);
		circle(0 + i, -40, 5);
	}

	for (int i = 0; i < 90; i += 30) {
		glColor3ub(237, 255, 67);
		circle(0 - i, -40, 5);
	}

	glColor3f(1, 1, 1);
	vprint(-12, -30, GLUT_BITMAP_8_BY_13, "%3d", (int)angle);
}

void drawFire()
{
	glColor3ub(255, 0, 0);
	circle(xSpaceGun + fireX, ySpaceGun + fireY, 5);
}

void endingBG()
{
	int x, y, status;
	srand(time(NULL));

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x >= 700) {
			x = x / 2 * (-1);
		}
		if (y >= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x <= 700) {
			x = x / 2 * (-1);
		}
		if (y <= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x <= 700) {
			x = x / 2 * (-1);
		}
		if (y >= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}

	for (int i = 0; i < 100; i++) {
		x = rand() % 1400;
		y = rand() % 800;

		if (x >= 700) {
			x = x / 2 * (-1);
		}
		if (y <= 800) {
			y = y / 2 * (-1);
		}
		glColor3f(1, 1, 1);
		circle(x, y, 1);
	}
	vprint(-150, 0, GLUT_BITMAP_8_BY_13, "THE TIME IS UP! %d SECONDS TO EXIT.", ending_timer / 60 + 1);
}

void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	degree_of_tan = atan2(yAmogus, xAmogus) / D2R;
	if (degree_of_tan < 0)
		degree_of_tan += 360;

	if (targetLocked == true)
		drawFire();

	if (app_state == START) {
		int red, green, blue;
		for (int i = 0; i < 20; i++) {
			red = rand() % 255;
			green = rand() % 255;
			blue = rand() % 255;
			glColor3ub(red, green, blue);
		}
		vprint(-100, 0, GLUT_BITMAP_8_BY_13, "CLICK TO START THE GAME!");
	}

	if (app_state == SHOW) {
		if ((timer / 60) > 0) {
			background1();
			alienInsideUfo();
			amogus();
			vprint(-160, 350, GLUT_BITMAP_8_BY_13, "%d seconds left until the end of the game", timer / 60);
		}
		else {
			endingBG();
			if (ending_timer == 0)
				exit(0);
		}
	}

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	if (key == GLUT_KEY_F3) {
		if (mode == EASY)
			mode = DIFFICULT;
		else
			mode = EASY;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
		if (app_state == START) {
			app_state = SHOW;
			xp = x - (winWidth / 2);
			yp = (winHeight / 2) - y;
		}
		else
		{
			xp = x - (winWidth / 2);
			yp = (winHeight / 2) - y;
		}
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (targetLocked == false)
	{
		if (angle > 360)
			angle = 0;
		else if (angle < 0)
			angle += 360;
		else
			angle++;
	}

	if ((int)angle == (int)degree_of_tan)
		targetLocked = true;

	if (targetLocked == true)
	{
		if (xAmogus != ((int)shoterX + (int)fireX))
		{
			fireY += sin(degree_of_tan * D2R);
			fireX += cos(degree_of_tan * D2R);
		}
	}


	if (xAmogus == ((int)shoterX + (int)fireX))
	{
		targetLocked = false;
		createTargetAgain();
	}

	if (targetLocked == false)
	{
		fireX = 0;
		fireY = 0;
	}

	timer--;

	if (timer / 60 <= 0 && ending_timer > 0)
		ending_timer--;

	if (ending_timer == 0)
		onSpecialKeyDown(27, 0, 0);

	int chance = rand() % 4 + 1;

	if (mode == DIFFICULT) {
		if (chance == 1) {
			yAmogus += 1;
			xAmogus += 1;
		}
		else if (chance == 2) {
			yAmogus -= 1;
			xAmogus -= 1;
		}
		else if (chance == 3) {
			yAmogus -= 1;
			xAmogus += 1;
		}
		else if (chance == 4) {
			yAmogus += 1;
			xAmogus -= 1;
		}

		if (xAmogus > 700 || yAmogus > 400 || xAmogus < -700 || yAmogus < -400) {
			createTargetAgain();
		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW-2 by Dogukan Hazar Alpay");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}