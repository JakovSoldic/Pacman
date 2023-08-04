#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "Orange.h"
#include "globalVariables.h"
#include "BFS.cpp"
#include "Music.h"

BFS bfs4;
Music m4;

void Clyde::drawEllipse(float centerX, float centerY, float radiusX, float radiusY)
{
	const int SEGMENTS = 64;

	glBegin(GL_POLYGON);
	for (int i = 0; i < SEGMENTS; i++)
	{
		float angle = 2.0 * 3.1415926 * i / SEGMENTS;
		float x = centerX + radiusX * cos(angle);
		float y = centerY + radiusY * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
}

void Clyde::drawClyde()
{

	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(clydeX, clydeY, 0);
	glPointSize(4);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(-0.2f, 0.1f);//left eye
	glEnd();
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0.2f, 0.1f);//right eye
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	drawEllipse(0.20f, 0.15f, 0.15f, 0.20f);//right eyeball
	drawEllipse(-0.20f, 0.15f, 0.15f, 0.20f);//left eyeball
	glBegin(GL_TRIANGLE_FAN);
	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0, 0.65, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	for (int i = 0; i <= SEGMENTS; i++)
	{
		float angle = PI * 2 * (float)i / (float)SEGMENTS;
		float x = cos(angle);
		float y = sin(angle);
		glVertex3f(x * 0.45, y * 0.45, 0.0);
	}
	glEnd();
	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0, 0.65, 0.0);
	glBegin(GL_POLYGON); // start drawing a polygon
	glVertex2f(-0.4f, -0.30f); // lower left vertex
	glVertex2f(0.4f, -0.30f); // lower right vertex
	glVertex2f(0.4f, 0.0f); // upper right vertex
	glVertex2f(-0.4f, 0.0f); // upper left vertex
	glEnd();
	drawEllipse(-0.30f, -0.20f, 0.15f, 0.30f);//feet left
	drawEllipse(0.00f, -0.20f, 0.15f, 0.30f);//feet middle
	drawEllipse(0.30f, -0.20f, 0.15f, 0.30f);//feet right

	glPopMatrix();
}

bool Clyde::isWithinRadius(int pacmanGridX, int pacmanGridY, int clydeGridX, int clydeGridY, int radius)
{
	int distanceX = abs(pacmanGridX - clydeGridX);
	int distanceY = abs(pacmanGridY - clydeGridY);
	int distance = distanceX + distanceY;

	return distance <= radius;
}

