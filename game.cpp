#include "game.h"
#include "endingStuff.h";
#include <Windows.h>
#include <mmsystem.h>
using namespace std;

void redrawFrame(int value);

Breakout::Breakout() {
	init();
}

Breakout::~Breakout() {

}

void Breakout::display(void) {

	// buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// OpenGL for drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glOrtho(0.0f, winWidth, winHeight, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw background call
	drawBckgrnd();

	// choose state of the game to display
	switch (gameState) {
	case INIT:
		// Init values
		init();
		break;

	case Gameplay:
		// Draw the game
		drawGame();
		// If ball leaves window and player has lives, subtract 1 life
		if (balls.size() <= 0 && lifeCount > 0) {
			addBall(-1, -1);
			lifeCount--;
			reward = 100;

		} //if no more balls and 0 lifes, end game
		else if (balls.size() <= 0) {

			system("cls");
			cout << "Uh oh.... The Player has lost his / her game.\n";
			cout << "Exiting Program...\n";

			addScore(score);

			system("pause");
			exit(0);
		}

		// If level 1 beaten move to level 2
		if (bricks.size() <= 0 && level <= 2) {
			level++;
			brickInit();
		}
		else if (bricks.size() <= 0) { //if both levels beat add new score
			system("cls");
			cout << "\nThe Player has cleared both levels...\n";
			cout << "Summoning Victory Screen...\n";

			displayVictory();
			cout << endl;
			addScore(score);

			system("pause");
			exit(0);
		}
		break;

	default:
		break;
	}

	glutTimerFunc(refresh, redrawFrame, 0); //redraw game and use preset timer from header

	glutSwapBuffers();
}

void redrawFrame(int value) {
	glutPostRedisplay();
}

void Breakout::init(void) {
	// Reset game statistics
	score = 0;
	level = 1;
	reward = 100;
	lifeCount = 3;

	// clear balls
	balls.clear();

	// clear bricks
	bricks.clear();

	// call brick init
	brickInit();

	// add a ball and paddle
	paddleInit();
	addBall(-1, -1);

	// start game
	gameState = Breakout::Gameplay;
}

void Breakout::drawBckgrnd(void) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	// top coloring
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(winWidth, winHeight);
	glVertex2f(-winWidth, winHeight);
	// bottom coloring
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glEnd();
}

void Breakout::drawGame(void) {

	// display balls
	drawBalls();

	//  display bricks
	drawBrick();

	// display paddle
	drawPaddle();

	// display game stats (life total and score)
	drawStats();

}

void Breakout::addBall(float x = -1, float y = -1) {
	Ball b1;
	if (x < 0 || y < 0) {
		b1.posX = winWidth / 2.0;
		b1.posY = winHeight - 30.0f;
	}
	else {
		b1.posX = x;
		b1.posY = y;
	}
	if ((float)rand() / (RAND_MAX) < 0.5)
		b1.velX = 5.0f;
	else
		b1.velX = -5.0f;
	b1.velY = -10.0f;
	b1.radius = BallRadius;
	//randomize colors of new balls !
	b1.r = 0.4f + (float)rand() / (RAND_MAX);
	b1.g = 0.25f + (float)rand() / (RAND_MAX);
	b1.b = 0.4f + (float)rand() / (RAND_MAX);
	balls.push_back(b1);
}

