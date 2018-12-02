#ifndef gObjects_h
#define gObjects_h

// Ball
struct Ball {
	GLfloat posX, posY;
	GLfloat velX, velY; //speeeeeeed
	GLfloat radius;
	GLfloat r, g, b;  //color wheel variables
};

// Paddle
struct Paddle {
	GLfloat posX, posY;
	GLfloat width, height;
	GLfloat r, g, b; //color wheel variables
};

// Brick
struct Brick {
	GLfloat posX, posY;
	GLfloat width, height;
	GLfloat r, g, b; //color wheel variables
	GLint health; //health of individual brick variable
	GLint value; //type of brick
};

#endif
