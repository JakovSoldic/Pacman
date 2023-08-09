#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "Teal.h"
#include "globalVariables.h"
#include "BFS.cpp"
#include "Music.h"

BFS bfs3;
Music m3;

void Inky::drawEllipse(float centerX, float centerY, float radiusX, float radiusY)
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

void Inky::drawInky()
{

	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(inkyX, inkyY, 0);
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
		glColor3f(0.0f, 0.5f, 0.5f);
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
		glColor3f(0.0f, 0.5f, 0.5f);
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

void Inky::getPath(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;

	vector<BFS::Node*> path = bfs3.bfs(inkyGridX, inkyGridY, targetX, targetY, 0, 0);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Inky::getPathChase(int targetX, int targetY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;

		vector<BFS::Node*> path = bfs3.bfs(inkyGridX, inkyGridY, targetX, targetY, prevGridX, prevGridY);
		reverse(path.begin(), path.end());

		for (BFS::Node* node : path) {
			pathCoordinates.emplace_back(node->x, node->y);
		}

		previousTargetX = targetX;
		previousTargetY = targetY;
	}
}

void Inky::setPath(int pacmanTargetX, int pacmanTargetY, bool status, int pacmanDirectionX, int pacmanDirectionY, int blinkyGridX, int blinkyGridY)
{
	if (!isDead)
		isFrightened = status;

	if (isDead)
	{
		if (hasReachedHome)
		{
			getPath(inkyYStart, inkyXStart);
			hasReachedHome = false;
		}

		if (inkyGridX == inkyYStart && inkyGridY == inkyXStart)
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
				getPath(14, 0);
			else if (rightTeleporter)
				getPath(14, 27);
			hasReachedTeleport = false;
		}

		if ((leftTeleporter && inkyGridX == 14 && inkyGridY == 26) || (rightTeleporter && inkyGridX == 14 && inkyGridY == 1)) {
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
				path = bfs3.bfs(inkyGridX, inkyGridY, randomGridX, randomGridY, prevGridX, prevGridY);
			} while (maze[randomGridX][randomGridY] == Tiles::wall && path.empty());

			getPath(randomGridX, randomGridY);
			hasReachedTarget = false;
		}

		if (inkyGridX == randomGridX && inkyGridY == randomGridY)
		{
			hasReachedTarget = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (animationComplete)
		{
			getPathChase(inkyCornerX, inkyCornerY);
		}
	}

	if (!isDead && !isFrightened && !isScatter && !leftTeleporter && !rightTeleporter)
	{
		if(animationComplete)
		{
			int dx = 0;
			int dy = 0;
			int targetX = blinkyGridX;
			int targetY = blinkyGridY;

			if (pacmanDirectionX == 1)
			{
				targetY = blinkyGridY + 2;
				dx = 2 * pacmanTargetX - blinkyGridX;
				dy = 2 * targetY - blinkyGridY;
			}
			else if (pacmanDirectionX == -1)
			{
				targetY = blinkyGridY - 2;
				dx = 2 * pacmanTargetX - blinkyGridX;
				dy = 2 * targetY - blinkyGridY;
			}
			else if (pacmanDirectionY == 1)
			{
				targetX = blinkyGridX - 2;
				dx = 2 * targetX - blinkyGridY;
				dy = 2 * pacmanTargetX - blinkyGridX;
			}
			else if (pacmanDirectionY == -1)
			{
				targetX = blinkyGridX + 2;
				dx = 2 * targetX - blinkyGridY;
				dy = 2 * pacmanTargetX - blinkyGridX;
			}

			pair<int, int> checkDoubleVector = bfs3.findClosestValidCoordinates(dx, dy, blinkyGridX, blinkyGridY);

			getPathChase(checkDoubleVector.first, checkDoubleVector.second);
		}
	}
}

void Inky::updateInky(float deltaTime)
{
	if (counter < pathCoordinates.size())
	{
		if (animationComplete)
		{
			x = pathCoordinates[counter].first;
			y = pathCoordinates[counter].second;

			targetGridX = x - inkyGridX;
			targetGridY = y - inkyGridY;

			targetPosX = inkyX + targetGridY;
			targetPosY = inkyY - targetGridX;
			animationComplete = false;
		}

		inkyX = constantInterpolation(inkyX, targetPosX, inkySpeed, deltaTime);
		inkyY = constantInterpolation(inkyY, targetPosY, inkySpeed, deltaTime);

		if (inkyX == targetPosX && inkyY == targetPosY)
		{
			prevGridX = inkyGridX;
			prevGridY = inkyGridY;

			inkyGridX = x;
			inkyGridY = y;

			counter++;
			animationComplete = true;
		}

	}

	if (maze[inkyGridX][inkyGridY] == Tiles::home_tile)
	{
		isFrightened = false;
	}

	if (inkyGridX == 14 && inkyGridY == 22 && prevGridY == 21)
	{
		rightTeleporter = true;
	}

	if (inkyGridX == 14 && inkyGridY == 5 && prevGridY == 6)
	{
		leftTeleporter = true;
	}

	if (maze[inkyGridX][inkyGridY] == Tiles::teleport_tile)
	{
		if (inkyGridX == 14 && inkyGridY == 0)
		{
			inkyGridX = 14;
			inkyGridY = 26;
			inkyX = 12;
		}
		else if (inkyGridX == 14 && inkyGridY == 27)
		{
			inkyGridX = 14;
			inkyGridY = 1;
			inkyX = -13;
		}
	}
}

void Inky::setInkySpeed()
{
	if (isFrightened)
		inkySpeed = 2.0f;
	else if (isDead)
		inkySpeed = 6.0f;
	else
		inkySpeed = 4.0f;
}

void Inky::checkCollision(int targetX, int targetY)
{
	if (isFrightened)
	{
		if (inkyGridX == targetX && inkyGridY == targetY)
		{
			isDead = true;
			isFrightened = false;
			score += 200;
			m3.playAteGhost();
		}
	}

	if (!isFrightened && !isDead)
	{
		if (inkyGridX == targetX && inkyGridY == targetY)
		{
			lives--;
			m3.stopMovementSound();
			m3.playDeath();
			currentState = GAME_OVER_MENU;
		}
	}
}