void Breakout::drawBalls(void) {
	for (vector<Ball>::iterator it = balls.begin(); it != balls.end(); ) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // use GL_LINE if no fill
		glBegin(GL_POLYGON);
		glColor3f(it->r, it->g, it->b);
		for (int j = 0; j < CircleParts; j++) {
			float const theta = 2.0f * 3.1415926f * (float)j / (float)CircleParts;
			float const x = it->radius * cosf(theta);
			float const y = it->radius * sinf(theta);
			glVertex2f(x + it->posX, y + it->posY);
		}
		glEnd();

		// Set new position
		it->posX += it->velX;
		it->posY += it->velY;

		// Collision with left/right/top window sides
		if ((it->posX <= (2 * it->radius)) || (it->posX >= (winWidth - 2 * it->radius))) {
			it->velX *= -1;
		}
		if ((it->posY <= (2 * it->radius))) {
			it->velY *= -1;
		}
		if (it->posY >= (winHeight - 2 * it->radius)) {
			it = balls.erase(it);
			continue;
		}

		// Collision with the bricks
		for (vector<Brick>::iterator br = bricks.begin(); br != bricks.end(); ) {
			// Check collision between circle and vertical brick sides
			if (it->posY >= br->posY && it->posY <= br->posY + br->height) {
				// ball hits right vertical side of brick
				if ((it->posX - it->radius - br->posX - br->width) <= 5 && (it->posX - it->radius - br->posX - br->width) >= 0) {
					it->velX *= -1;
					br = hitBrick(br);
					continue;
				}

				// ball hits left vertical side of brick
				if ((it->posX + it->radius - br->posX) >= -5 && (it->posX + it->radius - br->posX) <= 0) {
					it->velX *= -1;
					br = hitBrick(br);
					continue;
				}
			}

			// Check collision between circle and horizontal brick sides
			if (it->posX >= br->posX && it->posX <= br->posX + br->width) {
				// ball hits bottom of brick
				if ((it->posY - it->radius - br->posY - br->height) <= 10 && (it->posY - it->radius - br->posY - br->height) >= 0) {
					it->velY *= -1;
					br = hitBrick(br);
					continue;
				}

				// ball hits top of brick
				if ((it->posY + it->radius - br->posY) >= -10 && (it->posY + it->radius - br->posY) <= 0) {
					it->velY *= -1;
					br = hitBrick(br);
					continue;
				}
			}

			GLfloat d;
			// Check collision with top left corner
			d = pow((it->posX - br->posX), 2.0) + pow((it->posY - br->posY), 2.0);
			if (d < it->radius + 5.0) {
				it->velX *= -1;
				it->velY *= -1;
				br = hitBrick(br);
				continue;
			}

			// Check collision with top right corner
			d = pow((it->posX - br->posX - br->width), 2.0) + pow((it->posY - br->posY), 2.0);
			if (d < it->radius + 5.0) {
				it->velX *= -1;
				it->velY *= -1;
				br = hitBrick(br);
				continue;
			}

			// Check collision with bottom left corner
			d = pow((it->posX - br->posX), 2.0) + pow((it->posY - br->posY - br->height), 2.0);
			if (d < it->radius + 5.0) {
				it->velX *= -1;
				it->velY *= -1;
				br = hitBrick(br);
				continue;
			}

			// Check collission with bottom right corner
			d = pow((it->posX - br->posX - br->width), 2.0) + pow((it->posY - br->posY - br->height), 2.0);
			if (d < it->radius + 5.0) {
				it->velX *= -1;
				it->velY *= -1;
				br = hitBrick(br);
				continue;
			}

			++br; // next brick
		}

		// Check collision between paddle's top edge and bottom point on circle
		if (it->posX >= paddle.posX && it->posX <= paddle.posX + paddle.width) {
			if ((it->posY + it->radius - paddle.posY) >= -10 && (it->posY + it->radius - paddle.posY) <= 0) {
				it->velY *= -1;
				reward = 100;
				score += reward;
				continue;
			}
		}

		++it; // next ball
	}
}

void Breakout::paddleInit(void) {
	paddle.r = 0.2f;
	paddle.g = 0.5f;
	paddle.b = 1.0f;
	paddle.width = 150.0f;
	paddle.height = 12.0f;
	paddle.posX = winWidth / 2.0f - paddle.width / 2.0f;
	paddle.posY = winHeight - 20.0f;
}

