#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>
#include "Red.h"

void Blinky::drawCircle(float centerX, float centerY, float radiusX, float radiusY)
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

void Blinky::drawEyes()
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
	glTranslatef(blinkyX, blinkyY, 0);
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

void Blinky::drawBlinky()
{
	glPushMatrix();
	glTranslatef(blinkyX, blinkyY, 0);

	glColor3f(1.0, 1.0, 1.0);
	drawCircle(0.20f, 0.15f, 0.15f, 0.20f);
	drawCircle(-0.20f, 0.15f, 0.15f, 0.20f);

	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0, 0.0, 0.0);
	drawCircle(0.0f, 0.0f, 0.45f, 0.45f);
	
	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0, 0.0, 0.0);
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


void Blinky::getPath(int targetX, int targetY, int previousTileX, int previousTileY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;

		vector<BFS::Node*> path = bfsBlinky.bfs(blinkyGridX, blinkyGridY, targetX, targetY, previousTileX, previousTileY);

		for (auto node : path) {
			pathCoordinates.emplace_back(node->x, node->y);
		}

		previousTargetX = targetX;
		previousTargetY = targetY;
	}
}

void Blinky::setPath(int pacmanTargetX, int pacmanTargetY, bool status)
{
	if(!isDead)
		isFrightened = status;
	
	if (isDead)
	{
		getPath(blinkyXHome, blinkyYHome, 0, 0);

		if (blinkyGridX == blinkyXHome && blinkyGridY == blinkyYHome)
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

		if ((leftTeleporter && blinkyGridX == 14 && blinkyGridY == 26) || (rightTeleporter && blinkyGridX == 14 && blinkyGridY == 1)) {
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

		if (blinkyGridX == randomGridX && blinkyGridY == randomGridY)
		{
			hasReachedRandomTile = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (animationComplete)
		{
			getPath(blinkyCornerX, blinkyCornerY, prevGridX, prevGridY);
		}
	}

	if (!isDead && !isFrightened && !isScatter && !leftTeleporter && !rightTeleporter)
	{
		if (animationComplete)
		{
			getPath(pacmanTargetX, pacmanTargetY, prevGridX, prevGridY);
		}
	}
}

void Blinky::updateBlinky(float deltaTime)
{
	if (counter < pathCoordinates.size()) 
	{
		if (animationComplete)
		{
			x = pathCoordinates[counter].first;
			y = pathCoordinates[counter].second;

			targetGridX = x - blinkyGridX;
			targetGridY = y - blinkyGridY;

			targetPosX = blinkyX + targetGridY;
			targetPosY = blinkyY - targetGridX;
			animationComplete = false;
		}

		blinkyX = constantInterpolation(blinkyX, targetPosX, blinkySpeed, deltaTime);
		blinkyY = constantInterpolation(blinkyY, targetPosY, blinkySpeed, deltaTime);

		if (blinkyX == targetPosX && blinkyY == targetPosY)
		{
			prevGridX = blinkyGridX;
			prevGridY = blinkyGridY;

			blinkyGridX = x;
			blinkyGridY = y;

			counter++;
			animationComplete = true;
		}

	}

	if (maze[blinkyGridX][blinkyGridY] == Tiles::home_tile)
	{
		isFrightened = false;
	}

	if (blinkyGridX == 14 && blinkyGridY == 22 && prevGridY == 21)
	{
		rightTeleporter = true;
	}

	if (blinkyGridX == 14 && blinkyGridY == 5 && prevGridY == 6)
	{
		leftTeleporter = true;
	}

	if (maze[blinkyGridX][blinkyGridY] == Tiles::teleport_tile)
	{
		if (blinkyGridX == 14 && blinkyGridY == 0)
		{
			blinkyGridX = 14;
			blinkyGridY = 26;
			blinkyX = 13;
		}
		else if (blinkyGridX == 14 && blinkyGridY == 27)
		{
			blinkyGridX = 14;
			blinkyGridY = 1;
			blinkyX = -12;
		}
	}
}

void Blinky::setBlinkySpeed()
{
	if (isFrightened)
		blinkySpeed = 2.0f;
	else if (isDead)
		blinkySpeed = 6.0f;
	else
		blinkySpeed = 5.0f;
}

void Blinky::checkCollision(int targetX, int targetY)
{
	if (isFrightened)
	{
		if (blinkyGridX == targetX && blinkyGridY == targetY)
		{
			isDead = true;
			isFrightened = false;
			score += 200;
			musicPlayer.playAteGhost();
		}
	}

	if (!isFrightened && !isDead)
	{
		if (blinkyGridX == targetX && blinkyGridY == targetY)
		{
			lives--;
			musicPlayer.stopMovementSound();
			musicPlayer.playDeath();
			currentState = GAME_OVER_MENU;
		}
	}
}

void Blinky::resetBlinkyStats()
{
	blinkyX = 0;
	blinkyY = 0;
	blinkyGridX = blinkyYStart;
	blinkyGridY = blinkyXStart;
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