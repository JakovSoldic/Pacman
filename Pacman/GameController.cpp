#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;

#include <GL/GL.h>
#include <GL/freeglut.h>
#include "GameController.h"

#include "pacman.h"
#include "Red.h"
#include "Pink.h"
#include "Teal.h"
#include "Orange.h"
#include "globalVariables.h"

Pacman player;
Blinky r;
Pinky p;
Inky t;
Clyde o;

//stuff for drawing on the screen___________________________________________________________
void GameController::drawPacman()
{
	glPushMatrix();
	glTranslatef(player.pacmanXStart, -player.pacmanYStart, 0);
	player.drawMouth();
	player.drawPacMan();
	glPopMatrix();
}

void GameController::drawBlinky()
{
	glPushMatrix();
	glTranslatef(r.blinkyXStart, -r.blinkyYStart, 0);
	r.drawBlinky();
	glPopMatrix();
}

void GameController::drawPinky()
{
	glPushMatrix();
	glTranslatef(p.pinkyXStart, -p.pinkyYStart, 0);
	p.drawPinky();
	glPopMatrix();
}

void GameController::drawInky()
{
	glPushMatrix();
	glTranslatef(t.inkyXStart, -t.inkyYStart, 0);
	t.drawInky();
	glPopMatrix();
}

void GameController::drawClyde()
{
	glPushMatrix();
	glTranslatef(o.clydeXStart, -o.clydeYStart, 0);
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
	if (!bigPelletTime.time_since_epoch().count())
	{
		bigPelletTime = chrono::steady_clock::now();
	}

	chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
	chrono::duration<float> elapsedDuration = currentTime - bigPelletTime;

	if (elapsedDuration.count() >= 5.0f)
	{
		player.ateBigPellet = false;
		bigPelletTime = std::chrono::steady_clock::time_point();
	}
}

void GameController::startScatterDuration()
{
	if (!startScatter)
	{
		steady_clock::time_point currentTime = steady_clock::now();
		duration<float> elapsedDuration = currentTime - startScatterTime;

		if (elapsedDuration.count() >= 7.0f) {
			startScatter = true;
			endScatter = false;
			isScatter = true;
			startScatterTime = steady_clock::now();
		}
	}
}

void GameController::endScatterDuration()
{
	if (startScatter && !endScatter) {
		steady_clock::time_point currentTime = steady_clock::now();
		duration<float> elapsedDuration = currentTime - endScatterTime;

		if (elapsedDuration.count() >= 5.0f) {
			endScatter = true;
			startScatter = false;
			isScatter = false;
			endScatterTime = steady_clock::now();
		}
	}
}

//controllers for each character___________________________________________________________
void GameController::pacmanController(float deltaTime)
{
	player.updatePacman(deltaTime);

	if (player.ateBigPellet)
	{
		frightenedDuration();
	}
}

void GameController::blinkyController(float deltaTime)
{
	r.checkCollision(player.pacmanGridX, player.pacmanGridY);
	r.setPath(player.pacmanGridX, player.pacmanGridY, player.ateBigPellet);
	r.setBlinkySpeed();
	r.updateBlinky(deltaTime);
}

void GameController::pinkyController(float deltaTime)
{
	p.checkCollision(player.pacmanGridX, player.pacmanGridY);
	p.setPath(player.pacmanGridX, player.pacmanGridY, player.ateBigPellet, player.turnTo.x, player.turnTo.y);
	p.setPinkySpeed();
	p.updatePinky(deltaTime);
}

void GameController::inkyController(float deltaTime)
{
	t.checkCollision(player.pacmanGridX, player.pacmanGridY);
	t.setPath(player.pacmanGridX, player.pacmanGridY, player.ateBigPellet, player.turnTo.x, player.turnTo.y, r.blinkyGridX, r.blinkyGridY);
	t.setInkySpeed();
	t.updateInky(deltaTime);
}

void GameController::clydeController(float deltaTime)
{
	o.checkCollision(player.pacmanGridX, player.pacmanGridY);
	o.setPath(player.pacmanGridX, player.pacmanGridY, player.ateBigPellet);
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
	player.pacmanGridX = player.pacmanYStart;
	player.pacmanGridY = player.pacmanXStart;
	player.targetPosX = 0;
	player.targetPosY = 0;
	player.pacmanX = 0;
	player.pacmanY = 0;
	player.turnTo = { 0, 0 };

	//reset blinky
	r.blinkyX = 0;
	r.blinkyY = 0;
	r.blinkyGridX = r.blinkyYStart;
	r.blinkyGridY = r.blinkyXStart;
	r.prevGridX = 0;
	r.prevGridY = 0;
	r.targetGridX = 0;
	r.targetGridY = 0;
	r.targetPosX = 0;
	r.targetPosY = 0;
	r.previousTargetX = 0;
	r.previousTargetY = 0;
	r.counter = 0;
	r.animationComplete = true;
	r.isDead = false;
	r.isFrightened = false;
	r.hasReachedTarget = true;
	r.hasReachedHome = true;
	r.hasReachedTeleport = true;
	r.leftTeleporter = false;
	r.rightTeleporter = false;

	//reset pinky
	p.pinkyX = 0;
	p.pinkyY = 0;
	p.pinkyGridX = p.pinkyYStart;
	p.pinkyGridY = p.pinkyXStart;
	p.prevGridX = 0;
	p.prevGridY = 0;
	p.targetGridX = 0;
	p.targetGridY = 0;
	p.targetPosX = 0;
	p.targetPosY = 0;
	p.previousTargetX = 0;
	p.previousTargetY = 0;
	p.counter = 0;
	p.animationComplete = true;
	p.isDead = false;
	p.isFrightened = false;
	p.hasReachedTarget = true;
	p.hasReachedHome = true;
	p.hasReachedTeleport = true;
	p.leftTeleporter = false;
	p.rightTeleporter = false;

	//reset inky
	t.inkyX = 0;
	t.inkyY = 0;
	t.inkyGridX = t.inkyYStart;
	t.inkyGridY = t.inkyXStart;
	t.prevGridX = 0;
	t.prevGridY = 0;
	t.targetGridX = 0;
	t.targetGridY = 0;
	t.targetPosX = 0;
	t.targetPosY = 0;
	t.previousTargetX = 0;
	t.previousTargetY = 0;
	t.counter = 0;
	t.animationComplete = true;
	t.isDead = false;
	t.isFrightened = false;
	t.hasReachedTarget = true;
	t.hasReachedHome = true;
	t.hasReachedTeleport = true;
	t.leftTeleporter = false;
	t.rightTeleporter = false;

	//reset clyde
	o.clydeX = 0;
	o.clydeY = 0;
	o.clydeGridX = o.clydeYStart;
	o.clydeGridY = o.clydeXStart;
	o.prevGridX = 0;
	o.prevGridY = 0;
	o.targetGridX = 0;
	o.targetGridY = 0;
	o.targetPosX = 0;
	o.targetPosY = 0;
	o.previousTargetX = 0;
	o.previousTargetY = 0;
	o.counter = 0;
	o.animationComplete = true;
	o.isDead = false;
	o.isFrightened = false;
	o.hasReachedTarget = true;
	o.hasReachedHome = true;
	o.hasReachedTeleport = true;
	o.leftTeleporter = false;
	o.rightTeleporter = false;
}