void Breakout::drawPaddle() {
	if (paddle.width < 25) {
		paddle.width = 25;
	} //forces specific paddle width

	glColor3f(paddle.r, paddle.g, paddle.b);
	glRectf(paddle.posX, paddle.posY, paddle.posX + 5.0f, paddle.posY + paddle.height);
	glRectf(paddle.posX + 10.0f, paddle.posY, paddle.posX + paddle.width - 10.0f, paddle.posY + paddle.height);
	glRectf(paddle.posX + paddle.width - 5.0f, paddle.posY, paddle.posX + paddle.width, paddle.posY + paddle.height);
}

void Breakout::drawBrick(void) {
	for (vector<Brick>::iterator it = bricks.begin(); it != bricks.end(); ++it) { //iterate through brick vector set colors etc.
		glColor3f(it->r, it->g, it->b);
		glRectf(it->posX, it->posY, it->posX + it->width, it->posY + it->height);

		// colors for the top gradiant / triangle texture thingy to make it look cooler
		glBegin(GL_QUADS);
		glColor3f(it->r - 0.3f, it->g - 0.2f, it->b - 0.4f);
		glVertex2f(it->posX, it->posY);
		glColor3f(it->r - 0.05f, it->g - 0.08f, it->b - 0.1f);
		glVertex2f(it->posX + it->width, it->posY);
		glColor3f(it->r - 0.15f, it->g - 0.15f, it->b - 0.15f);
		glVertex2f(it->posX + it->width, it->posY + it->height);
		glVertex2f(it->posX, it->posY);
		glEnd();
	}
}

template <typename T> //handles either level of bricks
T Breakout::hitBrick(T brick) {
	score += reward;
	reward += 25;
	
	PlaySound(TEXT("Beep8.wav"), NULL, SND_FILENAME | SND_ASYNC);

	// Decrease brick health
	if (brick->health > 1) {
		brick->r = 0.95f;
		brick->g = 0.95f;
		brick->b = 0.95f;
		brick->health -= 1;
		return ++brick;
	}
	else {
		return bricks.erase(brick);
	}
}

void Breakout::brickInit(void) {
	if (level == 1)
		levelOneBricks();
	else if (level == 2)
		levelTwoBricks();
}

void Breakout::levelOneBricks(void) {
	Brick newBrick;
	//purple -> 1r and 1b , 0g values
	//orange -> 1r / .5g , 0b values

	newBrick.width = (wallWidth - (wallColmns - 2) * wallSpace) / wallColmns;
	newBrick.height = (wallHeight - (wallRows - 2) * wallSpace) / wallRows;

	for (int i = 0; i < wallRows; ++i) {
		for (int j = 0; j < wallColmns; ++j) {
			// Set stronger brick to purple
			if (i + 1 > ceil(wallRows / 2.0) - 2 && i < ceil(wallRows / 2.0) + 2 && j + 2 > ceil(wallColmns / 2.0) - 3 && j < ceil(wallColmns / 2.0) + 3) {
				newBrick.r = 1.0f;
				newBrick.g = 0.0f;
				newBrick.b = 1.0f;
				newBrick.health = 2;
			}
			else {
				newBrick.r = 1.0f;
				newBrick.g = 0.50f;
				newBrick.b = 0.00f;
				newBrick.health = 1;
			}

			newBrick.posX = wallX + j * newBrick.width + j * wallSpace;
			newBrick.posY = wallY + i * newBrick.height + i * wallSpace;
			bricks.push_back(newBrick);
		}
	}
}

void Breakout::levelTwoBricks(void) {
	Brick newBrick;
	newBrick.width = (wallWidth - (wallColmns - 2) * wallSpace) / wallColmns;
	newBrick.height = (wallHeight - (wallRows - 2) * wallSpace) / wallRows;
	
	//level 2 basic bricks are green
	for (int i = 0; i < wallRows; i++) {
		for (int j = 0; j < wallColmns; j++) {
			// Set stronger bricks to purple
			if (i == 1 || i == wallRows - 2 || j == 1 || j == wallColmns - 2) {
				newBrick.r = 1.0f;
				newBrick.g = 0.0f;
				newBrick.b = 1.0f;
				newBrick.health = 2;
			}
			else {
				newBrick.r = 0.00f;
				newBrick.g = 1.00f;
				newBrick.b = 0.00f;
				newBrick.health = 1;
			}

			newBrick.posX = wallX + j * newBrick.width + j * wallSpace;
			newBrick.posY = wallY + i * newBrick.height + i * wallSpace;
			bricks.push_back(newBrick);
		}
	}
}

