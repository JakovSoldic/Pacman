#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <array>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "pacmanVariables.h"
#include "Maze.h"

void drawMouth()//draw a mouth on pacmans location(basically a black triangle). Might possibly change this in the future
{
	glPushMatrix();
	glTranslatef(pacmanX, pacmanY, 0);
	if (mouthDirection == 0) {
		glRotatef(0, 0, 0, 1);
	}
	if (mouthDirection == 1) {
		glRotatef(180, 0, 0, 1);
	}
	if (mouthDirection == 2) {
		glRotatef(90, 0, 0, 1);
	}
	if (mouthDirection == 3) {
		glRotatef(-90, 0, 0, 1);
	}

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.499, -0.37, 0);
	glVertex3f(0.499, 0.37, 0);
	glEnd();
	glPopMatrix();
}

void drawPacMan()//draw a circle shape. Will have to make mouth within this function and then just rorate the whole pacman when he moves or will possibly texture the mouth on pacman with a future function 
{
	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(pacmanX, pacmanY, 0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 0.0); // yellow color
	glVertex3f(0.0, 0.0, 0.0);
	for (int i = 0; i <= SEGMENTS; i++)
	{
		float angle = PI * 2 * (float)i / (float)SEGMENTS;
		float x = cos(angle);
		float y = sin(angle);
		glVertex3f(x * 0.5, y * 0.5, 0.0);
	}
	glEnd();
	glPopMatrix();
}

void updatePacman(float deltaTime)//after the user presses a key(WASD) within the movePacMan() function, call this function to check if pacman can move/check if there is a wall. Will have to update this function once the issue with the pixel to grid translation is resolved
{
	//pacman is being drawn at the 6 location in the maze or his exact coords in the maze: [23][13]
	pacmanGridY = (int)(13 + pacmanX);//constantly update his location in the maze/grid by incrementing/decrementing his Y coordinate in the maze by pacman X pixel/drawn coordinates
	pacmanGridX = (int)(23 + pacmanY);//constantly update his location in the maze/grid by incrementing/decrementing his X coordinate in the maze by pacman Y pixel/drawn coordinates

	//why increment his Y axis with his X drawn coordinates and vice versa? The maze is being drawn [j][i] instead of [i][j]

	//mouthDirection is being used to figure out which direction will he be/is turning. Will have to replace with a better system after I figure out the issue with the pixel to grid translation 
	if (mouthDirection == 0)
	{
		if (maze[pacmanGridX][pacmanGridY + 1] != Tiles::wall)//check if pacman has hit a wall when going right + 1. '+1' means 1 tile lenght since a tile is long 1. Check drawTiles() for refrence. Will need to include the Tiles::gate aswell here
		{
			pacmanX += pacmanVX * deltaTime;//move pacman in the X axis direction by his velocity * deltaTime. deltaTime to make his movement smoother
		}
	}

	if (mouthDirection == 1)
	{
		if (maze[pacmanGridX][pacmanGridY - 1] != Tiles::wall)
		{
			pacmanX += pacmanVX * deltaTime;
		}
	}

	if (mouthDirection == 2)
	{
		if (maze[pacmanGridX - 1][pacmanGridY] != Tiles::wall)
		{
			pacmanY += pacmanVY * deltaTime;//move pacman in the Y axis direction by his velocity * deltaTime. deltaTime to make his movement smoother
		}
	}

	if (mouthDirection == 3)
	{
		if (maze[pacmanGridX + 1][pacmanGridY] != Tiles::wall)
		{
			pacmanY += pacmanVY * deltaTime;
		}
	}
}