void Clyde::getPathFrightened(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;

	vector<BFS::Node*> path = bfs4.bfs(clydeGridX, clydeGridY, targetX, targetY, 0, 0);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Clyde::getPathDead(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;

	vector<BFS::Node*> path = bfs4.bfs(clydeGridX, clydeGridY, targetX, targetY, 0, 0);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Clyde::getPathChase(int targetX, int targetY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;

		vector<BFS::Node*> path = bfs4.bfs(clydeGridX, clydeGridY, targetX, targetY, prevGridX, prevGridY);
		reverse(path.begin(), path.end());

		for (BFS::Node* node : path) {
			pathCoordinates.emplace_back(node->x, node->y);
		}

		previousTargetX = targetX;
		previousTargetY = targetY;
	}
}

void Clyde::setPath(int pacmanTargetX, int pacmanTargetY, bool status)
{
	if (!isDead)
		isFrightened = status;

	if (isDead)
	{
		if (hasReachedHome)
		{
			getPathDead(ghostHomeX, ghostHomeY);
			hasReachedHome = false;
		}

		if (clydeGridX == ghostHomeX && clydeGridY == ghostHomeY)
		{
			isDead = false;
			hasReachedTarget = true;
			hasReachedHome = true;
			isFrightened = false;
		}

	}

	if ((leftTeleporter || rightTeleporter) && !isDead)
	{
		if (hasReachedTeleport)
		{
			if (leftTeleporter)
				getPathDead(14, 0);
			else if (rightTeleporter)
				getPathDead(14, 27);
			hasReachedTeleport = false;
		}

		if ((leftTeleporter && clydeGridX == 14 && clydeGridY == 26) || (rightTeleporter && clydeGridX == 14 && clydeGridY == 1)) {
			hasReachedTeleport = true;
			leftTeleporter = false;
			rightTeleporter = false;
		}
	}

	if (isFrightened && !isDead)
	{
		if (hasReachedTarget)
		{
			srand(static_cast<unsigned>(time(0)));
			vector<BFS::Node*> path;
			do
			{
				randomGridX = rand() % (mapHeight - 2) + 1;
				randomGridY = rand() % (mapWidth - 2) + 1;
				path = bfs4.bfs(clydeGridX, clydeGridY, randomGridX, randomGridY, prevGridX, prevGridY);
			} while (maze[randomGridX][randomGridY] == Tiles::wall && path.empty());

			getPathFrightened(randomGridX, randomGridY);
			hasReachedTarget = false;
		}

		if (clydeGridX == randomGridX && clydeGridY == randomGridY)
		{
			hasReachedTarget = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (hasReachedCorner)
		{
			getPathDead(clydeCornerX, clydeCornerY);
			hasReachedCorner = false;
		}
	}

	if (!isDead && !isFrightened && !isScatter && !leftTeleporter && !rightTeleporter)
	{
		if (animationComplete)
		{
			if (isWithinRadius(pacmanTargetX, pacmanTargetY, clydeGridX, clydeGridY, radius))
			{
				getPathChase(clydeCornerX, clydeCornerY);
			}
			else
			{
				getPathChase(pacmanTargetX, pacmanTargetY);
			}
		}
	}
}

void Clyde::updateClyde(float deltaTime)
{
	if (counter < pathCoordinates.size())
	{
		if (animationComplete)
		{
			x = pathCoordinates[counter].first;
			y = pathCoordinates[counter].second;

			targetGridX = x - clydeGridX;
			targetGridY = y - clydeGridY;

			targetPosX = clydeX + targetGridY;
			targetPosY = clydeY - targetGridX;
			animationComplete = false;
		}

		clydeX = constantInterpolation(clydeX, targetPosX, clydeSpeed, deltaTime);
		clydeY = constantInterpolation(clydeY, targetPosY, clydeSpeed, deltaTime);

		if (clydeX == targetPosX && clydeY == targetPosY)
		{
			prevGridX = clydeGridX;
			prevGridY = clydeGridY;

			clydeGridX = x;
			clydeGridY = y;

			counter++;
			animationComplete = true;
		}

	}

	if (clydeGridX == 14 && clydeGridY == 22 && prevGridY == 21)
	{
		rightTeleporter = true;
	}

	if (clydeGridX == 14 && clydeGridY == 5 && prevGridY == 6)
	{
		leftTeleporter = true;
	}

	if (maze[clydeGridX][clydeGridY] == Tiles::teleport_tile)
	{
		if (clydeGridX == 14 && clydeGridY == 0)
		{
			clydeGridX = 14;
			clydeGridY = 26;
			clydeX = 13;
		}
		else if (clydeGridX == 14 && clydeGridY == 27)
		{
			clydeGridX = 14;
			clydeGridY = 1;
			clydeX = -12;
		}
	}
}

void Clyde::setClydeSpeed()
{
	if (isFrightened)
		clydeSpeed = 2.0f;
	else if (isDead)
		clydeSpeed = 6.0f;
	else
		clydeSpeed = 3.1f;
}

void Clyde::checkCollision(int targetX, int targetY)
{
	if (isFrightened)
	{
		if (clydeGridX == targetX && clydeGridY == targetY)
		{
			isDead = true;
			isFrightened = false;
			score += 200;
			m4.playAteGhost();
		}
	}

	if (!isFrightened && !isDead)
	{
		if (clydeGridX == targetX && clydeGridY == targetY)
		{
			m4.stopMovementSound();
			m4.playDeath();
			currentState = GAME_OVER_MENU;
		}
	}
}