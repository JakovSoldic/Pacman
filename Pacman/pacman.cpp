#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <chrono>
#include <thread>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "pacman.h"
#include "globalVariables.h"

void Pacman::drawMouth()
{
	glPushMatrix();
	glTranslatef(pacmanX, pacmanY, 0);
	if (turnTo.x == 1) {
		glRotatef(0, 0, 0, 1);
	}
	if (turnTo.x == -1) {
		glRotatef(180, 0, 0, 1);
	}
	if (turnTo.y == 1) {
		glRotatef(90, 0, 0, 1);
	}
	if (turnTo.y == -1) {
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

void Pacman::drawPacMan()
{
	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(pacmanX, pacmanY, 0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 0.0);
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

void Pacman::updatePacman(float deltaTime)
{
	pacmanX = constantInterpolation(pacmanX, targetPosX, playerSpeed, deltaTime);
	if (maze[pacmanGridX][pacmanGridY + turnTo.x] != Tiles::wall && maze[pacmanGridX][pacmanGridY + turnTo.x] != Tiles::gate)
	{
		if (pacmanX == targetPosX)
		{
			pacmanGridY += turnTo.x;
			targetPosX += turnTo.x;
		}
	}

	pacmanY = constantInterpolation(pacmanY, targetPosY, playerSpeed, deltaTime);
	if (maze[pacmanGridX - turnTo.y][pacmanGridY] != Tiles::wall && maze[pacmanGridX - turnTo.y][pacmanGridY] != Tiles::gate)
	{
		if (pacmanY == targetPosY)
		{
			pacmanGridX -= turnTo.y;
			targetPosY += turnTo.y;
		}
	}

	if (maze[pacmanGridX][pacmanGridY] == Tiles::small_pellet)
	{
		maze[pacmanGridX][pacmanGridY] = Tiles::empty_space;
		score += 20;
	}

	if (maze[pacmanGridX][pacmanGridY] == Tiles::big_pellet)
	{
		maze[pacmanGridX][pacmanGridY] = Tiles::empty_space;
		ateBigPellet = true;
		score += 40;
	}

	if (maze[pacmanGridX][pacmanGridY] == Tiles::teleport_tile)
	{
		if (pacmanGridX == 14 && pacmanGridY == 0)
		{
			pacmanGridX = 14;
			pacmanGridY = 25;
			pacmanX = 13;
			targetPosX = 12;
		}
		else if (pacmanGridX == 14 && pacmanGridY == 27)
		{
			pacmanGridX = 14;
			pacmanGridY = 2;
			pacmanX = -12;
			targetPosX = -11;
		}
	}
}