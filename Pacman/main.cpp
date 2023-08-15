#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>
#include "globalVariables.h"
#include "Music.h"
#include "Menu.h"
#include "GameController.h"

Music m;
GameController gc;
Menu menu;

void handleKeyPacman(unsigned char key, int x, int y)
{
	gc.movePacMan(key, x, y);
}

void handleKeyMenu(unsigned char key, int x, int y)
{
	gc.keyboardMenu(key, x, y);
}

void handleKeyGameOver(unsigned char key, int x, int y)
{
	gc.keyboardGameOver(key, x, y);
}

void drawGameBoard(void)
{
	gc.drawPacman();

	gc.drawBlinky();

	gc.drawPinky();

	gc.drawInky();

	gc.drawClyde();

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
			if (maze[i][j] == 4)
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);
				gc.drawCube();
			}
			if (maze[i][j] == 1) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0); 
				gc.drawSmallPellet();
			}
			
			if (maze[i][j] == 2) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);
				gc.drawBigPellet();
			}
			
			if (maze[i][j] == 3) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);
				gc.drawGate();
			}
            glPopMatrix();
        }
    }
}

void playIntroMusic(int value) {
	m.playIntro();
	isIntroDone = true;
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


	switch (currentState) {
	case START_MENU:
		
		glutKeyboardFunc(handleKeyMenu);
		menu.displayStartText();
		break;
	
	case GAME:

		glutKeyboardFunc(handleKeyPacman);

		updateHighScore();
		menu.displayScore();

		drawGameBoard();
		
		if(!isIntroDone)
			glutTimerFunc(0, playIntroMusic, 0);
		
		if(isIntroDone)
		{
			if (gc.checkGameState())
				currentState = GAME_WON_MENU;

			m.playMovementSound();

			if (scatterCounter <= 3)
			{
				gc.startScatterDuration();
				gc.endScatterDuration();
			}

			gc.pacmanController(deltaTime);

			gc.blinkyController(deltaTime);

			if (numberOfPelletsEaten >= 30)
				gc.pinkyController(deltaTime);

			if (numberOfPelletsEaten >= 30)
				gc.inkyController(deltaTime);

			if (numberOfPelletsEaten >= 50)
				gc.clydeController(deltaTime);
		}

		break;

	case GAME_OVER_MENU:
		
		glutKeyboardFunc(handleKeyGameOver);
		m.movementSoundPlayed = false;
		menu.displayOverText();
		break;

	case GAME_WON_MENU:

		glutKeyboardFunc(handleKeyGameOver);
		menu.displayWonText();
		break;

	}
	glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void initializeTileLocation()
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (maze[i][j] == 6)
			{
				maze[i][j] = Tiles::home_tile;
			}

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
	initializeTileLocation();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(600, 200);
    glutCreateWindow("A basic OpenGL Window");
    glutDisplayFunc(display);
	glutKeyboardFunc(handleKeyMenu);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}