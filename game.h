#ifndef game_H
#define game_H

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "glut.h" //standard glut header

//header files
#include "objects.h"      // Game objects header
#include "config.h"         // Game configs header
using namespace std;

class Breakout {

public:
	//Constructor & Destructor
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
	int score;
	int level;
	int reward;
	int lifeCount;

	// Possible ame mode
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
#endif // BREAKOUT_H