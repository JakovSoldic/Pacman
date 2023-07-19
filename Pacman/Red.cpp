#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "Red.h"
#include "globalVariables.h"
#include "pacman.h"
#include "AStar.cpp"
#include "BFS.cpp"

void Blinky::drawBlinky()
{

	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(blinkyX, blinkyY, 0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 0.0, 0.0); // set color to red
	glVertex3f(0.0, 0.0, 0.0);
	for (int i = 0; i <= SEGMENTS; i++)
	{
		float angle = PI * 2 * (float)i / (float)SEGMENTS;
		float x = cos(angle);
		float y = sin(angle);
		glVertex3f(x * 0.5, y * 0.5, 0.0);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0); // set color to red
	glBegin(GL_POLYGON); // start drawing a polygon
	glVertex2f(-0.5f, -0.50f); // lower left vertex
	glVertex2f(0.5f, -0.50f); // lower right vertex
	glVertex2f(0.5f, 0.0f); // upper right vertex
	glVertex2f(-0.5f, 0.0f); // upper left vertex
	glEnd();
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

void Blinky::setPath(int targetX, int targetY)
{
	if (targetX != previousTargetX || targetY != previousTargetY) {
		pathCoordinates.clear();
		counter = 0;
		BFS bfs;

		vector<BFS::Node*> path = bfs.bfs(blinkyGridX, blinkyGridY, targetX, targetY);
		reverse(path.begin(), path.end());

		for (BFS::Node* node : path) {
			pathCoordinates.emplace_back(node->x, node->y);
		}

		previousTargetX = targetX;
		previousTargetY = targetY;
	}
}


void Blinky::updateBlinky(float deltaTime)
{
	if (counter < pathCoordinates.size()) 
	{
		if (animationComplete == true)
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

//// OVO JE TOCNO NEMOJ BRISATI
//blinkyY -= targetGridX;
//blinkyX += targetGridY;
//blinkyGridX = x;
//blinkyGridY = y;
//counter++;
//-----------------