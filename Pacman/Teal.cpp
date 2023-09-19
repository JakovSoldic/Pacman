#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>
#include "Teal.h"

void Inky::drawCircle(float centerX, float centerY, float radiusX, float radiusY)
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

void Inky::drawEyes()
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
	glTranslatef(inkyX, inkyY, 0);
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

void Inky::drawInky()
{
	glPushMatrix();
	glTranslatef(inkyX, inkyY, 0);

	glColor3f(1.0, 1.0, 1.0);
	drawCircle(0.20f, 0.15f, 0.15f, 0.20f);
	drawCircle(-0.20f, 0.15f, 0.15f, 0.20f);

	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(0.0f, 0.5f, 0.5f);
	drawCircle(0.0f, 0.0f, 0.45f, 0.45f);

	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(0.0f, 0.5f, 0.5f);
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


void Inky::getPath(int targetX, int targetY, int previousTileX, int previousTileY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;

		vector<BFS::Node*> path = bfsInky.bfs(inkyGridX, inkyGridY, targetX, targetY, previousTileX, previousTileY);

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
		
		getPath(inkyYStart, inkyXStart, 0, 0);

		if (inkyGridX == inkyYStart && inkyGridY == inkyXStart)
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

		if ((leftTeleporter && inkyGridX == 14 && inkyGridY == 26) || (rightTeleporter && inkyGridX == 14 && inkyGridY == 1)) {
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

		if (inkyGridX == randomGridX && inkyGridY == randomGridY)
		{
			hasReachedRandomTile = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (animationComplete)
		{
			getPath(inkyCornerX, inkyCornerY, prevGridX, prevGridY);
		}
	}

	if (!isDead && !isFrightened && !isScatter && !leftTeleporter && !rightTeleporter)
	{
		if(animationComplete)
		{
			int targetX = pacmanTargetX;
			int targetY = pacmanTargetY;

			if (pacmanDirectionX == 1)
				targetY = pacmanTargetY + 2;
			
			else if (pacmanDirectionX == -1)
				targetY = pacmanTargetY - 2;
			
			else if (pacmanDirectionY == 1)
				targetX = pacmanTargetY - 2;
			
			else if (pacmanDirectionY == -1)
				targetX = pacmanTargetY + 2;

			int doubleVectorX = 2 * targetX - blinkyGridX;
			int doubleVectorY = 2 * targetY - blinkyGridY;

			getPath(doubleVectorX, doubleVectorY, prevGridX, prevGridY);
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
			musicPlayer.playAteGhost();
		}
	}

	if (!isFrightened && !isDead)
	{
		if (inkyGridX == targetX && inkyGridY == targetY)
		{
			lives--;
			musicPlayer.stopMovementSound();
			musicPlayer.playDeath();
			currentState = GAME_OVER_MENU;
		}
	}
}

void Inky::resetInkyStats()
{
	inkyX = 0;
	inkyY = 0;
	inkyGridX = inkyYStart;
	inkyGridY = inkyXStart;
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