#ifndef game_H
#define game_H

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <gl/glut.h> //include glut header from file directory

//programmer made header files
#include "gObjects.h" // Game objects header
#include "config.h"  // Game configs header
using namespace std;

class Breakout {

public:
	int score; //score

	//Constructor / Destructor
	Breakout();
	~Breakout();

	// Public functions (GLUT calls)
	void display(void);
	void init(void);
	void reshape(int width, int height);
	void mouseClick(int button, int state, int x, int y);
	void mouseMove(int x, int y);
	void keyInput(unsigned char key, int x, int y);
	void specialKeyPos(int key, int x, int y);

private:
	// Game stats
	int level; //either 1 or 2
	int reward; //reward for completing game (score boost)
	int lifeCount; //balls / life total

	// game mode state
	enum State { INIT, Menus, Gameplay, Scoreboard };
	Breakout::State gameState;

	// Balls
	vector <Ball> balls;

	// Paddle
	Paddle paddle;

	// Bricks
	vector<Brick> bricks;

	// game creation functions
	void drawBckgrnd(void);
	void drawGame(void);
	void addBall(float x, float y);
	void drawBalls(void);
	void paddleInit(void);
	void drawPaddle(void);
	void brickInit(void);
	void levelOneBricks(void);
	void levelTwoBricks(void);
	void drawBrick(void);

	template <typename T>
	int wallHit(T it);

	template <typename T>
	bool brickHit(T it, T br);

	template <typename T>
	T hitBrick(T brick);

	void drawLifeTotal(float x, float y);
	void drawStats(void);
	void drawScore(void);
	void drawCoords(void);
};

#endif