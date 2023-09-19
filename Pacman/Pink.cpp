#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>
#include "Pink.h"

void Pinky::drawCircle(float centerX, float centerY, float radiusX, float radiusY)
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

void Pinky::drawEyes()
{

	if (targetGridX != 0)
	{
		eyesXPos = targetGridX;
		eyesYPos = 0;
	}
	else if (targetGridY != 0)
	{
		eyesXPos = 0;
		eyesYPos = targetGridY;
	}

	glPushMatrix();
	glTranslatef(pinkyX, pinkyY, 0);
	glPointSize(4);

	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	if (eyesXPos == 0 && eyesYPos == 0)
	{
		glVertex2f(-0.2f, 0.1f);
		glVertex2f(0.2f, 0.1f);
	}
	else if (eyesXPos == 1)
	{
		glVertex2f(-0.2f, 0.0f);
		glVertex2f(0.2f, 0.0f);
	}
	else if (eyesXPos == -1)
	{
		glVertex2f(-0.2f, 0.2f);
		glVertex2f(0.2f, 0.2f);
	}
	else if (eyesYPos == 1)
	{
		glVertex2f(-0.1f, 0.1f);
		glVertex2f(0.3f, 0.1f);
	}
	else if (eyesYPos == -1)
	{
		glVertex2f(-0.3f, 0.1f);
		glVertex2f(0.1f, 0.1f);
	}

	glEnd();
	glPopMatrix();

}

void Pinky::drawPinky()
{
	glPushMatrix();
	glTranslatef(pinkyX, pinkyY, 0);

	glColor3f(1.0, 1.0, 1.0);
	drawCircle(0.20f, 0.15f, 0.15f, 0.20f);
	drawCircle(-0.20f, 0.15f, 0.15f, 0.20f);

	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0f, 0.713f, 0.756f);
	drawCircle(0.0f, 0.0f, 0.45f, 0.45f);

	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0f, 0.713f, 0.756f);
	glBegin(GL_POLYGON);
	glVertex2f(-0.4f, -0.30f);
	glVertex2f(0.4f, -0.30f);
	glVertex2f(0.4f, 0.0f);
	glVertex2f(-0.4f, 0.0f);
	glEnd();

	drawCircle(-0.30f, -0.20f, 0.15f, 0.30f);
	drawCircle(0.00f, -0.20f, 0.15f, 0.30f);
	drawCircle(0.30f, -0.20f, 0.15f, 0.30f);

	glPopMatrix();
}

void Pinky::getPath(int targetX, int targetY, int previousTileX, int previousTileY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;

		vector<BFS::Node*> path = bfsPinky.bfs(pinkyGridX, pinkyGridY, targetX, targetY, previousTileX, previousTileY);

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
		getPath(pinkyYStart, pinkyXStart, 0, 0);

		if (pinkyGridX == pinkyYStart && pinkyGridY == pinkyXStart)
		{
			isDead = false;
			hasReachedRandomTile = true;
			isFrightened = false;
		}

	}

	if ((leftTeleporter || rightTeleporter) && !isDead)
	{
		if (hasReachedTeleport)
		{
			if (leftTeleporter)
				getPath(14, 0, prevGridX, prevGridY);
			else if (rightTeleporter)
				getPath(14, 27, prevGridX, prevGridY);
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
		if (hasReachedRandomTile)
		{
			srand(static_cast<unsigned>(time(0)));
			randomGridX = rand() % mapHeight;
			randomGridY = rand() % mapWidth;
			getPath(randomGridX, randomGridY, 0, 0);
			hasReachedRandomTile = false;
		}

		if (pinkyGridX == randomGridX && pinkyGridY == randomGridY)
		{
			hasReachedRandomTile = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (animationComplete)
		{
			getPath(pinkyCornerX, pinkyCornerY, prevGridX, prevGridY);
		}
	}

	if (!isDead && !isFrightened && !isScatter && !leftTeleporter && !rightTeleporter)
	{
		if (animationComplete)
		{
			int targetX = pacmanTargetX;
			int targetY = pacmanTargetY;

			if (pacmanDirectionX == 1)
				targetY = pacmanTargetY + 4;
			
			else if (pacmanDirectionX == -1)
				targetY = pacmanTargetY - 4;
			
			else if (pacmanDirectionY == 1)
				targetX = pacmanTargetX - 4;
			
			else if (pacmanDirectionY == -1)
				targetX = pacmanTargetX + 4;

			getPath(targetX, targetY, prevGridX, prevGridY);
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

	if (maze[pinkyGridX][pinkyGridY] == Tiles::home_tile)
	{
		isFrightened = false;
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
		pinkySpeed = 4.5f;
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
			musicPlayer.playAteGhost();
		}
	}

	if (!isFrightened && !isDead)
	{
		if (pinkyGridX == targetX && pinkyGridY == targetY)
		{
			lives--;
			musicPlayer.stopMovementSound();
			musicPlayer.playDeath();
			currentState = GAME_OVER_MENU;
		}
	}
}

void Pinky::resetPinkyStats()
{
	pinkyX = 0;
	pinkyY = 0;
	pinkyGridX = pinkyYStart;
	pinkyGridY = pinkyXStart;
	prevGridX = 0;
	prevGridY = 0;
	targetGridX = 0;
	targetGridY = 0;
	targetPosX = 0;
	targetPosY = 0;
	previousTargetX = 0;
	previousTargetY = 0;
	counter = 0;
	animationComplete = true;
	isDead = false;
	isFrightened = false;
	hasReachedRandomTile = true;
	hasReachedTeleport = true;
	leftTeleporter = false;
	rightTeleporter = false;
}