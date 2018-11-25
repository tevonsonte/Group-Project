#include "game.h"

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

	case Menus:
		// TODO List menu
		break;

	case Gameplay:
		// Draw the game
		drawGame();
		// If no balls, player loses the game
		if (balls.size() <= 0 & lifeCount > 0) {
			addBall(-1, -1);
			lifeCount--;
			reward = 100;
		}
		else if (balls.size() <= 0) {
			// TODO - GAME OVER
		}

		// If no bricks, player wins the level
		if (bricks.size() <= 0 && level <= 2) {
			level++;
			brickInit();
		}
		else if (bricks.size() <= 0) {
			// TODO - PLAYER WON
		}
		break;

	case Scoreboard:
		// TODO
		break;

	default:
		break;
	}

	glutTimerFunc(refresh, redrawFrame, 0);

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
	// // Draw coordinates for guidance
	// drawCoordinate();

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
	b1.r = 0.4f + (float)rand() / (RAND_MAX);
	b1.g = 0.25f + (float)rand() / (RAND_MAX);
	b1.b = 0.4f + (float)rand() / (RAND_MAX);
	balls.push_back(b1);
}

void Breakout::drawBalls(void) {
	for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ) {
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

		// Collission with the bricks
		for (std::vector<Brick>::iterator br = bricks.begin(); br != bricks.end(); ) {
			// Check collission between circle and vertical brick sides
			if (it->posY >= br->posY && it->posY <= br->posY + br->height) {
				// brick right edge and left point on circle
				if ((it->posX - it->radius - br->posX - br->width) <= 5 && (it->posX - it->radius - br->posX - br->width) >= 0) {
					it->velX *= -1;
					br = hitBrick(br);
					continue;
				}

				// brick left edge and right point on circle
				if ((it->posX + it->radius - br->posX) >= -5 && (it->posX + it->radius - br->posX) <= 0) {
					it->velX *= -1;
					br = hitBrick(br);
					continue;
				}
			}

			// Check collission between circle and horizontal brick sides
			if (it->posX >= br->posX && it->posX <= br->posX + br->width) {
				// brick bottom edge and top point on circle
				if ((it->posY - it->radius - br->posY - br->height) <= 10 && (it->posY - it->radius - br->posY - br->height) >= 0) {
					it->velY *= -1;
					br = hitBrick(br);
					continue;
				}

				// brick top edge and bottom point on circle
				if ((it->posY + it->radius - br->posY) >= -10 && (it->posY + it->radius - br->posY) <= 0) {
					it->velY *= -1;
					br = hitBrick(br);
					continue;
				}
			}

			GLfloat d;
			// Check collission with top left corner
			d = pow((it->posX - br->posX), 2.0) + pow((it->posY - br->posY), 2.0);
			if (d < it->radius + 5.0) {
				it->velX *= -1;
				it->velY *= -1;
				br = hitBrick(br);
				continue;
			}

			// Check collission with top right corner
			d = pow((it->posX - br->posX - br->width), 2.0) + pow((it->posY - br->posY), 2.0);
			if (d < it->radius + 5.0) {
				it->velX *= -1;
				it->velY *= -1;
				br = hitBrick(br);
				continue;
			}

			// Check collission with bottom left corner
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

		// Check collission between paddle's top edge and bottom point on circle
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
	// Make sure paddle is larger than 25px
	if (paddle.width < 25) {
		paddle.width = 25;
	}

	glColor3f(paddle.r, paddle.g, paddle.b);
	glRectf(paddle.posX, paddle.posY, paddle.posX + 5.0f, paddle.posY + paddle.height);
	glRectf(paddle.posX + 10.0f, paddle.posY, paddle.posX + paddle.width - 10.0f, paddle.posY + paddle.height);
	glRectf(paddle.posX + paddle.width - 5.0f, paddle.posY, paddle.posX + paddle.width, paddle.posY + paddle.height);
}

void Breakout::drawBrick(void) {
	for (std::vector<Brick>::iterator it = bricks.begin(); it != bricks.end(); ++it) {
		glColor3f(it->r, it->g, it->b);
		glRectf(it->posX, it->posY, it->posX + it->width, it->posY + it->height);

		// Top cool triangle (kind of texture)
		glBegin(GL_QUADS);
		glColor3f(it->r - 0.2f, it->g - 0.2f, it->b - 0.2f);
		glVertex2f(it->posX, it->posY);
		glColor3f(it->r - 0.05f, it->g - 0.05f, it->b - 0.05f);
		glVertex2f(it->posX + it->width, it->posY);
		glColor3f(it->r - 0.15f, it->g - 0.15f, it->b - 0.15f);
		glVertex2f(it->posX + it->width, it->posY + it->height);
		glVertex2f(it->posX, it->posY);
		glEnd();
	}
}

template <typename T>
T Breakout::hitBrick(T brick) {
	score += reward;
	reward += 25;
	//    system("afpqlay ../../cartoon008.wav");

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
	newBrick.r = 0.95f;
	newBrick.g = 0.95f;
	newBrick.b = 0.95f;
	newBrick.health = 1;
	newBrick.width = (wallWidth - (wallColmns - 2) * wallSpace) / wallColmns;
	newBrick.height = (wallHeight - (wallRows - 2) * wallSpace) / wallRows;

	for (int i = 0; i < wallRows; ++i) {
		for (int j = 0; j < wallColmns; ++j) {
			// Set stronger bricks
			if (i + 1 > ceil(wallRows / 2.0) - 2 && i < ceil(wallRows / 2.0) + 2 && j + 2 > ceil(wallColmns / 2.0) - 3 && j < ceil(wallColmns / 2.0) + 3) {
				newBrick.r = 1.0f;
				newBrick.g = 0.5f;
				newBrick.b = 0.5f;
				newBrick.health = 2;
			}
			else {
				newBrick.r = 0.95f;
				newBrick.g = 0.95f;
				newBrick.b = 0.95f;
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

	for (int i = 0; i < wallRows; i++) {
		for (int j = 0; j < wallColmns; j++) {
			// Set stronger bricks
			if (i == 1 || i == wallRows - 2 || j == 1 || j == wallColmns - 2) {
				newBrick.r = 1.0f;
				newBrick.g = 0.5f;
				newBrick.b = 0.5f;
				newBrick.health = 2;
			}
			else {
				newBrick.r = 0.95f;
				newBrick.g = 0.95f;
				newBrick.b = 0.95f;
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
	for (int i = 0; i < lifeCount & i < 10; ++i) {
		drawLifeTotal(35 + offset * i, 15);
	}

	drawScore();
}

void Breakout::drawLifeTotal(float x, float y) {
	// Scale the heart symbol
	float const scale = 0.5f;

	// Heart symbol equations from Walfram Mathworld: http://mathworld.wolfram.com/HeartCurve.html
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.2f, 0.2f);
	for (int j = 0; j < CircleParts; j++) {
		float const theta = 2.0f * 3.1415926f * (float)j / (float)CircleParts;
		float const xx = scale * 16.0f * sinf(theta) * sinf(theta) * sinf(theta);
		float const yy = -1 * scale * (13.0f * cosf(theta) - 5.0f * cosf(2.0f * theta) - 2 * cosf(3.0f * theta) - cosf(4.0f * theta));
		glVertex2f(x + xx, y + yy);
	}
	glEnd();
}

void Breakout::drawScore(void) {
	glPushMatrix();
	// Write score word
	glColor3f(1.0f, 0.7f, 0.7f);
	glRasterPos2f(winWidth - 120, 20);
	char buf[300], *p;
	p = buf;
	sprintf(buf, "Score: ");
	do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while (*(++p));
	// Print the score
	p = buf;
	sprintf(buf, "           %d", score);
	glColor3f(1.0f, 0.2f, 0.2f);
	glRasterPos2f(winWidth - 120, 20);
	do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while (*(++p));
	glPopMatrix();
}

void Breakout::drawCoords(void) {
	glBegin(GL_LINES);
	// Top left (white)
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(20.0f, 10.0f);
	glVertex2f(20.0f, 30.0f);
	glVertex2f(10.0f, 20.0f);
	glVertex2f(30.0f, 20.0f);

	// Bottom right (red)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(winWidth - 20.0f, winHeight - 10.0f);
	glVertex2f(winWidth - 20.0f, winHeight - 30.0f);
	glVertex2f(winWidth - 10.0f, winHeight - 20.0f);
	glVertex2f(winWidth - 30.0f, winHeight - 20.0f);
	glEnd();
}

void Breakout::reshape(int width, int height) {
	if (width != winWidth || height != winHeight)
		glutReshapeWindow(winWidth, winHeight);
}

void Breakout::mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		addBall(x, y);
	}

	// Force redraw
	glutPostRedisplay();
}

void Breakout::mouseMove(int x, int y) {
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
	case 'q': // Exit
		exit(0);
		break;
	case 'n': // New game
		init();
		break;
	case 'h':
		lifeCount++;
		break;
	case 27: // Esc button
		exit(0);
		break;
	default:
		break;
	}
}

void Breakout::specialKeyPos(int key, int x, int y) {
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