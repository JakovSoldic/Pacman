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

BFS bfs3;

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

void Inky::getPathFrightened(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;

	vector<BFS::Node*> path = bfs3.bfs(inkyGridX, inkyGridY, targetX, targetY, 0, 0);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Inky::getPathDead(int targetX, int targetY)
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
			getPathDead(ghostHomeX, ghostHomeY);
			hasReachedHome = false;
		}

		if (inkyGridX == ghostHomeX && inkyGridY == ghostHomeY)
		{
			isDead = false;
			hasReachedTarget = true;
			hasReachedHome = true;
			isFrightened = false;
		}

	}

	if (isFrightened && !isDead)
	{
		if (hasReachedTarget)
		{
			srand(static_cast<unsigned>(time(0)));
			do
			{
				randomGridX = rand() % (mapHeight - 2) + 1;
				randomGridY = rand() % (mapWidth - 2) + 1;
			} while (maze[randomGridX][randomGridY] == Tiles::wall);

			getPathFrightened(randomGridX, randomGridY);
			hasReachedTarget = false;
		}

		if (inkyGridX == randomGridX && inkyGridY == randomGridY)
		{
			hasReachedTarget = true;
		}
	}

	if (isScatter && !isFrightened && !isDead)
	{
		if (hasReachedCorner)
		{
			getPathDead(inkyCornerX, inkyCornerY);
			hasReachedCorner = false;
		}
	}

	if (!isDead && !isFrightened && !isScatter)
	{
		//if (bfs3.countValidDirections(inkyGridX, inkyGridY) >= 3 || bfs3.isCorner(inkyGridX, inkyGridY, prevGridX, prevGridY))
		if(animationComplete)
		{
			int dx = 0;
			int dy = 0;
			int targetX = blinkyGridX;
			int targetY = blinkyGridY;

			if (pacmanDirectionX == 1)
			{
				targetY = blinkyGridY + 2;
				dx = pacmanTargetX - blinkyGridX;
				dy = pacmanTargetY - blinkyGridY;
			}
			else if (pacmanDirectionX == -1)
			{
				targetY = blinkyGridY - 2;
				dx = pacmanTargetX - blinkyGridX;
				dy = pacmanTargetY - blinkyGridY;
			}
			else if (pacmanDirectionY == 1)
			{
				targetX = blinkyGridX - 2;
				dx = pacmanTargetY - blinkyGridY;
				dy = pacmanTargetX - blinkyGridX;
			}
			else if (pacmanDirectionY == -1)
			{
				targetX = blinkyGridX + 2;
				dx = pacmanTargetY - blinkyGridY;
				dy = pacmanTargetX - blinkyGridX;
			}

			dx *= 2;
			dy *= 2;

			while (bfs3.isValidPosition(targetX, targetY) && maze[targetX][targetY] == Tiles::wall)
			{
				//if (targetX < 0 || targetX >= mapHeight || targetY < 0 || targetY >= mapWidth)
				//	break;

				if (dx > 0) 
					targetX++;
				else if (dx < 0) 
					targetX--;

				if (dy > 0) 
					targetY++;
				else if (dy < 0) 
					targetY--;
			}

			getPathChase(targetX, targetY);

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

		inkyX = constantInterpolation(inkyX, targetPosX, blinkySpeed, deltaTime);
		inkyY = constantInterpolation(inkyY, targetPosY, blinkySpeed, deltaTime);

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
}

void Inky::setInkySpeed()
{
	if (isFrightened)
		blinkySpeed = 2.0f;
	else if (isDead)
		blinkySpeed = 6.0f;
	else
		blinkySpeed = 4.0f;
}

void Inky::checkCollision(int targetX, int targetY)
{
	if (isFrightened)
	{
		if (inkyGridX == targetX && inkyGridY == targetY)
		{
			isDead = true;
			isFrightened = false;
		}
	}

	if (!isFrightened && !isDead)
	{
		if (inkyGridX == targetX && inkyGridY == targetY)
		{
			std::cout << "Collision detected! Exiting the program." << std::endl;
			exit(0);
		}
	}
}