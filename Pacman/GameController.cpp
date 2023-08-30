#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;

#include <GL/GL.h>
#include <GL/freeglut.h>
#include "GameController.h"

//stuff for drawing on the screen___________________________________________________________
void GameController::drawPacman()
{
	glPushMatrix();
	glTranslatef(player.getPacmanXStart(), -player.getPacmanYStart(), 0);
	player.drawMouth();
	player.drawPacMan();
	glPopMatrix();
}

void GameController::drawBlinky()
{
	glPushMatrix();
	glTranslatef(r.getBlinkyXStart(), -r.getBlinkyYStart(), 0);
	r.drawEyes();
	r.drawBlinky();
	glPopMatrix();
}

void GameController::drawPinky()
{
	glPushMatrix();
	glTranslatef(p.getPinkyXStart(), -p.getPinkyYStart(), 0);
	p.drawEyes();
	p.drawPinky();
	glPopMatrix();
}

void GameController::drawInky()
{
	glPushMatrix();
	glTranslatef(t.getInkyXStart(), -t.getInkyYStart(), 0);
	t.drawEyes();
	t.drawInky();
	glPopMatrix();
}

void GameController::drawClyde()
{
	glPushMatrix();
	glTranslatef(o.getClydeXStart(), -o.getClydeYStart(), 0);
	o.drawEyes();
	o.drawClyde();
	glPopMatrix();
}

void GameController::drawCube()
{
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

void GameController::drawSmallPellet()
{
	glColor3f(255, 255, 255);
	glutSolidSphere(0.13f, 10, 10);
}

void GameController::drawBigPellet()
{
	glColor3f(1, 0, 0);
	glutSolidSphere(0.3f, 10, 10);
}

void GameController::drawGate()
{
	glBegin(GL_LINES);
	glColor3f(255, 255, 255);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
}

//keyboard control for game screens___________________________________________________________
void GameController::movePacMan(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:  // escape key
		exit(0);
		break;
	case 'd'://right
		if (maze[player.getPacmanGridX()][player.getPacmanGridY() + 1] != Tiles::wall && maze[player.getPacmanGridX()][player.getPacmanGridY() + 1] != Tiles::gate)
		{
			player.setTurnTo(1, 0);
			break;
		}
	case 'a'://left
		if (maze[player.getPacmanGridX()][player.getPacmanGridY() - 1] != Tiles::wall && maze[player.getPacmanGridX()][player.getPacmanGridY() - 1] != Tiles::gate)
		{
			player.setTurnTo(-1, 0);
			break;
		}
	case 'w'://up
		if (maze[player.getPacmanGridX() - 1][player.getPacmanGridY()] != Tiles::wall && maze[player.getPacmanGridX() - 1][player.getPacmanGridY()] != Tiles::gate)
		{
			player.setTurnTo(0, 1);
			break;
		}
	case 's'://down
		if (maze[player.getPacmanGridX() + 1][player.getPacmanGridY()] != Tiles::wall && maze[player.getPacmanGridX() + 1][player.getPacmanGridY()] != Tiles::gate)
		{
			player.setTurnTo(0, -1);
			break;
		}
	}
	glutPostRedisplay();
}

void GameController::keyboardMenu(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:  // escape key
		exit(0);
		break;
	case '1':
		currentState = GAME;
		break;
	case '2':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void GameController::keyboardGameOver(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:  // escape key
		exit(0);
		break;

	case '1':
		if (lives > 0 || !checkGameState())
		{
			resetGameState();
			musicPlayer.stopDeath();
			currentState = GAME;
		}
		break;

	case '2':
		exit(0);
		break;
	}

	glutPostRedisplay();
}


//timers___________________________________________________________
void GameController::frightenedDuration()
{
	if (!startFirghtened)
	{
		bigPelletTime = steady_clock::now();
		startFirghtened = true;
	}

	steady_clock::time_point currentTime = steady_clock::now();
	duration<float> elapsedDuration = currentTime - bigPelletTime;

	if (elapsedDuration.count() >= 5.0f)
	{
		player.setAteBigPellet(false);
		startFirghtened = false;
	}
}

void GameController::endScatterDuration()
{
	if (!startScatter)
	{
		steady_clock::time_point currentTime = steady_clock::now();
		duration<float> elapsedDuration = currentTime - scatterTime;

		if (elapsedDuration.count() >= 4.0f) {
			startScatter = true;
			isScatter = false;
			scatterCounter++;
			scatterTime = steady_clock::now();
		}
	}
}

void GameController::startScatterDuration()
{
	if (startScatter) 
	{
		steady_clock::time_point currentTime = steady_clock::now();
		duration<float> elapsedDuration = currentTime - scatterTime;

		if (elapsedDuration.count() >= 15.0f) {
			startScatter = false;
			isScatter = true;
			scatterTime = steady_clock::now();
		}
	}
}

//controllers for each character___________________________________________________________
void GameController::pacmanController(float deltaTime)
{
	player.updatePacman(deltaTime);

	if (player.getAteBigPellet())
	{
		frightenedDuration();
	}
}

void GameController::blinkyController(float deltaTime)
{
	r.checkCollision(player.getPacmanGridX(), player.getPacmanGridY());
	r.setPath(player.getPacmanGridX(), player.getPacmanGridY(), player.getAteBigPellet());
	r.setBlinkySpeed();
	r.updateBlinky(deltaTime);
}

void GameController::pinkyController(float deltaTime)
{
	p.checkCollision(player.getPacmanGridX(), player.getPacmanGridY());
	p.setPath(player.getPacmanGridX(), player.getPacmanGridY(), player.getAteBigPellet(), player.getPacmanTurnToX(), player.getPacmanTurnToY());
	p.setPinkySpeed();
	p.updatePinky(deltaTime);
}

void GameController::inkyController(float deltaTime)
{
	t.checkCollision(player.getPacmanGridX(), player.getPacmanGridY());
	t.setPath(player.getPacmanGridX(), player.getPacmanGridY(), player.getAteBigPellet(), player.getPacmanTurnToX(), player.getPacmanTurnToY(), r.getBlinkyGridX(), r.getBlinkyGridY());
	t.setInkySpeed();
	t.updateInky(deltaTime);
}

void GameController::clydeController(float deltaTime)
{
	o.checkCollision(player.getPacmanGridX(), player.getPacmanGridY());
	o.setPath(player.getPacmanGridX(), player.getPacmanGridY(), player.getAteBigPellet());
	o.setClydeSpeed();
	o.updateClyde(deltaTime);
}

//game state controllers___________________________________________________________
bool GameController::checkGameState()
{
	int pelletCounter = 0;

	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (maze[i][j] == Tiles::small_pellet || maze[i][j] == Tiles::big_pellet)
				pelletCounter++;
		}
	}

	if (pelletCounter == 0)
		return true;

	return false;
}

void GameController::resetGameState()
{
	//reset pacman
	player.resetPacmanStats();

	//reset blinky
	r.resetBlinkyStats();

	//reset pinky
	p.resetPinkyStats();

	//reset inky
	t.resetInkyStats();

	//reset clyde
	o.resetClydeStats();
}