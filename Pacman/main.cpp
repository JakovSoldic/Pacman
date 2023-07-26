#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "pacman.h"
#include "Red.h"
#include "Pink.h"
#include "Orange.h"
#include "globalVariables.h"

Pacman player;
Blinky r;
Pinky p;
//inky here
Clyde c;

void movePacMan(unsigned char key, int x, int y) 
{
	switch (key) {
	case 27:  // escape key
		exit(0);
		break;
	case 'd'://right
		if (maze[player.pacmanGridX][player.pacmanGridY + 1] != Tiles::wall && maze[player.pacmanGridX][player.pacmanGridY + 1] != Tiles::gate)
		{
			player.turnTo = { 1, 0 };
			break;
		}
	case 'a'://left
		if (maze[player.pacmanGridX][player.pacmanGridY - 1] != Tiles::wall && maze[player.pacmanGridX][player.pacmanGridY - 1] != Tiles::gate)
		{
			player.turnTo = { -1, 0 };
			break;
		}
	case 'w'://up
		if (maze[player.pacmanGridX - 1][player.pacmanGridY] != Tiles::wall && maze[player.pacmanGridX - 1][player.pacmanGridY] != Tiles::gate)
		{
			player.turnTo = { 0, 1 };
			break;
		}
	case 's'://down
		if (maze[player.pacmanGridX + 1][player.pacmanGridY] != Tiles::wall && maze[player.pacmanGridX + 1][player.pacmanGridY] != Tiles::gate)
		{
			player.turnTo = { 0, -1 };
			break;
		}
	}
	glutPostRedisplay();
}

void drawTiles(void)
{
	glPushMatrix();
	glTranslatef(player.pacmanXStart, -player.pacmanYStart, 0);
	player.drawMouth();
	player.drawPacMan();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(r.blinkyXStart, -r.blinkyYStart, 0);
	r.drawBlinky();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(p.pinkyXStart, -p.pinkyYStart, 0);
	p.drawPinky();
	glPopMatrix();

	//inky here

	glPushMatrix();
	glTranslatef(c.clydeXStart, -c.clydeYStart, 0);
	c.drawClyde();
	glPopMatrix();

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
			if (maze[i][j] == 4)
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);

				glColor3f(0, 0, 1);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, -0.500000, 0.500000);
				glVertex3f(0.500000, -0.500000, 0.500000);
				glVertex3f(0.500000, 0.500000, 0.500000);
				glVertex3f(-0.500000, 0.500000, 0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, 0.500000, 0.500000);
				glVertex3f(0.500000, 0.500000, 0.500000);
				glVertex3f(0.500000, 0.500000, -0.500000);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, 0.500000, -0.500000);
				glVertex3f(0.500000, 0.500000, -0.500000);
				glVertex3f(0.500000, -0.500000, -0.500000);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glVertex3f(0.500000, -0.500000, -0.500000);
				glVertex3f(0.500000, -0.500000, 0.500000);
				glVertex3f(-0.500000, -0.500000, 0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(0.500000, -0.500000, 0.500000);
				glVertex3f(0.500000, -0.500000, -0.500000);
				glVertex3f(0.500000, 0.500000, -0.500000);
				glVertex3f(0.500000, 0.500000, 0.500000);
				glEnd();
				
				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glVertex3f(-0.500000, -0.500000, 0.500000);
				glVertex3f(-0.500000, 0.500000, 0.500000);
				glVertex3f(-0.500000, 0.500000, -0.500000);
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

				glBegin(GL_LINES);
				glColor3f(255, 255, 255);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(-0.5f, 0.5f);
				glEnd();
			}
            glPopMatrix();
        }
    }
}

void toggleScatter(int value)
{
	isScatter = !isScatter;

	if (scatterCounter < 2)
	{
		scatterCounter++;
		glutTimerFunc(5000, toggleScatter, 0);
	}
}

void startScatter()
{
	if (scatterCounter == 0)
	{
		scatterCounter++;
		glutTimerFunc(14000, toggleScatter, 0);
	}
}

void resetFrighten(int value) {
	player.ateBigPellet = false;
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

	//startScatter();

	player.updatePacman(deltaTime);
	if (player.ateBigPellet)
		glutTimerFunc(5000, resetFrighten, 0);

	r.checkCollision(player.pacmanGridX, player.pacmanGridY);
	r.setPath(player.pacmanGridX, player.pacmanGridY, player.ateBigPellet);
	r.setBlinkySpeed();
	r.updateBlinky(deltaTime);

	//p.checkCollision(player.pacmanGridX, player.pacmanGridY);
	//p.setPath(player.pacmanGridX, player.pacmanGridY, player.ateBigPellet, player.turnTo.x, player.turnTo.y);
	//p.setPinkySpeed();
	//p.updatePinky(deltaTime);

	//inky here

	c.checkCollision(player.pacmanGridX, player.pacmanGridY);
	c.setPath(player.pacmanGridX, player.pacmanGridY, player.ateBigPellet);
	c.setClydeSpeed();
	c.updateClyde(deltaTime);

	glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void saveTileLocation()
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (maze[i][j] == 5)
			{
				maze[i][j] = Tiles::teleport_tile;
			}

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