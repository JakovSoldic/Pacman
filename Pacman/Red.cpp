#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "Red.h"
#include "globalVariables.h"
#include "BFS.cpp"

BFS bfs;

void drawEllipse(float centerX, float centerY, float radiusX, float radiusY)
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

	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

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
	drawEllipse(0.20f, 0.15f, 0.15f, 0.20f);//right eyeball
	drawEllipse(-0.20f, 0.15f, 0.15f, 0.20f);//left eyeball
	glBegin(GL_TRIANGLE_FAN);
	if (isFrightened)
		glColor3f(0.0, 0.0, 1.0);
	else if (isDead)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(1.0, 0.0, 0.0);
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
		glColor3f(1.0, 0.0, 0.0);
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

float Blinky::constantInterpolation(float startPoint, float endPoint, float speed, float time)
{
	float distance = endPoint - startPoint;
	float totalDuration = abs(distance) / speed;

	if (time >= totalDuration)
	{
		return endPoint;  // Reached the endpoint
	}
	else
	{
		float t = time / totalDuration;
		return startPoint + (distance * t);
	}
}

void Blinky::getPathFrightened(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;
	
	vector<BFS::Node*> path = bfs.bfs(blinkyGridX, blinkyGridY, targetX, targetY);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}
}

void Blinky::getPathDead(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;
	
	vector<BFS::Node*> path = bfs.bfs(blinkyGridX, blinkyGridY, targetX, targetY);
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

		vector<BFS::Node*> path = bfs.bfs(blinkyGridX, blinkyGridY, targetX, targetY);
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
			getPathDead(13, 13);
			hasReachedHome = false;
		}

		if (blinkyGridX == 13 && blinkyGridY == 13)
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
				randomGridX = rand() % mapWidth;
				randomGridY = rand() % mapHeight;
			} while (maze[randomGridX][randomGridY] == Tiles::wall);

			getPathFrightened(randomGridX, randomGridY);
			hasReachedTarget = false;
		}

		if (blinkyGridX == randomGridX && blinkyGridY == randomGridY)
		{
			hasReachedTarget = true;
		}
	}

	if (!isDead && !isFrightened)
		getPathChase(pacmanTargetX, pacmanTargetY);
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
			blinkyGridX = x;
			blinkyGridY = y;
			counter++;
			animationComplete = true;
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
		blinkySpeed = 4.0f;
}

void Blinky::checkCollision(int targetX, int targetY)
{
	if (isFrightened)
	{
		if (blinkyGridX == targetX && blinkyGridY == targetY)
		{
			isDead = true;
			isFrightened = false;
		}
	}

	if (!isFrightened && !isDead)
	{
		if (blinkyGridX == targetX && blinkyGridY == targetY)
		{
			std::cout << "Collision detected! Exiting the program." << std::endl;
			exit(0);
		}
	}
}

//// OVO JE TOCNO NEMOJ BRISATI
//blinkyY -= targetGridX;
//blinkyX += targetGridY;
//blinkyGridX = x;
//blinkyGridY = y;
//counter++;
//-----------------