#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

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

#include <chrono>

std::chrono::steady_clock::time_point bigPelletTime;

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
		exit(0);
		break;
	}
	glutPostRedisplay();
}

//controllers for each character___________________________________________________________
void frightenedDuration()
{
	if (!bigPelletTime.time_since_epoch().count())
	{
		bigPelletTime = chrono::steady_clock::now();
	}

	chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	chrono::duration<float> elapsedDuration = currentTime - bigPelletTime;

	if (elapsedDuration.count() >= 5.0f)
	{
		player.ateBigPellet = false;
		bigPelletTime = std::chrono::steady_clock::time_point();
	}
}

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