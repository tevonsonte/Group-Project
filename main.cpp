#include <iostream>
#include <assert.h>
#include "math.h"
#include <gl/glut.h> //include from file directory
#include "game.h"
#include "consoleStuff.h"
#include <Windows.h>
using namespace std;

Breakout game;

// display function
void myDisplay()
{
	game.display();
}

// redraw function
void redraw(int width, int height)
{
	game.reshape(width, height);
}

// mouse click event function
void mouseClick(int button, int state, int x, int y)
{
	game.mouseClick(button, state, x, y);
}

// mouse drag event function
void mouseMovement(int x, int y)
{
	game.mouseMove(x, y);
}

// key press event function
void keyPress(unsigned char key, int x, int y)
{
	game.keyInput(key, x, y);
}

void specialKeyPress(int key, int x, int y)
{
	game.specialKeyPos(key, x, y);
}

// main
int main(int argc, char ** argv)
{
	MainMenu m;

	int user = m.display(); //call menu

	switch (user) {
	case 1: { //start game case

		// Init for glut
		glutInit(&argc, argv);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
		// display Init

		//set up game window
		glutInitWindowSize(winWidth, winHeight);
		glutInitWindowPosition(100, 100);
		glutCreateWindow(title);

		Sleep(500); //slow down loading so game doesnt start before player is ready

		// game init
		game.init();

		// display game
		glutDisplayFunc(myDisplay);
		// call reshape function
		glutReshapeFunc(redraw);
		// call mouse click event
		glutMouseFunc(mouseClick);

		// track mouse motion function calls
		//	glutMotionFunc(myMouseMove);
		glutPassiveMotionFunc(mouseMovement);
		// grab keyboard input
		glutKeyboardFunc(keyPress);
		// identify special key inputs
		glutSpecialFunc(specialKeyPress);
		// opengl loop
		glutMainLoop();
		
	} break;
	case 2: { //scoreboard case
		m.scoreboard();
	}break;
	case 3: //exit case
		return 0;
	}


	system("pause");
	return 0;
} // end main