void Breakout::drawStats(void) {
	glBegin(GL_LINES);
	// Bottom right (red)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(20.0f, 30.0f);
	glVertex2f(winWidth - 20.0f, 30.0f);
	glEnd();

	float offset = 25.0f;
	for (int i = 0; i < lifeCount && i < 10; ++i) {
		drawLifeTotal(35 + offset * i, 15);
	}

	drawScore();
}

void Breakout::drawLifeTotal(float x, float y) {
	// Scale the heart symbol
	float const scale = 0.5f;

	// Heart symbol equations
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.2f, 0.2f);
	
	//crazy math function for the hearts
	for (int j = 0; j < CircleParts; j++) {
		float const theta = 2.0f * 3.1415926f * (float)j / (float)CircleParts;
		float const xx = scale * 16.0f * sinf(theta) * sinf(theta) * sinf(theta);
		float const yy = -1 * scale * (13.0f * cosf(theta) - 5.0f * cosf(2.0f * theta) - 2 * cosf(3.0f * theta) - cosf(4.0f * theta));
		glVertex2f(x + xx, y + yy);
	}
	glEnd();
}

void Breakout::drawScore(void) { //display the score (either Times new roman or Helvetica)
	glPushMatrix();
	//score color, font type, pos, etc.
	glColor3f(1.0f, 0.7f, 0.7f);
	glRasterPos2f(winWidth - 120, 20);
	char buf[300], *p;
	p = buf;
	sprintf_s(buf, "Score: ");
	do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while (*(++p));
	// print score
	p = buf;
	sprintf_s(buf, "           %d", score);
	glColor3f(1.0f, 0.2f, 0.2f);
	glRasterPos2f(winWidth - 120, 20);
	do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while (*(++p));
	glPopMatrix();
}

void Breakout::reshape(int width, int height) {
	if (width != winWidth || height != winHeight)
		glutReshapeWindow(winWidth, winHeight);
}

void Breakout::mouseClick(int button, int state, int x, int y) { //add a ball on mouse click option
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		addBall(x, y);
	}

	// Force redraw
	glutPostRedisplay();
}

void Breakout::mouseMove(int x, int y) { //control paddle with pure mouse movement
	y = winHeight - y;
	if (x - paddle.width / 2.0f >= 0 && x + paddle.width / 2.0f <= winWidth) {
		paddle.posX = x - paddle.width / 2.0f;
	}
	else if (x - paddle.width / 2.0f <= 0) {
		paddle.posX = 0;
	}
	else if (x + paddle.width / 2.0f >= winWidth) {
		paddle.posX = winWidth - paddle.width;
	}
	glutPostRedisplay();
}

void Breakout::keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'q': // Exit game
		exit(0);
		break;
	case 'n': // New game
		init();
		break;
	case 'h': //add more balls (health)
		lifeCount++;
		break;
	case 27: // esc button case
		exit(0);
		break;
	default:
		break;
	}
}

void Breakout::specialKeyPos(int key, int x, int y) { //game paddle movement with arrow keys
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if (paddle.posX > 0) {
			paddle.posX -= 5.0f;
			paddle.posX -= 5.0f;
			glutPostRedisplay();
			paddle.posX -= 5.0f;
			paddle.posX -= 5.0f;
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_RIGHT:
		if (paddle.posX + paddle.width < winWidth) {
			paddle.posX += 5.0f;
			paddle.posX += 5.0f;
			glutPostRedisplay();
			paddle.posX += 5.0f;
			paddle.posX += 5.0f;
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}