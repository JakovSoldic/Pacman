#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "pacman.h"
#include "Red.h"
#include "globalVariables.h"

Pacman p;
Blinky r;

void movePacMan(unsigned char key, int x, int y) 
{
	switch (key) {
	case 27:  // escape key
		exit(0);
		break;
	case 'd'://right
		if (maze[p.pacmanGridX][p.pacmanGridY + 1] != Tiles::wall && maze[p.pacmanGridX][p.pacmanGridY + 1] != Tiles::gate)
		{
			p.turnTo = { 1, 0 };
			break;
		}
	case 'a'://left
		if (maze[p.pacmanGridX][p.pacmanGridY - 1] != Tiles::wall && maze[p.pacmanGridX][p.pacmanGridY - 1] != Tiles::gate)
		{
			p.turnTo = { -1, 0 };
			break;
		}
	case 'w'://up
		if (maze[p.pacmanGridX - 1][p.pacmanGridY] != Tiles::wall && maze[p.pacmanGridX - 1][p.pacmanGridY] != Tiles::gate)
		{
			p.turnTo = { 0, 1 };
			break;
		}
	case 's'://down
		if (maze[p.pacmanGridX + 1][p.pacmanGridY] != Tiles::wall && maze[p.pacmanGridX + 1][p.pacmanGridY] != Tiles::gate)
		{
			p.turnTo = { 0, -1 };
			break;
		}
	}
	glutPostRedisplay();
}

void drawTiles(void)//go through the maze and then at certain locations draw certain shapes
{
	glPushMatrix();
	glTranslatef(p.pacmanXStart, -p.pacmanYStart, 0);
	p.drawMouth();
	p.drawPacMan();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(r.blinkyXStart, -r.blinkyYStart, 0);
	r.drawBlinky();
	glPopMatrix();

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

				glBegin(GL_LINES);//horizontal lines which represent the gate
				glColor3f(255, 255, 255);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(-0.5f, 0.5f);
				glEnd();
			}
            glPopMatrix();
        }
    }
}

void resetFrighten(int value) {
	p.ateBigPellet = false;
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
	p.updatePacman(deltaTime);
	if (p.ateBigPellet)
		glutTimerFunc(5000, resetFrighten, 0);

	r.checkCollision(p.pacmanGridX, p.pacmanGridY);
	r.setPath(p.pacmanGridX, p.pacmanGridY, p.ateBigPellet);
	r.setBlinkySpeed();
	r.updateBlinky(deltaTime);
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