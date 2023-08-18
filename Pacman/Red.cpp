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

void Blinky::drawBlinky()
{
	glPushMatrix();
	glTranslatef(blinkyX, blinkyY, 0);

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
	drawCircle(0.20f, 0.15f, 0.15f, 0.20f);//right eyeball
	drawCircle(-0.20f, 0.15f, 0.15f, 0.20f);//left eyeball

	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0, 0.0, 0.0);
	drawCircle(0.0f, 0.0f, 0.45f, 0.45f);//circle body
	
	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.4f, -0.30f); //lower left vertex
	glVertex2f(0.4f, -0.30f); //lower right vertex
	glVertex2f(0.4f, 0.0f); //upper right vertex
	glVertex2f(-0.4f, 0.0f); //upper left vertex
	glEnd();

	drawCircle(-0.30f, -0.20f, 0.15f, 0.30f);//feet left
	drawCircle(0.00f, -0.20f, 0.15f, 0.30f);//feet middle
	drawCircle(0.30f, -0.20f, 0.15f, 0.30f);//feet right

	glPopMatrix();
}

void Blinky::getPath(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;
	
	vector<BFS::Node*> path = bfsBlinky.bfs(blinkyGridX, blinkyGridY, targetX, targetY, 0, 0);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Blinky::getPathChase(int targetX, int targetY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;

		vector<BFS::Node*> path = bfsBlinky.bfs(blinkyGridX, blinkyGridY, targetX, targetY, prevGridX, prevGridY);
		reverse(path.begin(), path.end());

		for (BFS::Node* node : path) {
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
		if (hasReachedHome)
		{
			getPath(blinkyXHome, blinkyYHome);
			hasReachedHome = false;
		}

		if (blinkyGridX == blinkyXHome && blinkyGridY == blinkyYHome)
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

		if ((leftTeleporter && blinkyGridX == 14 && blinkyGridY == 26) || (rightTeleporter && blinkyGridX == 14 && blinkyGridY == 1)) {
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
				path = bfsBlinky.bfs(blinkyGridX, blinkyGridY, randomGridX, randomGridY, prevGridX, prevGridY);
			} while (maze[randomGridX][randomGridY] == Tiles::wall && path.empty());

			getPath(randomGridX, randomGridY);
			hasReachedTarget = false;
		}

		if (blinkyGridX == randomGridX && blinkyGridY == randomGridY)
		{
			hasReachedTarget = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (animationComplete)
		{
			getPathChase(blinkyCornerX, blinkyCornerY);
		}
	}

	if (!isDead && !isFrightened && !isScatter && !leftTeleporter && !rightTeleporter)
	{
		if (animationComplete)
		{
			getPathChase(pacmanTargetX, pacmanTargetY);
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
	hasReachedTarget = true;
	hasReachedHome = true;
	hasReachedTeleport = true;
	leftTeleporter = false;
	rightTeleporter = false;
}