void movePacMan(unsigned char key, int x, int y) 
{
	switch (key) {
	case 27:  // escape key
		exit(0);
		break;
	case 'd'://right
		mouthDirection = 0;//rotate mouth to the right inside drawMouth() function
		pacmanVX = playerSpeed;//set pacman X velocity to 2.5
		pacmanVY = 0;//set pacman Y velocity to 0
		break;
	case 'a'://left
		mouthDirection = 1;//rotate mouth to the left inside drawMouth() function
		pacmanVX = -playerSpeed;
		pacmanVY = 0;
		break;
	case 'w'://up
		mouthDirection = 2;//rotate mouth to the up inside drawMouth() function
		pacmanVY = playerSpeed;//set pacman Y velocity to 2.5
		pacmanVX = 0;//set pacman X velocity to 0
		break;
	case 's'://down
		mouthDirection = 3;//rotate mouth to the down inside drawMouth() function
		pacmanVY = -playerSpeed;
		pacmanVX = 0;
		break;
	}
	glutPostRedisplay();
}

void drawTiles(void)//go through the maze and then at certain locations draw certain shapes
{
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
			if (maze[i][j] == 4)
			{
				glPushMatrix(); //push the matrix so that our translations only affect this tile
				glTranslatef(j, -i, 0); //translate the tile to where it should belong

				//the code below draws 6 squares that represent each side of a cube, each one colored with a teal color except the top square being blue(glColor3f(0, 0, 1))
				glColor3f(0, 0, 1);
				glBegin(GL_POLYGON);
				glVertex3f(-0.5, -0.5, 0.5);
				glVertex3f(0.5, -0.5, 0.5);
				glVertex3f(0.5, 0.5, 0.5);
				glVertex3f(-0.5, 0.5, 0.5);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.5, 0.5, 0.5);
				glVertex3f(0.5, 0.5, 0.5);
				glVertex3f(0.5, 0.5, -0.5);
				glVertex3f(-0.5, -0.5, -0.5);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.5, 0.5, -0.5);
				glVertex3f(0.5, 0.5, -0.5);
				glVertex3f(0.5, -0.5, -0.5);
				glVertex3f(-0.5, -0.5, -0.5);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.5, -0.5, -0.5);
				glVertex3f(0.5, -0.5, -0.5);
				glVertex3f(0.5, -0.5, 0.5);
				glVertex3f(-0.5, -0.5, 0.5);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(0.5, -0.5, 0.5);
				glVertex3f(0.5, -0.5, -0.5);
				glVertex3f(0.5, 0.5, -0.5);
				glVertex3f(0.5, 0.5, 0.5);
				glEnd();
				
				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.5, -0.5, -0.5);
				glVertex3f(-0.5, -0.5, 0.5);
				glVertex3f(-0.5, 0.5, 0.5);
				glVertex3f(-0.5, 0.5, -0.5);
				glEnd();
			}
			if (maze[i][j] == 1) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0); 

				glColor3f(255,255,255);
				glutSolidSphere(0.13f, 10, 10);
			}
			
			if (maze[i][j] == 2) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);

				glColor3f(1, 0, 0);
				glutSolidSphere(0.3f, 10, 10);
			}
			
			if (maze[i][j] == 3) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);

				glBegin(GL_LINES);//horizontal lines which represent the gate
				glColor3f(255, 255, 255);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(-0.5f, 0.5f);
				glEnd();
			}

			if (maze[i][j] == 6)
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);
				drawMouth();
				drawPacMan();
			}

            glPopMatrix();
        }
    }
}

void display(void) 
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - previousTime) / 1000.0f;
	previousTime = currentTime;
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
    glTranslatef(-13.5, 15, -30);

    drawTiles(); 
	updatePacman(deltaTime);


    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void saveTileLocation()//set all of the tile location
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (maze[i][j] == 4)
			{
				maze[i][j] = Tiles::wall;
			}

			if (maze[i][j] == 3)
			{
				maze[i][j] = Tiles::gate;
			}

			if (maze[i][j] == 2)
			{
				maze[i][j] = Tiles::big_pellet;
			}

			if (maze[i][j] == 1)
			{
				maze[i][j] = Tiles::small_pellet;
			}
		}
	}
}

int main(int argc, char** argv) 
{
	saveTileLocation();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(600, 200);
    glutCreateWindow("A basic OpenGL Window");
    glutDisplayFunc(display);
	glutKeyboardFunc(movePacMan);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}

