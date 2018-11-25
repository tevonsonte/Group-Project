#ifndef Objects_h
#define Objects_h

// Ball
struct Ball {
	GLfloat posX, posY;
	GLfloat velX, velY;
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
	GLint value; //
}; 

#endif
