#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "Pink.h"
#include "globalVariables.h"
#include "BFS.cpp"
#include "Music.h"

BFS bfs2;
Music m2;

void Pinky::drawEllipse(float centerX, float centerY, float radiusX, float radiusY)
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

void Pinky::drawPinky()
{

	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(pinkyX, pinkyY, 0);
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
		glColor3f(1.0f, 0.713f, 0.756f);
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
		glColor3f(1.0f, 0.713f, 0.756f);
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

void Pinky::getPathFrightened(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;

	vector<BFS::Node*> path = bfs2.bfs(pinkyGridX, pinkyGridY, targetX, targetY, 0, 0);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Pinky::getPathDead(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;

	vector<BFS::Node*> path = bfs2.bfs(pinkyGridX, pinkyGridY, targetX, targetY, 0, 0);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Pinky::getPathChase(int targetX, int targetY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;

		vector<BFS::Node*> path = bfs2.bfs(pinkyGridX, pinkyGridY, targetX, targetY, prevGridX, prevGridY);
		reverse(path.begin(), path.end());

		for (BFS::Node* node : path) {
			pathCoordinates.emplace_back(node->x, node->y);
		}

		previousTargetX = targetX;
		previousTargetY = targetY;
	}
}

void Pinky::setPath(int pacmanTargetX, int pacmanTargetY, bool status, int pacmanDirectionX, int pacmanDirectionY)
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

		if (pinkyGridX == ghostHomeX && pinkyGridY == ghostHomeY)
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

		if ((leftTeleporter && pinkyGridX == 14 && pinkyGridY == 26) || (rightTeleporter && pinkyGridX == 14 && pinkyGridY == 1)) {
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
				path = bfs2.bfs(pinkyGridX, pinkyGridY, randomGridX, randomGridY, prevGridX, prevGridY);
			} while (maze[randomGridX][randomGridY] == Tiles::wall && path.empty());

			getPathFrightened(randomGridX, randomGridY);
			hasReachedTarget = false;
		}

		if (pinkyGridX == randomGridX && pinkyGridY == randomGridY)
		{
			hasReachedTarget = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (hasReachedCorner)
		{
			getPathDead(pinkyCornerX, pinkyCornerY);
			hasReachedCorner = false;
		}
	}

	if (!isDead && !isFrightened && !isScatter && !leftTeleporter && !rightTeleporter)
	{
		//if (bfs2.countValidDirections(pinkyGridX, pinkyGridY) >= 3 || bfs2.isCorner(pinkyGridX, pinkyGridY, prevGridX, prevGridY))
		if (animationComplete)
		{
			int targetX = pacmanTargetX;
			int targetY = pacmanTargetY;

			if (pacmanDirectionX == 1)
			{
				targetX = pacmanTargetX;
				targetY = pacmanTargetY + 4;
			}
			else if (pacmanDirectionX == -1)
			{
				targetX = pacmanTargetX;
				targetY = pacmanTargetY - 4;
			}
			else if (pacmanDirectionY == 1)
			{
				targetX = pacmanTargetX - 4;
				targetY = pacmanTargetY;
			}
			else if (pacmanDirectionY == -1)
			{
				targetX = pacmanTargetX + 4;
				targetY = pacmanTargetY;
			}
			pair<int, int> checkFourInfront = bfs2.findClosestValidCoordinates(targetX, targetY, pinkyGridX, pinkyGridY);

			getPathChase(checkFourInfront.first, checkFourInfront.second);
		}
	}
}

void Pinky::updatePinky(float deltaTime)
{
	if (counter < pathCoordinates.size())
	{
		if (animationComplete)
		{
			x = pathCoordinates[counter].first;
			y = pathCoordinates[counter].second;

			targetGridX = x - pinkyGridX;
			targetGridY = y - pinkyGridY;

			targetPosX = pinkyX + targetGridY;
			targetPosY = pinkyY - targetGridX;
			animationComplete = false;
		}

		pinkyX = constantInterpolation(pinkyX, targetPosX, pinkySpeed, deltaTime);
		pinkyY = constantInterpolation(pinkyY, targetPosY, pinkySpeed, deltaTime);

		if (pinkyX == targetPosX && pinkyY == targetPosY)
		{
			prevGridX = pinkyGridX;
			prevGridY = pinkyGridY;

			pinkyGridX = x;
			pinkyGridY = y;

			counter++;
			animationComplete = true;
		}

	}

	if (pinkyGridX == 14 && pinkyGridY == 22 && prevGridY == 21)
	{
		rightTeleporter = true;
	}

	if (pinkyGridX == 14 && pinkyGridY == 5 && prevGridY == 6)
	{
		leftTeleporter = true;
	}

	if (maze[pinkyGridX][pinkyGridY] == Tiles::teleport_tile)
	{
		if (pinkyGridX == 14 && pinkyGridY == 0)
		{
			pinkyGridX = 14;
			pinkyGridY = 26;
			pinkyX = 13;
		}
		else if (pinkyGridX == 14 && pinkyGridY == 27)
		{
			pinkyGridX = 14;
			pinkyGridY = 1;
			pinkyX = -12;
		}
	}
}

void Pinky::setPinkySpeed()
{
	if (isFrightened)
		pinkySpeed = 2.0f;
	else if (isDead)
		pinkySpeed = 6.0f;
	else
		pinkySpeed = 3.7f;
}

void Pinky::checkCollision(int targetX, int targetY)
{
	if (isFrightened)
	{
		if (pinkyGridX == targetX && pinkyGridY == targetY)
		{
			isDead = true;
			isFrightened = false;
			score += 200;
			m2.playAteGhost();
		}
	}

	if (!isFrightened && !isDead)
	{
		if (pinkyGridX == targetX && pinkyGridY == targetY)
		{
			m2.stopMovementSound();
			m2.playDeath();
			currentState = GAME_OVER_MENU;
		}
	}
}