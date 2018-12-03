#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <GL/glut.h>
#include "glutMenu.h"

Breakout game;
//functions to call game members

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

//functions to call Menu members

// display function
void gDraw()
{
	Draw();
}

// mouse click event function
void gMouseButton(int button, int state, int x, int y)
{
	MouseButton(button, state, x, y);
}

// mouse drag event function
void gMousePassiveMotion(int x, int y)
{
	MousePassiveMotion(x, y);
}